#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Core/Graphics.hpp"
#include "Scene/IMeshSet.hpp"

enum ModelBasicType : unsigned int {
    Cube = 0,
    Sphere,
};

class Model : public IMeshSet {
public:
    Model();
    ~Model();

    // create model
    void Create(const ModelBasicType& createType);
    // void Create(const std::string& filePath);

    void AddInstance(const glm::mat4& transform);
    void CreateRenderingResources(IGraphics* graphics);

    // IMeshSet implementation
    const std::vector<MeshInfo*>& GetMeshes() const override { return m_Meshes; }
    const std::vector<MeshInstance*>& GetMeshInstances() const override { return m_MeshInstances; }
    const std::vector<Material*>& GetMaterials() const override { return m_Materials; }

protected:
    void GenerateVertices();
    // void LoadVerticesFromData();

    virtual MeshInfo* CreateMeshInfo();
    virtual MeshInstance* CreateMeshInstance();
    virtual Material* CreateMaterial();

    virtual void CreateVertexBuffer(IGraphics* graphics);
    virtual void CreateElementBuffer(IGraphics* graphics);
    virtual void CreateVertexArray(IGraphics* graphics);

    std::vector<MeshInfo*>      m_Meshes;
    std::vector<MeshInstance*>  m_MeshInstances;
    std::vector<Material*>      m_Materials;
    BufferGroup                 m_Buffers;

    uint8_t m_AttributeMask;
    std::vector<uint32_t>       m_Indices;
    std::vector<float>          m_Vertices;

private:

};

#endif
