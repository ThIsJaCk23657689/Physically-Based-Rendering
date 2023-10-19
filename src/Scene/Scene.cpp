#include "Scene/Scene.hpp"

Scene::Scene() {

}

Scene::~Scene() {

}

bool Scene::Load() {
    bool success = false;

    success = LoadModels();
    success = success && LoadLights();
    success = success && LoadCameras();

    for (auto& model : m_models) {
        m_Materials.insert(m_Materials.end(), model->GetMaterials().begin(), model->GetMaterials().end());
        m_Meshes.insert(m_Meshes.end(), model->GetMeshes().begin(), model->GetMeshes().end());
        m_MeshInstances.insert(m_MeshInstances.end(), model->GetMeshInstances().begin(), model->GetMeshInstances().end());
    }

    return success;
}

void Scene::CreateRenderingResources(IGraphics* graphics) {
    for (auto& model : m_models) {
        model->CreateRenderingResources(graphics);
    }
}

Model* Scene::CreateModel() {
    return new Model();
}

bool Scene::LoadModels() {
    Model* model = CreateModel();
    m_models.push_back(std::unique_ptr<Model>(model));

    glm::mat4 transform = glm::mat4(1.0f);
    model->AddInstance(transform);
    model->Create(ModelBasicType::Cube);

    return true;
}

bool Scene::LoadLights() {
    return true;
}

bool Scene::LoadCameras() {
    return true;
}
