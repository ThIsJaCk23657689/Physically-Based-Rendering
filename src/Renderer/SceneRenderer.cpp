#include "Renderer/SceneRenderer.hpp"

SceneRenderer::SceneRenderer(Application* app)
    : Super(app), m_IsAsyncLoad(true), m_SceneLoaded(false) {

}

void SceneRenderer::Render() {
    if (!m_SceneLoaded)
    {
        RenderSplashScreen();
        return;
    }

    if (m_SceneLoaded && m_SceneLoadingThread)
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
    return true;
}

void SceneRenderer::SceneUnloading() {

}

void SceneRenderer::SceneLoaded() {
    m_SceneLoaded = true;
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

    m_SceneLoaded = false;

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

