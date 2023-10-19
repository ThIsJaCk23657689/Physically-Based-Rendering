#include "Renderer/SceneRenderer.hpp"

SceneRenderer::SceneRenderer(Application* app)
    : Super(app), m_IsAsyncLoad(false), m_SceneLoaded(false) {

}

void SceneRenderer::Render() {
    if (m_TextureCache)
    {
        m_TextureCache->ProcessRenderingThreadCommands(20.0f);
    }

    if (!m_SceneLoaded)
    {
        RenderSplashScreen();
        return;
    }

    // If multi-thread...
    if (m_SceneLoadingThread)
    {
        m_SceneLoadingThread->join();
        m_SceneLoadingThread = nullptr;

        SceneLoaded();
    }

    RenderScene();
}

void SceneRenderer::RenderScene() {

}

void SceneRenderer::RenderSplashScreen() {

}

bool SceneRenderer::LoadScene() {
    Scene* scene = new Scene();

    if (scene->Load()) {
        m_Scene = std::unique_ptr<Scene>(scene);
        return true;
    }

    return false;
}

void SceneRenderer::SceneUnloading() {

}

void SceneRenderer::SceneLoaded() {
    if (m_TextureCache)
    {
        m_TextureCache->ProcessRenderingThreadCommands(0.0f);
        // m_TextureCache->LoadingFinished();
    }
    m_SceneLoaded = true;
}

void SceneRenderer::SetCurrentScene(const std::string& sceneName) {
    if ( m_CurrentSceneName == sceneName ) {
        return;
    }

    m_CurrentSceneName = sceneName;
    BeginLoadingScene();
}

void SceneRenderer::SetAsynchronousLoadingEnabled(bool enabled) {
    m_IsAsyncLoad = enabled;
}

bool SceneRenderer::IsSceneLoading() const {
    return m_SceneLoadingThread != nullptr;
}

bool SceneRenderer::IsSceneLoaded() const {
    return m_SceneLoaded;
}

void SceneRenderer::BeginLoadingScene() {
    if (m_SceneLoaded)
    {
        SceneUnloading();
    }

    // reset all resource
    m_SceneLoaded = false;
    if (m_TextureCache)
    {
        m_TextureCache->Reset();
    }

    // determine multi-thread or not
    if (m_IsAsyncLoad)
    {
        m_SceneLoadingThread = std::make_unique<std::thread>([this](){ m_SceneLoaded = LoadScene(); });
    }
    else
    {
        m_SceneLoaded = LoadScene();
        SceneLoaded();
    }
}

