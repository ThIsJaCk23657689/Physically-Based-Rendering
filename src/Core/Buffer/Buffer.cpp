#include "Core/Buffer/Buffer.hpp"

GLObject Buffer::GetGLObject(GLObjectType objectType) {
    switch (objectType) {
        case GLObjectTypes::GL_Buffer:
            return GLObject(id);
        default:
            return nullptr;
    }
}