#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <memory>
#include "Core/IResource.hpp"

enum class TextureDimension {
    Unknown,
    Texture1D,
    Texture1DArray,
    Texture2D,
    Texture2DArray,
    Texture2DMS,
    Texture2DMSArray,
    Texture3D,
    TextureCube,
    TextureCubeArray,
    TextureRectangle
};

struct TextureDesc {
    std::uint32_t width = 1;
    std::uint32_t height = 1;
    std::uint32_t depth = 1;
    std::uint32_t arraySize = 1;
    std::uint32_t mipLevels = 1;
    std::uint32_t sampleCount = 1;
    std::uint32_t sampleQuality = 0;
    // Format format = Format::UNKNOW;
    TextureDimension dimension = TextureDimension::Texture2D;
    const char* debugName = nullptr;
};

struct ITexture : public IResource {
    virtual const TextureDesc& GetDesc() const = 0;
};

typedef std::shared_ptr<ITexture> TextureHandle;

#endif