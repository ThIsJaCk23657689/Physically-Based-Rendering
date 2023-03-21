#include "Core/Graphics.hpp"

#include <glad/glad.h>

#include "Core/Texture/Texture.hpp"
#include "Utility/Log.hpp"

Graphics::Graphics() {
}

Graphics::~Graphics() noexcept {
}

void Graphics::Open() {
    // clear state ?
}

void Graphics::Close() {
    // clear state ?
}

TextureHandle Graphics::CreateTexture(const TextureDesc& desc, const void* data) {
    
    switch (desc.dimension) {
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
            Log::Error("Can't create a texture of unknown dimension.");
            return nullptr;
    }

    if (desc.debugName) {
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
    unsigned int textureID;
    unsigned int width = desc.width;
    unsigned int height = desc.height;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
        glTextureStorage2D(textureID, 1, GL_RGB8, width, height);
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

        glGenerateMipmap(textureID);

//        glCreateSamplers(1, &m_Sampler);
//        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//        glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glBindSampler(0, m_Sampler);
    }
    auto* texture = new Texture(this);
    texture->desc = desc;
    texture->id = textureID;
    return TextureHandle::Create(texture);
}