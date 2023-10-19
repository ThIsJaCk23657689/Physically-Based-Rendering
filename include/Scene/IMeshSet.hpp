#ifndef IMESHSET_HPP
#define IMESHSET_HPP

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct LoadedTexture;

struct VertexAttribute {
    enum Enum : uint8_t {
        NONE        = 0x00,
        POSITION    = 0x01,
        NORMAL      = 0x02,
        TEXCOORD    = 0x04,
        ALL         = 0xFF
    };
};

struct BufferGroup {
    VertexArrayHandle vertexArray;
    BufferHandle indexBuffer;
    BufferHandle vertexBuffer;
};

struct Material {
    unsigned int id = 0;
    std::string name;
    std::shared_ptr<LoadedTexture> diffuseTexture;
    std::shared_ptr<LoadedTexture> specularTexture;
    std::shared_ptr<LoadedTexture> normalsTexture;
    std::shared_ptr<LoadedTexture> emissiveTexture;
    glm::vec3 diffuseColor = glm::vec3(0.0f);
    glm::vec3 specularColor = glm::vec3(0.0f);
    glm::vec3 emissiveColor = glm::vec3(0.0f);
    float shininess = 0.0f;
    float opacity = 1.0f;
};

struct MeshInfo {
    Material* material;
    BufferGroup* buffers;
    uint32_t indexOffset;
    uint32_t vertexOffset;
    uint32_t numIndices;
    uint32_t numVertices;
};

struct MeshInstance {
    MeshInfo* mesh;

    // Transform 概念
};

class IMeshSet {
public:
    virtual const std::vector<MeshInfo*>& GetMeshes() const = 0;
    virtual const std::vector<MeshInstance*>& GetMeshInstances() const = 0;
    virtual const std::vector<Material*>& GetMaterials() const = 0;
};

#endif
