#ifndef IVERTEXARRAY_H
#define IVERTEXARRAY_H

#include <map>
#include "Core/Buffer/IBuffer.hpp"
#include "Core/IResource.hpp"

struct VertexArrayDesc {
    std::map<unsigned int, BufferHandle> vertexBuffers;
    BufferHandle elementBuffer;
};

class IVertexArray : public IResource {
public:
    virtual const VertexArrayDesc& GetDesc() const = 0;
    virtual const unsigned int GetID() const = 0;
};
typedef RefCountPtr<IVertexArray> VertexArrayHandle;

#endif
