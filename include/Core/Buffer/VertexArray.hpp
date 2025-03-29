#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

#include "Core/Buffer/IVertexArray.hpp"
#include "Core/Graphics.hpp"
#include "Core/MemoryManager/RefCounter.hpp"

class Graphics;

class VertexArray : public RefCounter< IVertexArray >
{
public:
    VertexArray( Graphics* _parent ) : parent( _parent ), id( 0 ) {}
    const VertexArrayDesc& GetDesc() const override { return desc; }
    const unsigned int GetID() const override { return id; }
    GLObject GetGLObject( GLObjectType objectType ) override;

    Graphics* parent;
    VertexArrayDesc desc;
    unsigned int id;
};
#endif
