#ifndef TEXTURECACHE_HPP
#define TEXTURECACHE_HPP

#include <atomic>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

#include "Core/Graphics.hpp"
#include "Core/Texture/ITexture.hpp"
#include "FileSystem/Blob.hpp"

struct LoadedTexture
{
    TextureHandle texture;
    std::string relativePath;
};

struct TextureData : public LoadedTexture
{
    std::shared_ptr< IBlob > data;
    // format
    std::uint32_t width = 1;
    std::uint32_t height = 1;
    std::uint32_t depth = 1;
    std::uint32_t arraySize = 1;
    std::uint32_t mipLevels = 1;
    TextureDimension dimension = TextureDimension::Texture2D;
    bool isFramebuffer = false;
    bool forceSRGB = false;
};

class TextureCache
{
public:
    TextureCache( GraphicsHandle graphics );
    ~TextureCache();

    // Release all cached textures
    void Reset();

    std::shared_ptr< LoadedTexture > LoadTextureFromFile( const std::filesystem::path& path, bool sRGB );

    // Synchronous read and decode, deferred upload and mip generation
    std::shared_ptr< LoadedTexture > LoadTextureFromFileDeferred( const std::filesystem::path& path, bool sRGB );

    void ProcessRenderingThreadCommands( float timeLimitMilliseconds );
    void LoadingFinished();

protected:
    bool FindTextureInCache( const std::filesystem::path& path, std::shared_ptr< TextureData >& texture );
    bool FillTextureData( const std::filesystem::path& path, std::shared_ptr< TextureData >& texture );
    void FinalizeTexture( std::shared_ptr< TextureData > texture );
    void SendTextureLoadedMessage( std::shared_ptr< TextureData > texture );

    GraphicsHandle m_Graphics;
    std::mutex m_LoadedTexturesMutex;
    std::map< std::string, std::shared_ptr< TextureData > > m_LoadedTextures;

    std::mutex m_TexturesToFinalizeMutex;
    std::queue< std::shared_ptr< TextureData > > m_TexturesToFinalize;

    std::atomic< uint32_t > m_TexturesRequested = 0;
    std::atomic< uint32_t > m_TexturesLoaded = 0;
};

#endif
