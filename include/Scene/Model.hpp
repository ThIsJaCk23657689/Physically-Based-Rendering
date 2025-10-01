#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

#include "Core/Graphics.hpp"
#include "Scene/IMeshSet.hpp"
#include "Scene/Vertex.hpp"

class TextureCache;

namespace UI
{
    struct Prop;
    struct Material;
}

class Model : public IMeshSet
{
public:
    Model();
    ~Model();

    // create model
    void Load( const UI::Prop& uiProp, TextureCache& textureCache );
    // void Create(const std::string& filePath);

    void AddInstance( const glm::mat4& transform );
    void CreateRenderingResources( IGraphics* graphics );

    // IMeshSet implementation
    [[nodiscard]] const std::vector< Material* >& GetMaterials() const override { return m_Materials; }
    [[nodiscard]] const std::vector< MeshInfo* >& GetMeshes() const override { return m_Meshes; }
    [[nodiscard]] const std::map< MaterialShaderType, std::vector< MeshInstance* > >& GetMeshInstances() const override { return m_MeshInstancesByShaderType; }

    [[nodiscard]] MaterialShaderType GetShaderType() const { return m_ShaderType; }
    void SetShaderType( MaterialShaderType type ) { m_ShaderType = type; }
protected:
    void PrepareVerticesData( const std::vector< glm::vec3 >& vertices,
                              const std::vector< glm::vec3 >& normals,
                              const std::vector< glm::vec2 >& texCoords );
    void PrepareIndicesData( const std::vector< uint32_t >& indices );
    void LoadMaterials( const UI::Material& uiMaterial, TextureCache& textureCache );

    virtual MeshInfo* CreateMeshInfo();
    virtual MeshInstance* CreateMeshInstance();
    virtual Material* CreateMaterial();

    virtual void CreateVertexBuffer( IGraphics* graphics );
    virtual void CreateElementBuffer( IGraphics* graphics );
    virtual void CreateVertexArray( IGraphics* graphics );

    std::vector< MeshInfo* > m_Meshes;
    std::map< MaterialShaderType, std::vector< MeshInstance* > > m_MeshInstancesByShaderType;
    std::vector< Material* > m_Materials;
    std::map< std::string, Material* > m_MaterialsByName;
    BufferGroup m_Buffers;

    uint8_t m_AttributeMask;
    std::vector< uint32_t > m_Indices;
    std::vector< Vertex > m_Vertices;

    MaterialShaderType m_ShaderType = MaterialShaderType::Lighting;
    std::vector< glm::mat4 > m_ObjectInstanceTransforms;

private:
};

#endif
