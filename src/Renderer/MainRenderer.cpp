#include "Renderer/MainRenderer.hpp"

#include "Core/Graphics.hpp"
#include "Core/Shader.hpp"
#include "Core/Texture/TextureCache.hpp"
#include "Scene/Scene.hpp"
#include "UIData.hpp"
#include "glm/glm.hpp"

MainRenderer::MainRenderer( Application* app, UIData& ui ) : Super( app ), m_UI( ui )
{
    m_Graphics = GetApplication()->GetGraphics();
    m_Graphics->EnableDepthTest( true );

    // Texture: TextureCache
    m_TextureCache = std::make_shared< TextureCache >( m_Graphics );

    // Shaders: ShaderFactory
    m_LightingShader = std::make_shared< Shader >( "assets/shaders/basic.vert", "assets/shaders/lighting.frag" );
    m_LightCubeShader = std::make_shared< Shader >( "assets/shaders/basic.vert", "assets/shaders/lightcube.frag" );

    // CommonPasses

    // ShadowMap (CascadedShadowMap) FrameBufferFactory

    m_Camera = std::make_unique< FPSCamera >();
    m_Camera->SetMoveSpeed( 1.0f );

    // Loaded Scene
    SetAsynchronousLoadingEnabled( false );
    SetCurrentScene( "The World of Rick Roll" );

    // Load EnvironmentMap
    // m_Texture = m_TextureCache->LoadTextureFromFile( "assets/textures/rickroll.jpg", false );
    // m_TextureCache->LoadTextureFromFileDeferred("assets/textures/rickroll.jpg", false);
}

void MainRenderer::SetCurrentScene( const std::string& sceneName )
{
    if ( m_CurrentSceneName == sceneName )
    {
        return;
    }

    m_CurrentSceneName = sceneName;
    BeginLoadingScene();
}

bool MainRenderer::SetupView()
{
    return true;
}

void MainRenderer::CreateRenderPasses() {}

void MainRenderer::RenderScene()
{
    int windowWidth, windowHeight;
    GetApplication()->GetWindowSize( windowWidth, windowHeight );
    float aspect = static_cast< float >( windowWidth ) / static_cast< float >( windowHeight );

    m_Graphics->SetViewport( 0, 0, windowWidth, windowHeight );
    m_Graphics->ClearCache( m_UI.clearColor );

    auto view = m_Camera->GetWorldToViewMatrix();
    auto projection = glm::perspective( glm::radians( m_Camera->m_VerticalFov ), aspect, 0.1f, 1000.0f );

    for ( const auto& kPair : m_Scene->GetMeshInstances() )
    {
        const auto& shaderType = kPair.first;
        if ( shaderType == MaterialShaderType::LightCube )
        {
            m_LightCubeShader->Start();
            m_LightCubeShader->SetMat4( "View", view );
            m_LightCubeShader->SetMat4( "Projection", projection );

            // TODO: 把 LightColor 移除掉，這裡應該直接填 Light Prop Color
            m_LightCubeShader->SetVec3( "LightColor", { 1.0f, 1.0f, 1.0f } );

            for ( const auto& instance : kPair.second )
            {
                auto model = instance->localTransform;
                m_LightCubeShader->SetMat4( "Model", model );

                const auto& vao = instance->mesh->buffers->vertexArray;
                const auto& indexCount = instance->mesh->numIndices;
                if ( instance->mesh->material->useTexture )
                {
                    m_Graphics->DrawIndexed( vao, instance->mesh->material->diffuseTexture->texture, indexCount );
                }
                else
                {
                    m_Graphics->DrawIndexed( vao, nullptr, indexCount );
                }
            }
        }
        else if ( shaderType == MaterialShaderType::Lighting )
        {
            m_LightingShader->Start();
            m_LightingShader->SetMat4( "View", view );
            m_LightingShader->SetMat4( "Projection", projection );

            m_LightingShader->SetVec3( "LightColor", { 1.0f, 1.0f, 1.0f } );

            for ( const auto& instance : kPair.second )
            {
                auto model = instance->localTransform;
                m_LightingShader->SetMat4( "Model", model );

                const auto& vao = instance->mesh->buffers->vertexArray;
                const auto& indexCount = instance->mesh->numIndices;
                if ( instance->mesh->material->useTexture )
                {
                    m_LightingShader->SetInt( "MySampler", 0 );
                    m_Graphics->DrawIndexed( vao, instance->mesh->material->diffuseTexture->texture, indexCount );
                }
                else
                {
                    // TODO: 應該要支援純色 Shader
                    m_Graphics->DrawIndexed( vao, nullptr, indexCount );
                }
            }
        }
    }
}

void MainRenderer::RenderSplashScreen()
{
    GetApplication()->SetVsyncEnabled( true );
}

bool MainRenderer::LoadScene()
{
    auto scene = std::make_unique< Scene >();

    // load ui scene data to create real scene object
    if ( scene->Load( m_UI.project, *m_TextureCache ) )
    {
        m_Scene = std::move( scene );
        return true;
    }

    return false;
}

void MainRenderer::SceneLoaded()
{
    Super::SceneLoaded();
    m_Scene->CreateRenderingResources( m_Graphics );

    // Draw Strategy

    // Process with lights

    // Process with Camera
    m_Camera->LookAt( m_Scene->DefaultCamera->position, m_Scene->DefaultCamera->target );
    m_Camera->SetVerticalFov( m_Scene->DefaultCamera->fovY );
}

void MainRenderer::SceneUnloading() {}

void MainRenderer::Animate( const float& deltaTime )
{
    m_Camera->Animate( GetApplication()->GetWindow(), deltaTime );
}

bool MainRenderer::OnKeyboardEvent( const SDL_KeyboardEvent& event )
{
    const auto scancode = event.scancode;
    if ( keyboardMap.find( scancode ) != keyboardMap.end() )
    {
        auto applicationKey = keyboardMap.at( scancode );
        if ( event.down || event.repeat )
        {
            m_Camera->SwitchCameraCursorMode();
        }
    }

    m_Camera->OnKeyboardEvent( event );
    return true;
}

bool MainRenderer::OnMouseButtonEvent( const SDL_MouseButtonEvent& event )
{
    m_Camera->OnMouseButtonEvent( event );
    return true;
}

bool MainRenderer::OnMouseMotionEvent( const SDL_MouseMotionEvent& event )
{
    m_Camera->OnMouseMotionEvent( event );
    return true;
}

bool MainRenderer::OnMouseWheelEvent( const SDL_MouseWheelEvent& event )
{
    m_Camera->OnMouseWheelEvent( event );
    return true;
}
