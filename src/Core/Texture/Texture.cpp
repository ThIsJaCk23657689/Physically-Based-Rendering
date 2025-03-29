#include "Core/Texture/Texture.hpp"

GLObject Texture::GetGLObject( GLObjectType objectType )
{
    switch ( objectType )
    {
        case GLObjectTypes::GL_Texture:
            return GLObject( id );
        default:
            return nullptr;
    }
}
