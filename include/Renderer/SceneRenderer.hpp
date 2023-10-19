#ifndef SCENERENDERER_HPP
#define SCENERENDERER_HPP

#include <memory>
#include <string>
#include <thread>

#include "App/Application.hpp"
#include "Core/Texture/TextureCache.hpp"
#include "IRenderPass.hpp"
#include "Scene/Scene.hpp"

class SceneRenderer : public IRenderPass {
public:
    SceneRenderer(Application* app);

    virtual bool LoadScene() = 0;
    virtual void RenderScene();
    virtual void RenderSplashScreen();
    virtual void BeginLoadingScene();
    virtual void SceneLoaded();
    virtual void SceneUnloading();

    void SetAsynchronousLoadingEnabled(bool enabled);
    bool IsSceneLoading() const;
    bool IsSceneLoaded() const;

    // override IRenderPass
    virtual void Render() override;

    // CommonPass

protected:
    typedef IRenderPass Super;

    bool m_IsAsyncLoad;
    std::shared_ptr<TextureCache> m_TextureCache;
    std::unique_ptr<std::thread> m_SceneLoadingThread;
    // CommonPass

private:
    bool m_SceneLoaded;
};

#endif