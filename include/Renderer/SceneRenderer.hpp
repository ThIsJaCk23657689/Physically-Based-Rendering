#ifndef SCENERENDERER_HPP
#define SCENERENDERER_HPP

#include <memory>
#include <thread>
#include <string>

#include "App/Application.hpp"
#include "Core/Texture/TextureCache.hpp"
#include "Scene/Scene.hpp"
#include "IRenderPass.hpp"

class SceneRenderer : public IRenderPass {
public:
    SceneRenderer(Application* app);

    void Render() override;

    virtual void RenderScene();
    virtual void RenderSplashScreen();
    virtual void BeginLoadingScene();
    virtual bool LoadScene();
    virtual void SceneUnloading();
    virtual void SceneLoaded();

    void SetCurrentScene(const std::string& sceneName);
    void SetAsynchronousLoadingEnabled(bool enabled);
    bool IsSceneLoading() const;
    bool IsSceneLoaded() const;

    // CommonPass

protected:
    typedef IRenderPass Super;

    std::string m_CurrentSceneName;
    std::shared_ptr<TextureCache> m_TextureCache;
    std::unique_ptr<Scene> m_Scene;
    std::unique_ptr<std::thread> m_SceneLoadingThread;
    bool m_IsAsyncLoad;

    // CommonPass

private:
    bool m_SceneLoaded;

};

#endif