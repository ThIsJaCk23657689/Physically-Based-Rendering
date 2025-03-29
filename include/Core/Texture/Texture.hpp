#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Core/Graphics.hpp"
#include "Core/MemoryManager/RefCounter.hpp"
#include "Core/Texture/ITexture.hpp"

class Graphics;

class Texture : public RefCounter< ITexture >
{
public:
    Texture( Graphics* _parent ) : parent( _parent ), id( 0 ) {}
    const TextureDesc& GetDesc() const override { return desc; }
    const unsigned int GetID() const override { return id; }
    GLObject GetGLObject( GLObjectType objectType ) override;

    Graphics* parent;
    TextureDesc desc;
    unsigned int id;
    // resource
    // SRV
    // RTV
    // DSV
    // UAV
};

#endif
