#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "Core/Graphics.hpp"
#include "Scene/FPSCamera.hpp"
#include "Scene/Light.hpp"
#include "Scene/Model.hpp"
#include "UIData.hpp"

class TextureCache;

class Scene : public IMeshSet
{
public:
    Scene();
    ~Scene();

    virtual bool Load( const UI::Project& uiProject, TextureCache& textureCache );
    void CreateRenderingResources( IGraphics* graphics );

    // IMeshSet implementation
    [[nodiscard]] const std::vector< Material* >& GetMaterials() const override { return m_Materials; }
    [[nodiscard]] const std::vector< MeshInfo* >& GetMeshes() const override { return m_Meshes; }
    [[nodiscard]] const std::map< MaterialShaderType, std::vector< MeshInstance* > >& GetMeshInstances() const override { return m_MeshInstancesByShaderType; }

protected:
    virtual Model* CreateModel();

private:
    bool LoadProps( const std::vector< UI::Prop >& uiProps, TextureCache& textureCache );
    void LoadProp( const UI::Prop& uiProp, TextureCache& textureCache, MaterialShaderType shaderType );
    bool LoadLights( const std::vector< UI::Light >& uiLights, TextureCache& textureCache );
    bool LoadCameras( const std::vector< UI::Camera >& uiCameras );

    std::vector< Material* > m_Materials;
    std::vector< MeshInfo* > m_Meshes;
    std::map< MaterialShaderType, std::vector< MeshInstance* > > m_MeshInstancesByShaderType;

    std::vector< std::unique_ptr< Model > > m_Models;
    std::vector< std::shared_ptr< Light > > m_Lights;
    std::vector< std::shared_ptr< CameraPreset > > m_Cameras;

public:
    std::shared_ptr< CameraPreset > DefaultCamera;
    std::shared_ptr< CameraPreset > GetCameraByName( const std::string& name );
};

#endif
