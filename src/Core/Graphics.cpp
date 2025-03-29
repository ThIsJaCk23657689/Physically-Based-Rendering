#include "Core/Graphics.hpp"

#include <glad/glad.h>

#include "Core/Buffer/Buffer.hpp"
#include "Core/Buffer/VertexArray.hpp"
#include "Core/Texture/Texture.hpp"
#include "Utility/Log.hpp"

Graphics::Graphics() {}

Graphics::~Graphics() noexcept {}

void Graphics::Open()
{
    // clear state ?
}

void Graphics::Close()
{
    // clear state ?
}

void Graphics::SetViewport( const int& x, const int& y, const size_t& width, const size_t& height )
{
    glViewport( x, y, static_cast< GLsizei >( width ), static_cast< GLsizei >( height ) );
}

void Graphics::ClearCache( const glm::vec4& color )
{
    glClearColor( color.r, color.g, color.b, color.a );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Graphics::Draw( IVertexArray* vao, ITexture* texture, const size_t& vertexCount )
{
    glBindVertexArray( vao->GetID() );
    glBindTextureUnit( 0, texture->GetID() );

    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    glDrawArrays( GL_TRIANGLES, 0, static_cast< GLsizei >( vertexCount ) );
}

void Graphics::DrawIndexed( IVertexArray* vao, ITexture* texture, const size_t& indexCount )
{
    glBindVertexArray( vao->GetID() );
    glBindTextureUnit( 0, texture->GetID() );

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_Texture);

    glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( indexCount ), GL_UNSIGNED_INT, nullptr );
}

VertexArrayHandle Graphics::CreateVertexArray( const VertexArrayDesc& desc )
{
    unsigned int newVAO = 0;
    glCreateVertexArrays( 1, &newVAO );

    auto* vertexArray = new VertexArray( this );
    vertexArray->desc = desc;
    vertexArray->id = newVAO;
    return VertexArrayHandle::Create( vertexArray );
}

BufferHandle Graphics::CreateBuffer( const BufferDesc& desc )
{
    unsigned int bufferID = 0;
    glCreateBuffers( 1, &bufferID );

    auto* buffer = new Buffer( this );
    buffer->desc = desc;
    buffer->id = bufferID;
    return BufferHandle::Create( buffer );
}

void Graphics::WriteBuffer( IBuffer* b, const void* data, size_t dataSize )
{
    const auto& desc = b->GetDesc();

    int bufferFlag = GL_MAP_WRITE_BIT;
    switch ( desc.cpuAccess )
    {
        case CpuAccessMode::Read:
            bufferFlag = GL_MAP_READ_BIT;
        case CpuAccessMode::Write:
            bufferFlag = GL_MAP_WRITE_BIT;
            break;
        case CpuAccessMode::None:
        default:
            assert( 0 );
            break;
    }

    glNamedBufferStorage( b->GetID(), static_cast< GLsizeiptr >( dataSize ), data, bufferFlag );
}

void Graphics::BindVertexBuffer( IVertexArray* vao, const uint32_t& bindingIndex, IBuffer* vbo )
{
    if ( !vbo )
    {
        assert( 0 );
    }

    const auto& vboDesc = vbo->GetDesc();
    auto offset = vboDesc.dataOffset;
    auto stride = vboDesc.structStride;

    glVertexArrayVertexBuffer( vao->GetID(), bindingIndex, vbo->GetID(), offset, stride );
}

void Graphics::BindIndexBuffer( IVertexArray* vao, IBuffer* ebo )
{
    if ( !ebo )
    {
        assert( 0 );
    }

    glVertexArrayElementBuffer( vao->GetID(), ebo->GetID() );
}

void Graphics::BindAttributePtr( IVertexArray* vao,
                                 uint32_t attributeIndex,
                                 uint32_t bindingIndex,
                                 int32_t attributeStride,
                                 uint32_t relativeOffset )
{
    const auto& desc = vao->GetDesc();

    glVertexArrayAttribFormat( vao->GetID(), attributeIndex, attributeStride, GL_FLOAT, GL_FALSE, relativeOffset );
    glVertexArrayAttribBinding( vao->GetID(), attributeIndex, bindingIndex );
    glEnableVertexArrayAttrib( vao->GetID(), attributeIndex );
}

TextureHandle Graphics::CreateTexture( const TextureDesc& desc, const void* data )
{
    switch ( desc.dimension )
    {
        case TextureDimension::Texture1D:
        case TextureDimension::Texture1DArray:
            break;
        case TextureDimension::Texture2D:
        case TextureDimension::Texture2DArray:
        case TextureDimension::TextureCube:
        case TextureDimension::TextureCubeArray:
        case TextureDimension::Texture2DMS:
        case TextureDimension::Texture2DMSArray:
            break;
        case TextureDimension::Texture3D:
            break;
        default:
            Log::Error( "Can't create a texture of unknown dimension." );
            return nullptr;
    }

    if ( desc.debugName )
    {
        // SetDebugName
    }

    // OpenGL 3.3
    {
        //        glGenTextures(1, &m_Texture);
        //        glBindTexture(GL_TEXTURE_2D, m_Texture);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // OpenGL 4.5
    unsigned int textureID = 0;
    unsigned int width = desc.width;
    unsigned int height = desc.height;
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    {
        glCreateTextures( GL_TEXTURE_2D, 1, &textureID );
        glTextureStorage2D( textureID, 1, GL_RGB8, width, height );
        glTextureSubImage2D( textureID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );

        glTextureParameteri( textureID, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTextureParameteri( textureID, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTextureParameteri( textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTextureParameteri( textureID, GL_TEXTURE_MAG_FILTER, GL_REPEAT );

        glGenerateMipmap( textureID );

        //        glCreateSamplers(1, &m_Sampler);
        //        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        //        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //        glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //        glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //        glBindSampler(0, m_Sampler);
    }
    auto* texture = new Texture( this );
    texture->desc = desc;
    texture->id = textureID;
    return TextureHandle::Create( texture );
}
