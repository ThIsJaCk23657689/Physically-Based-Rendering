#include "Scene/Scene.hpp"
#include "Core/Texture/TextureCache.hpp"
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {}

Scene::~Scene() {}

bool Scene::Load( const UI::Project& uiProject, TextureCache& textureCache )
{
    bool success = false;

    success = LoadProps( uiProject.GetProps(), textureCache );
    success = success && LoadLights( uiProject.GetLights(), textureCache );
    success = success && LoadCameras( uiProject.GetCameras() );

    for ( auto& model : m_Models )
    {
        m_Materials.insert( m_Materials.end(), model->GetMaterials().begin(), model->GetMaterials().end() );
        m_Meshes.insert( m_Meshes.end(), model->GetMeshes().begin(), model->GetMeshes().end() );

        for ( auto& kPair : model->GetMeshInstances() )
        {
            auto& shaderType = kPair.first;
            auto& instances = kPair.second;
            m_MeshInstancesByShaderType[ shaderType ].insert( m_MeshInstancesByShaderType[ shaderType ].end(), instances.begin(), instances.end() );
        }
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

bool Scene::LoadProps( const std::vector< UI::Prop >& uiProps, TextureCache& textureCache )
{
    for ( const auto& uiProp : uiProps )
    {
        LoadProp( uiProp, textureCache, MaterialShaderType::Lighting );
    }
    return true;
}

void Scene::LoadProp( const UI::Prop& uiProp, TextureCache& textureCache, MaterialShaderType shaderType )
{
    Model* model = CreateModel();
    m_Models.push_back( std::unique_ptr< Model >( model ) );

    auto transform = glm::mat4( 1.0f );
    transform = glm::translate( transform, uiProp.position );
    transform = glm::rotate( transform, uiProp.rotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
    transform = glm::rotate( transform, uiProp.rotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    transform = glm::rotate( transform, uiProp.rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    transform = glm::scale( transform, uiProp.scale );

    model->AddInstance( transform );
    model->SetShaderType( shaderType );

    // Load
    model->Load( uiProp, textureCache );
}

bool Scene::LoadLights( const std::vector< UI::Light >& uiLights, TextureCache& textureCache )
{
    for ( const auto& uiLight : uiLights )
    {
        std::shared_ptr< Light > light = std::make_shared< Light >();
        light->name = uiLight.name;
        light->color = uiLight.color;
        m_Lights.push_back( light );

        if ( uiLight.gizmo )
        {
            uiLight.gizmo->position = uiLight.position;
            uiLight.gizmo->material->color = uiLight.color;
            LoadProp( *uiLight.gizmo, textureCache, MaterialShaderType::LightCube );
        }
    }
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
