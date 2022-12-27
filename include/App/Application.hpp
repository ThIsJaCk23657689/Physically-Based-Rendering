#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <thread>
#include "App/IRenderPass.hpp"
#include "App/ContextManager.hpp"

class Application : public IRenderPass {
public:
    Application(ContextManager* contextManager);

    virtual void Render() override;

    virtual void RenderScene();
    virtual void RenderSplashScreen();
    virtual void BeginLoadingScene();
    virtual bool LoadScene();
    virtual void SceneUnloading();
    virtual void SceneLoaded();

    void SetAsynchronousLoadingEnabled(bool enabled);
    bool IsSceneLoading() const;
    bool IsSceneLoaded() const;

    // CommonPass

protected:
    typedef IRenderPass Super;

    std::unique_ptr<std::thread> m_SceneLoadingThread;
    bool m_IsAsyncLoad;

    // CommonPass

private:
    bool m_SceneLoaded;

};

#endif