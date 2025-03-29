#ifndef IRESOURCE_HPP
#define IRESOURCE_HPP

#include "Core/GLObject.hpp"
#include "Core/MemoryManager/RefCountPtr.hpp"

struct IResource
{
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;

    virtual GLObject GetGLObject( GLObjectType objectType )
    {
        (void)objectType;
        return nullptr;
    }
    virtual ~IResource() = default;
};

typedef RefCountPtr< IResource > ResourceHandle;

#endif
