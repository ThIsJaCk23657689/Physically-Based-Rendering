#include "App/Application.hpp"

Application::Application(ContextManager* contextManager)
    : Super(contextManager), m_IsAsyncLoad(true), m_SceneLoaded(false) {

}

void Application::Render() {
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

void Application::RenderScene() {

}

void Application::RenderSplashScreen() {

}

bool Application::LoadScene() {
    return true;
}

void Application::SceneUnloading() {

}

void Application::SceneLoaded() {
    m_SceneLoaded = true;
}

void Application::SetAsynchronousLoadingEnabled(bool enabled) {
    m_IsAsyncLoad = enabled;
}

bool Application::IsSceneLoading() const {
    return m_SceneLoadingThread != nullptr;
}

bool Application::IsSceneLoaded() const {
    return m_SceneLoaded;
}

void Application::BeginLoadingScene() {
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

