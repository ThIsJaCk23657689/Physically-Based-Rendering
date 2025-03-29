#include "Scene/Scene.hpp"

Scene::Scene() {}

Scene::~Scene() {}

bool Scene::Load( const UI::Project& uiProject )
{
    bool success = false;

    success = LoadModels( uiProject.m_kObjects );
    success = success && LoadLights( uiProject.m_kLights );
    success = success && LoadCameras( uiProject.m_kCameras );

    for ( auto& model : m_Models )
    {
        m_Materials.insert( m_Materials.end(), model->GetMaterials().begin(), model->GetMaterials().end() );
        m_Meshes.insert( m_Meshes.end(), model->GetMeshes().begin(), model->GetMeshes().end() );
        m_MeshInstances.insert(
            m_MeshInstances.end(), model->GetMeshInstances().begin(), model->GetMeshInstances().end() );
    }

    return success;
}

void Scene::CreateRenderingResources( IGraphics* graphics )
{
    for ( auto& model : m_Models )
    {
        model->CreateRenderingResources( graphics );
    }
}

Model* Scene::CreateModel()
{
    return new Model();
}

bool Scene::LoadModels( const std::vector< UI::BaseNode >& uiObjects )
{
    Model* model = CreateModel();
    m_Models.push_back( std::unique_ptr< Model >( model ) );

    glm::mat4 transform = glm::mat4( 1.0f );
    model->AddInstance( transform );
    model->Create( ModelBasicType::Cube );

    return true;
}

bool Scene::LoadLights( const std::vector< UI::Light >& uiLights )
{
    return true;
}

bool Scene::LoadCameras( const std::vector< UI::Camera >& uiCameras )
{
    for ( const auto& uiCamera : uiCameras )
    {
        std::shared_ptr< CameraPreset > camera = std::make_shared< CameraPreset >();
        camera->name = uiCamera.name;
        camera->position = uiCamera.position;
        camera->target = uiCamera.target;
        camera->up = uiCamera.up;
        camera->fovY = uiCamera.fovY;
        m_Cameras.push_back( camera );
    }

    if ( !m_Cameras.empty() )
    {
        DefaultCamera = GetCameraByName( m_Cameras[ 0 ]->name );
    }
    else
    {
        std::shared_ptr< CameraPreset > camera = std::make_shared< CameraPreset >();
        camera->name = "Auto Generate Camera";
        camera->position = glm::vec3( 0.0f );
        camera->target = glm::vec3( 0.0f, 0.0f, -1.0f );
        camera->up = glm::vec3( 0.0f, 1.0f, 0.0f );
        camera->fovY = 45.0f;
        m_Cameras.push_back( camera );

        DefaultCamera = m_Cameras[ 0 ];
    }
    return true;
}

std::shared_ptr< CameraPreset > Scene::GetCameraByName( const std::string& name )
{
    for ( auto camera : m_Cameras )
    {
        if ( camera->name == name )
        {
            return camera;
        }
    }
    return nullptr;
}
