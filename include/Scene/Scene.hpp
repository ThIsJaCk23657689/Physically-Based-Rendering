#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include "Core/Graphics.hpp"
#include "Scene/Model.hpp"

class Scene : public IMeshSet {
public:
    Scene();
    ~Scene();

    virtual bool Load();
    void CreateRenderingResources(IGraphics* graphics);

    // IMeshSet implementation
    const std::vector<MeshInfo*>& GetMeshes() const override { return m_Meshes; }
    const std::vector<MeshInstance*>& GetMeshInstances() const override { return m_MeshInstances; }
    const std::vector<Material*>& GetMaterials() const override { return m_Materials; }

protected:
    virtual Model* CreateModel();

private:
    bool LoadModels();
    bool LoadLights();
    bool LoadCameras();

    std::vector<MeshInfo*>      m_Meshes;
    std::vector<MeshInstance*>  m_MeshInstances;
    std::vector<Material*>      m_Materials;

    std::vector<std::unique_ptr<Model>> m_models;
};

#endif
