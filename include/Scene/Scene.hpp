#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include "UIData.hpp"
#include "Core/Graphics.hpp"
#include "Scene/Model.hpp"
#include "Scene/FPSCamera.hpp"

class Scene : public IMeshSet {
public:
    Scene();
    ~Scene();

    virtual bool Load(const UI::Project& uiProject);
    void CreateRenderingResources(IGraphics* graphics);

    // IMeshSet implementation
    const std::vector<MeshInfo*>& GetMeshes() const override { return m_Meshes; }
    const std::vector<MeshInstance*>& GetMeshInstances() const override { return m_MeshInstances; }
    const std::vector<Material*>& GetMaterials() const override { return m_Materials; }

protected:
    virtual Model* CreateModel();

private:
    bool LoadModels(const std::vector<UI::BaseNode>& uiObjects);
    bool LoadLights(const std::vector<UI::Light>& uiLights);
    bool LoadCameras(const std::vector<UI::Camera>& uiCameras);

    std::vector<MeshInfo*>      m_Meshes;
    std::vector<MeshInstance*>  m_MeshInstances;
    std::vector<Material*>      m_Materials;

    std::vector<std::unique_ptr<Model>> m_Models;
    std::vector<std::shared_ptr<CameraPreset>> m_Cameras;

public:
    std::shared_ptr<CameraPreset> DefaultCamera;
    std::shared_ptr<CameraPreset> GetCameraByName(const std::string& name);

};

#endif
