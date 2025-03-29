#include "Core/Buffer/VertexArray.hpp"

GLObject VertexArray::GetGLObject( GLObjectType objectType )
{
    switch ( objectType )
    {
        case GLObjectTypes::GL_VertexArray:
            return GLObject( id );
        default:
            return nullptr;
    }
}
