#include "Core/Texture/TextureCache.hpp"

#include "Utility/ImageLoader.hpp"
#include "Utility/Log.hpp"

TextureCache::TextureCache( GraphicsHandle graphics ) : m_Graphics( graphics ) {}

TextureCache::~TextureCache()
{
    Reset();
}

void TextureCache::Reset()
{
    std::lock_guard< std::mutex > guard( m_LoadedTexturesMutex );
    m_LoadedTextures.clear();
    m_TexturesRequested = 0;
    m_TexturesLoaded = 0;
}

std::shared_ptr< LoadedTexture > TextureCache::LoadTextureFromFile( const std::filesystem::path& path, bool sRGB )
{
    std::shared_ptr< TextureData > texture;

    if ( FindTextureInCache( path, texture ) )
    {
        return texture;
    }

    texture->forceSRGB = sRGB;
    texture->relativePath = path.generic_string();

    if ( FillTextureData( path, texture ) )
    {
        FinalizeTexture( texture );
        SendTextureLoadedMessage( texture );
    }

    return texture;
}

std::shared_ptr< LoadedTexture > TextureCache::LoadTextureFromFileDeferred( const std::filesystem::path& path,
                                                                            bool sRGB )
{
    std::shared_ptr< TextureData > texture;

    if ( FindTextureInCache( path, texture ) )
    {
        return texture;
    }

    texture->forceSRGB = sRGB;
    texture->relativePath = path.generic_string();

    if ( FillTextureData( path, texture ) )
    {
        std::lock_guard< std::mutex > guard( m_TexturesToFinalizeMutex );
        m_TexturesToFinalize.push( texture );
        SendTextureLoadedMessage( texture );
    }

    ++m_TexturesLoaded;
    return texture;
}

void TextureCache::ProcessRenderingThreadCommands( float timeLimitMilliseconds )
{
    using namespace std::chrono;

    time_point< high_resolution_clock > startTime = high_resolution_clock::now();

    std::uint32_t commandsExecuted = 0;
    while ( true )
    {
        std::shared_ptr< TextureData > pTexture;
        if ( timeLimitMilliseconds > 0 )
        {
            time_point< high_resolution_clock > now = high_resolution_clock::now();
            if ( float( duration_cast< microseconds >( now - startTime ).count() ) > timeLimitMilliseconds * 1e3 )
            {
                break;
            }
        }

        {
            std::lock_guard< std::mutex > guard( m_TexturesToFinalizeMutex );
            if ( m_TexturesToFinalize.empty() )
            {
                break;
            }

            pTexture = m_TexturesToFinalize.front();
            m_TexturesToFinalize.pop();
        }

        if ( pTexture->data )
        {
            commandsExecuted += 1;

            FinalizeTexture( pTexture );
        }
    }
}

void TextureCache::LoadingFinished() {}

bool TextureCache::FindTextureInCache( const std::filesystem::path& path, std::shared_ptr< TextureData >& texture )
{
    std::lock_guard< std::mutex > guard( m_LoadedTexturesMutex );

    texture = m_LoadedTextures[ path.generic_string() ];
    if ( texture )
    {
        return true;
    }

    // Allocate a new texture slot for this file name and return it. Load the file later in a thread pool.
    texture = std::make_shared< TextureData >();
    m_LoadedTextures[ path.generic_string() ] = texture;

    ++m_TexturesRequested;

    return false;
}

bool TextureCache::FillTextureData( const std::filesystem::path& path, std::shared_ptr< TextureData >& texture )
{
    auto extension = path.extension();

    int width = 0, height = 0, originalChannels, channels = 0;
    stbi_set_flip_vertically_on_load( true );
    unsigned char* bitmap = stbi_load( texture->relativePath.c_str(), &width, &height, &originalChannels, 0 );

    texture->width = width;
    texture->height = height;
    texture->isFramebuffer = false;
    texture->mipLevels = 1;
    texture->dimension = TextureDimension::Texture2D;
    texture->data = std::make_shared< StbImageBlob >( bitmap );
    bitmap = nullptr;

    return true;
}

void TextureCache::FinalizeTexture( std::shared_ptr< TextureData > texture )
{
    assert( texture->data );

    const char* dataPointer = static_cast< const char* >( texture->data->data() );

    // Create TextureDesc
    TextureDesc textureDesc;
    textureDesc.width = texture->width;
    textureDesc.height = texture->height;
    textureDesc.depth = texture->depth;
    textureDesc.debugName = texture->relativePath.c_str();

    // Send it to Graphics
    texture->texture = m_Graphics->CreateTexture( textureDesc, dataPointer );
}

void TextureCache::SendTextureLoadedMessage( std::shared_ptr< TextureData > texture )
{
    Log::Info( "Loaded %d x %d: %s", texture->width, texture->height, texture->relativePath.c_str() );
}
