#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL.h>

#include <list>

#include "App/AppConfig.hpp"
#include "Renderer/IRenderPass.hpp"

class IRenderPass;

class Application {
public:
    Application() = default;

    bool CreateContextAndWindow(const AppConfig& config);

    void AddRenderPassToFront(IRenderPass* pRenderPass);
    void AddRenderPassToBack(IRenderPass* pRenderPass);
    void RemoveRenderPass(IRenderPass* pRenderPass);

    void RunMainLoop();

    SDL_Window* GetWindow() const { return m_Window; }
    void GetWindowSize(int& width, int& height) const;

    SDL_GLContext GetContext() { return m_GLContext; }

    const AppConfig& GetAppConfig();
    double GetAverageFrameTimeSeconds() const { return m_AverageFrameTime; }
    void SetFrameTimeUpdateInterval(double seconds) { m_AverageTimeUpdateInterval = seconds; }
    bool IsVsyncEnabled() const { return m_Config.vsyncEnabled; }
    void SetVsyncEnabled(bool enabled) { m_Config.vsyncEnabled = enabled; }

    // Event functions

    void Shutdown();
    virtual ~Application() = default;

protected:
    bool m_WindowVisible = false;

    AppConfig m_Config;
    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_GLContext{};
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