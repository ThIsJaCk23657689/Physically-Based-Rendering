#include "Scene/Model.hpp"

BufferHandle CreateVertexBufferPtr( IGraphics* graphics, const Vertex* data, size_t size, const std::string& debugName )
{
    if ( !data || !size )
    {
        return nullptr;
    }

    BufferDesc bufferDesc;
    bufferDesc.type = BufferType::VertexBufferObject;
    bufferDesc.byteSize = static_cast< unsigned int >( size ) * sizeof( Vertex );

    bufferDesc.structStride = sizeof( Vertex );
    bufferDesc.dataOffset = 0;

    bufferDesc.cpuAccess = CpuAccessMode::Write;
    bufferDesc.debugName = debugName.c_str();

    BufferHandle buffer = graphics->CreateBuffer( bufferDesc );
    graphics->WriteBuffer( buffer, data, bufferDesc.byteSize );

    return buffer;
}

BufferHandle CreateIndexBufferPtr( IGraphics* graphics,
                                   const uint32_t* data,
                                   size_t size,
                                   const std::string& debugName )
{
    if ( !data || !size )
    {
        return nullptr;
    }

    BufferDesc bufferDesc;
    bufferDesc.type = BufferType::ElementBufferObject;
    bufferDesc.byteSize = static_cast< unsigned int >( size ) * sizeof( uint32_t );
    bufferDesc.cpuAccess = CpuAccessMode::Write;
    bufferDesc.debugName = debugName.c_str();

    BufferHandle buffer = graphics->CreateBuffer( bufferDesc );
    graphics->WriteBuffer( buffer, data, bufferDesc.byteSize );

    return buffer;
}

VertexArrayHandle CreateVertexArrayPtr( IGraphics* graphics,
                                        const uint32_t& bindingIndex,
                                        BufferHandle& vbo,
                                        BufferHandle& ebo,
                                        const std::string& debugName )
{
    if ( !vbo || !ebo )
    {
        return nullptr;
    }

    VertexArrayDesc vertexArrayDesc;
    vertexArrayDesc.vertexBuffers[ bindingIndex ] = vbo;
    vertexArrayDesc.elementBuffer = ebo;

    VertexArrayHandle vertexArray = graphics->CreateVertexArray( vertexArrayDesc );
    graphics->BindVertexBuffer( vertexArray, bindingIndex, vbo );
    graphics->BindIndexBuffer( vertexArray, ebo );

    return vertexArray;
}

void CreateVertexAttributePtr( IGraphics* graphics,
                               VertexArrayHandle& vao,
                               uint32_t attributeIndex,
                               uint32_t bindingIndex,
                               int32_t attributeStride,
                               uint32_t relativeOffset = 0 )
{
    if ( !vao )
    {
        return;
    }

    graphics->BindAttributePtr( vao, attributeIndex, bindingIndex, attributeStride, relativeOffset );
}

Model::Model() : m_AttributeMask( VertexAttribute::ALL ) {}

Model::~Model()
{
    for ( auto instance : m_MeshInstances )
    {
        delete instance;
    }

    for ( auto mesh : m_Meshes )
    {
        delete mesh;
    }

    for ( auto material : m_Materials )
    {
        delete material;
    }
}

void Model::Create( const ModelBasicType& createType )
{
    GenerateVertices();
}

void Model::AddInstance( const glm::mat4& transform ) {}

void Model::CreateRenderingResources( IGraphics* graphics )
{
    assert( graphics );

    // Buffers
    CreateVertexBuffer( graphics );
    CreateElementBuffer( graphics );
    CreateVertexArray( graphics );

    // Uniform Buffers

    // Materials
}

void Model::GenerateVertices()
{
    m_Vertices = {
        { {
              -0.5f,
              -0.5f,
              0.0f,
          },
          { 0.0f, 0.0f, -1.0f },
          { 0.0f, 0.0f } },
        { {
              0.5f,
              -0.5f,
              0.0f,
          },
          { 0.0f, 0.0f, -1.0f },
          { 2.0f, 0.0f } },
        { {
              0.5f,
              0.5f,
              0.0f,
          },
          { 0.0f, 0.0f, -1.0f },
          { 2.0f, 2.0f } },
        { {
              -0.5f,
              0.5f,
              0.0f,
          },
          { 0.0f, 0.0f, -1.0f },
          { 0.0f, 2.0f } },
    };

    m_Indices = {
        0,
        1,
        2,
        0,
        2,
        3,
    };

    m_AttributeMask = VertexAttribute::ALL;

    MeshInfo* mesh = CreateMeshInfo();
    {
        mesh->buffers = &m_Buffers;
        mesh->numIndices = m_Indices.size();
        mesh->numVertices = m_Vertices.size();
        mesh->indexOffset = 0;
        mesh->vertexOffset = 0;
        m_Meshes.push_back( mesh );
    }

    MeshInstance* instance = CreateMeshInstance();
    instance->mesh = mesh;
    m_MeshInstances.push_back( instance );
}

MeshInfo* Model::CreateMeshInfo()
{
    return new MeshInfo();
}

MeshInstance* Model::CreateMeshInstance()
{
    return new MeshInstance();
}

Material* Model::CreateMaterial()
{
    return new Material();
}

void Model::CreateVertexBuffer( IGraphics* graphics )
{
    m_Buffers.vertexBuffer = CreateVertexBufferPtr( graphics, m_Vertices.data(), m_Vertices.size(), "VertexBuffer" );

    // Force deallocate the array
    std::vector< Vertex >().swap( m_Vertices );
}

void Model::CreateElementBuffer( IGraphics* graphics )
{
    m_Buffers.indexBuffer = CreateIndexBufferPtr( graphics, m_Indices.data(), m_Indices.size(), "IndexBuffer" );

    // Force deallocate the array
    std::vector< uint32_t >().swap( m_Indices );
}

void Model::CreateVertexArray( IGraphics* graphics )
{
    const uint32_t bindingIndex = 0;
    m_Buffers.vertexArray =
        CreateVertexArrayPtr( graphics, bindingIndex, m_Buffers.vertexBuffer, m_Buffers.indexBuffer, "VertexArray" );

    if ( m_AttributeMask & VertexAttribute::POSITION )
    {
        const uint32_t attributeStride = 3;
        uint32_t currentOffset = offsetof( Vertex, position );
        CreateVertexAttributePtr( graphics, m_Buffers.vertexArray, 0, 0, attributeStride, currentOffset );
    }

    if ( m_AttributeMask & VertexAttribute::NORMAL )
    {
        const uint32_t attributeStride = 3;
        uint32_t currentOffset = offsetof( Vertex, normal );
        CreateVertexAttributePtr( graphics, m_Buffers.vertexArray, 1, 0, attributeStride, currentOffset );
    }

    if ( m_AttributeMask & VertexAttribute::TEXCOORD )
    {
        const uint32_t attributeStride = 2;
        uint32_t currentOffset = offsetof( Vertex, texCoord );
        CreateVertexAttributePtr( graphics, m_Buffers.vertexArray, 2, 0, attributeStride, currentOffset );
    }
}
