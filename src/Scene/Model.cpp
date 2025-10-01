#include "Scene/Model.hpp"
#include "Core/Texture/TextureCache.hpp"
#include "UIData.hpp"

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
    for ( const auto& kPair : m_MeshInstancesByShaderType )
    {
        for ( auto instance : kPair.second )
        {
            delete instance;
        }
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

void Model::Load( const UI::Prop& uiProp, TextureCache& textureCache )
{
    m_AttributeMask = VertexAttribute::ALL;

    // Load materials
    if ( uiProp.material )
    {
        LoadMaterials( *uiProp.material, textureCache );
    }

    // Load vertices and indices
    const auto& vertices = uiProp.mesh->vertices;
    const auto& normals = uiProp.mesh->normals;
    const auto& texCoords = uiProp.mesh->texCoords;
    const auto& indices = uiProp.mesh->indices;
    assert( vertices.size() == normals.size() &&
            vertices.size() == texCoords.size() );
    PrepareVerticesData( vertices, normals, texCoords );
    PrepareIndicesData( indices );

    // Materials have already been initialized from json, just copy them to the m_Materials array
    for ( const auto& it : m_MaterialsByName )
    {
        m_Materials.push_back( it.second );
    }

    // Create mesh and instance for now
    MeshInfo* mesh = CreateMeshInfo();
    {
        mesh->buffers = &m_Buffers;
        mesh->numIndices = m_Indices.size();
        mesh->numVertices = m_Vertices.size();
        mesh->indexOffset = 0;
        mesh->vertexOffset = 0;
        mesh->material = m_MaterialsByName[ uiProp.material->name ];
        m_Meshes.push_back( mesh );
    }

    for( const auto& transform : m_ObjectInstanceTransforms )
    {
        MeshInstance* instance = CreateMeshInstance();
        instance->mesh = mesh;
        instance->localTransform = transform;
        m_MeshInstancesByShaderType[ m_ShaderType ].push_back( instance );
    }
}

void Model::AddInstance( const glm::mat4& transform )
{
    m_ObjectInstanceTransforms.emplace_back(transform );
}

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

void Model::PrepareVerticesData( const std::vector< glm::vec3 >& vertices,
                                 const std::vector< glm::vec3 >& normals,
                                 const std::vector< glm::vec2 >& texCoords )
{
    for ( size_t i = 0; i < vertices.size(); ++i )
    {
        Vertex vertex = {};
        vertex.position = vertices[ i ];
        vertex.normal = normals[ i ];
        vertex.texCoord = texCoords[ i ];
        m_Vertices.push_back( vertex );
    }
}

void Model::PrepareIndicesData( const std::vector< uint32_t >& indices )
{
    m_Indices = indices;
}

void Model::LoadMaterials( const UI::Material& uiMaterial, TextureCache& textureCache )
{
    Material* material = CreateMaterial();
    const auto& strMaterialName = uiMaterial.name;
    m_MaterialsByName[ strMaterialName ] = material;

    material->name = strMaterialName;
    material->diffuseColor = uiMaterial.color;

    auto fnLoadTexture = [ this, &textureCache ]( const std::string& path, bool sRGB )
    {
        if ( path.empty() )
        {
            assert( 0 && "Texture path is empty" );
            return std::make_shared< LoadedTexture >();
        }
        return textureCache.LoadTextureFromFile( path, sRGB );
    };

    if ( uiMaterial.bUseTexture )
    {
        material->useTexture = true;
        material->diffuseTexture = fnLoadTexture( uiMaterial.texturePath, false );
    }
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
