#ifndef IBUFFER_HPP
#define IBUFFER_HPP

#include <cstddef>

#include "Core/IResource.hpp"
#include "Core/MemoryManager/RefCountPtr.hpp"

struct BufferType
{
    enum Enum
    {
        None = 0,
        VertexBufferObject = 1,
        ElementBufferObject = 2,
    };
};

struct CpuAccessMode
{
    enum Enum
    {
        None,
        Read,
        Write
    };
};

struct BufferDesc
{
    // Buffer Size (Bytes)
    uint32_t byteSize = 0;

    // Buffer Stride (Bytes) 這邊儲存的是指你整個 VBO 每個筆資料的 Stride
    int32_t structStride = 0;

    // Data Offset (Bytes) 這邊儲存的是指你整個 VBO 的 Data Offset，不是 Attribute 的
    uint32_t dataOffset = 0;

    BufferType::Enum type = BufferType::None;
    CpuAccessMode::Enum cpuAccess = CpuAccessMode::None;
    const char* debugName = nullptr;
};

class IBuffer : public IResource
{
public:
    virtual const BufferDesc& GetDesc() const = 0;
    virtual unsigned int GetID() const = 0;
};
typedef RefCountPtr< IBuffer > BufferHandle;

#endif
