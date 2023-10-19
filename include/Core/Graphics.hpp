#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <cstddef>
#include <glm/glm.hpp>
#include "Core/Buffer/IBuffer.hpp"
#include "Core/Buffer/IVertexArray.hpp"
#include "Core/IResource.hpp"
#include "Core/MemoryManager/RefCountPtr.hpp"
#include "Core/MemoryManager/RefCounter.hpp"
#include "Core/Texture/ITexture.hpp"

// 算是專門針對 OpenGL 操作的函式
class IGraphics : public IResource {
public:
    // utility
    virtual void SetViewport(const int& x, const int& y, const size_t& width, const size_t& height) = 0;
    virtual void ClearCache(const glm::vec4& color) = 0;
    virtual void Draw(IVertexArray* vao, ITexture* texture, const size_t& vertexCount) = 0;
    virtual void DrawIndexed(IVertexArray* vao, ITexture* texture, const size_t& indexCount) = 0;

    // create shader

    // create buffer (vao, vbo, ebo)
    virtual VertexArrayHandle CreateVertexArray(const VertexArrayDesc& desc) = 0;
    virtual BufferHandle CreateBuffer(const BufferDesc& desc) = 0;

    virtual void WriteBuffer(IBuffer* b, const void* data, size_t dataSize) = 0;
    virtual void BindVertexBuffer(IVertexArray* vao, const uint32_t& bindingIndex, IBuffer* vbo) = 0;
    virtual void BindIndexBuffer(IVertexArray* vao, IBuffer* ebo) = 0;
    virtual void BindAttributePtr(IVertexArray* vao,
                                  uint32_t attributeIndex,
                                  uint32_t bindingIndex,
                                  int32_t attributeStride,
                                  uint32_t relativeOffset) = 0;

    // create texture
    virtual TextureHandle CreateTexture(const TextureDesc& desc, const void* data) = 0;

    // create sampler

    // create framebuffer

    // create Graphics Pipeline

    // create Compute Pipeline

protected:
    ~IGraphics() override = default;

private:
    IGraphics& operator=(const IGraphics& other) = delete;  // undefined
};
typedef RefCountPtr<IGraphics> GraphicsHandle;

class Graphics : public RefCounter<IGraphics> {
public:
    Graphics();
    ~Graphics() override;

    // IResource implementation
    // virtual GLObject GetGLObject(GLObjectType objectType) override;

    void Open();
    void Close();
    IGraphics* GetGraphics() { return this; }

    void SetViewport(const int& x, const int& y, const size_t& width, const size_t& height) override;
    void ClearCache(const glm::vec4& color) override;
    void Draw(IVertexArray* vao, ITexture* texture, const size_t& vertexCount) override;
    void DrawIndexed(IVertexArray* vao, ITexture* texture, const size_t& indexCount) override;

    VertexArrayHandle CreateVertexArray(const VertexArrayDesc& desc) override;
    BufferHandle CreateBuffer(const BufferDesc& desc) override;
    void WriteBuffer(IBuffer* b, const void* data, size_t dataSize) override;
    void BindVertexBuffer(IVertexArray* vao, const uint32_t& bindingIndex, IBuffer* vbo) override;
    void BindIndexBuffer(IVertexArray* vao, IBuffer* ebo) override;
    void BindAttributePtr(IVertexArray* vao,
                          uint32_t attributeIndex,
                          uint32_t bindingIndex,
                          int32_t attributeStride,
                          uint32_t relativeOffset) override;

    TextureHandle CreateTexture(const TextureDesc& desc, const void* data) override;

protected:
private:
    Graphics& operator=(const Graphics& other);  // undefined (Singleton)
};
#endif