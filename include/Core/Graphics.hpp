#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "Core/IResource.hpp"
#include "Core/Texture/ITexture.hpp"
#include "Core/MemoryManager/RefCounter.hpp"
#include "Core/MemoryManager/RefCountPtr.hpp"

class IGraphics;

// 還不太確定用途
class ICommandList : public IResource {
public:
    virtual void Open() = 0;
    virtual void Close() = 0;

    // return the owning "Graphics", does NOT call AddRef on it.
    virtual IGraphics* GetGraphics() = 0;

private:
    ICommandList& operator=(const ICommandList& other); // undefined

};
typedef RefCountPtr<ICommandList> CommandListHandle;



// 算是專門針對 OpenGL 操作的函式
class IGraphics : public IResource {
public:
    // create buffer (vao, vbo, ebo)
    // virtual BufferHandle CreateBuffer() = 0;

    // create shader

    // create texture
    virtual TextureHandle CreateTexture(const TextureDesc& desc, const void* data) = 0;

    // create sampler

    // create framebuffer

    // create Graphics Pipeline

    // create Compute Pipeline

    // create CommandList

protected:
    ~IGraphics() override = default;

private:
    IGraphics& operator=(const IGraphics& other) = delete; // undefined

};
typedef RefCountPtr<IGraphics> GraphicsHandle;



class IGraphicsAndCommandList : public IGraphics, public ICommandList {
};



class Graphics : public RefCounter<IGraphicsAndCommandList> {
public:
    Graphics();
    ~Graphics() override;

    // IResource implementation
    // virtual GLObject GetGLObject(GLObjectType objectType) override;

    // ICommandList implementation
    void Open() override;
    void Close() override;
    IGraphics* GetGraphics() override { return this; }

    // IGraphics implementation
    TextureHandle CreateTexture(const TextureDesc& desc, const void* data) override;

protected:


private:
    Graphics& operator=(const Graphics& other); // undefined (Singleton)

};
#endif