#ifndef MAINRENDERER_HPP
#define MAINRENDERER_HPP

#include <string>
#include <vector>

#include "Renderer/SceneRenderer.hpp"
#include "Scene/FPSCamera.hpp"

class Graphics;
class Shader;
class Scene;
struct LoadedTexture;
struct UIData;

class MainRenderer : public SceneRenderer
{
public:
    MainRenderer( Application* app, UIData& ui );

    std::string GetCurrentSceneName() const { return m_CurrentSceneName; }
    void SetCurrentScene( const std::string& sceneName );

    std::shared_ptr< TextureCache > GetTextureCache() { return m_TextureCache; }
    std::shared_ptr< Scene > GetScene() { return m_Scene; }
    FPSCamera* GetMainCamera() { return m_Camera.get(); }

    bool SetupView();
    void CreateRenderPasses();

    // override SceneRenderer
    void RenderScene() override;
    void RenderSplashScreen() override;
    bool LoadScene() override;
    void SceneLoaded() override;
    void SceneUnloading() override;

    // override IRenderPass
    void Animate( const float& deltaTime ) override;
    bool OnKeyboardEvent( const SDL_KeyboardEvent& event ) override;
    bool OnMouseButtonEvent( const SDL_MouseButtonEvent& event ) override;
    bool OnMouseMotionEvent( const SDL_MouseMotionEvent& event ) override;
    bool OnMouseWheelEvent( const SDL_MouseWheelEvent& event ) override;

private:
    typedef SceneRenderer Super;

    GraphicsHandle m_Graphics;
    std::string m_CurrentSceneName;
    std::shared_ptr< Scene > m_Scene;

    // Temp (Shader Factory)
    std::shared_ptr< Shader > m_LightingShader = nullptr;
    std::shared_ptr< Shader > m_LightCubeShader = nullptr;

    // IView
    std::unique_ptr< FPSCamera > m_Camera;
    std::shared_ptr< LoadedTexture > m_Texture;

    UIData& m_UI;

    typedef enum
    {
        SwitchCursorShowHide,
        KeyboardControlCount
    } KeyboardControls;

    const std::map< int, int > keyboardMap = { { SDL_SCANCODE_TAB, KeyboardControls::SwitchCursorShowHide } };
};

#endif
