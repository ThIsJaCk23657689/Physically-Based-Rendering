#ifndef CONTEXTMANAGER_HPP
#define CONTEXTMANAGER_HPP

#include <SDL.h>
#include <list>
#include "App/AppConfig.hpp"
#include "App/IRenderPass.hpp"

class IRenderPass;

class ContextManager {
public:
    ContextManager() {}

    bool CreateWindow(const AppConfig& config);

    void AddRenderPassToFront(IRenderPass* pRenderPass);
    void AddRenderPassToBack(IRenderPass* pRenderPass);
    void RemoveRenderPass(IRenderPass* pRenderPass);

    void RunMainLoop();

    SDL_Window* GetWindow() const { return m_Window; }
    void GetWindowSize(int& width, int& height);

    SDL_GLContext GetContext() { return m_GLContext; }

    const AppConfig& GetAppConfig();
    double GetAverageFrameTimeSeconds() const { return m_AverageFrameTime; }
    void SetFrameTimeUpdateInterval(double seconds) { m_AverageTimeUpdateInterval = seconds; }
    bool IsVsyncEnabled() const { return m_Config.vsyncEnabled; }
    void SetVsyncEnabled(bool enabled) { m_Config.vsyncEnabled = enabled; }

    // Event functions

    void Shutdown();
    virtual ~ContextManager() {}

protected:
    bool m_WindowVisible = false;

    AppConfig m_Config;
    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_GLContext;
    std::list<IRenderPass*> m_RenderPasses;

    bool m_ShouldClose = false;
    double m_PreviousFrameTime = 0.0;
    double m_AverageFrameTime = 0.0;
    double m_AverageTimeUpdateInterval = 0.5;
    double m_FrameTimeSum = 0.0;
    int m_NumberOfAccumulatedFrames = 0;

    void UpdateWindowSize();

    void BackBufferResizing();
    void BackBufferResized();

    void Animate(double deltaTime);
    void Render();
    void UpdateAverageFrameTime(double deltaTime);

    void BeginFrame();
    void Present();
};

#endif