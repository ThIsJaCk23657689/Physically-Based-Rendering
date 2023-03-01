#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "Core/IResource.hpp"
#include "Core/Texture.hpp"

class ICommandList : public IResource {

};

class IGraphics : public IResource {

};

class IGraphicsAndCommandList : public IGraphics, public ICommandList {

};

class Graphics : public IGraphicsAndCommandList {
public:
    TextureHandle CreateTexture(const TextureDesc& desc);

protected:
    ~Graphics();

private:
    Graphics& operator=(const Graphics& other); // undefined (Singleton)

};

#endif