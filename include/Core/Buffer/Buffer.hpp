#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Core/Buffer/IBuffer.hpp"
#include "Core/Graphics.hpp"
#include "Core/MemoryManager/RefCounter.hpp"

class Graphics;

class Buffer : public RefCounter< IBuffer >
{
public:
    explicit Buffer( Graphics* _parent ) : parent( _parent ), id( 0 ) {}
    const BufferDesc& GetDesc() const override { return desc; }
    unsigned int GetID() const override { return id; }
    GLObject GetGLObject( GLObjectType objectType ) override;

    Graphics* parent;
    BufferDesc desc;
    unsigned int id;
};

#endif
