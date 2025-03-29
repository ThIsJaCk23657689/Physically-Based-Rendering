#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL.h>

#include <list>
#include <vector>

#include "App/AppConfig.hpp"
#include "Core/MemoryManager/RefCountPtr.hpp"

class IGraphics;
using GraphicsHandle = RefCountPtr< IGraphics >;
class IRenderPass;

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;

    bool CreateContextAndWindow( const AppConfig& config );

    void AddRenderPassToFront( IRenderPass* pRenderPass );
    void AddRenderPassToBack( IRenderPass* pRenderPass );
    void RemoveRenderPass( IRenderPass* pRenderPass );

    void RunMainLoop();

    [[nodiscard]] SDL_Window* GetWindow() const { return m_Window; }
    void GetWindowSize( int& width, int& height ) const;

    SDL_GLContext GetContext() { return m_GLContext; }
    IGraphics* GetGraphics() { return m_Graphics; }

    const AppConfig& GetAppConfig();
    double GetAverageFrameTimeSeconds() const { return m_AverageFrameTime; }
    void SetFrameTimeUpdateInterval( double seconds ) { m_AverageTimeUpdateInterval = seconds; }
    bool IsVsyncEnabled() const { return m_Config.vsyncEnabled; }
    void SetVsyncEnabled( bool enabled ) { m_Config.vsyncEnabled = enabled; }

    void Shutdown();

protected:
    bool m_WindowVisible = false;

    AppConfig m_Config;
    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_GLContext{};
    GraphicsHandle m_Graphics;
    std::vector< SDL_Event > m_Events = {};
    std::list< IRenderPass* > m_RenderPasses;

    bool m_ShouldClose = false;
    double m_PreviousFrameTime = 0.0;
    double m_AverageFrameTime = 0.0;
    double m_AverageTimeUpdateInterval = 0.5;
    double m_FrameTimeSum = 0.0;
    int m_NumberOfAccumulatedFrames = 0;

    void HandleEvents();
    void PollEvents();
    void ProcessEvents();

    // Event functions
    void OnWindowCloseEvent();
    void OnWindowEvent( const SDL_WindowEvent& event );
    void OnKeyboardEvent( const SDL_KeyboardEvent& event );
    void OnTextInputEvent( const SDL_TextInputEvent& event );
    void OnMouseButtonEvent( const SDL_MouseButtonEvent& event );
    void OnMouseMotionEvent( const SDL_MouseMotionEvent& event );
    void OnMouseWheelEvent( const SDL_MouseWheelEvent& event );

    void UpdateWindowSize();
    void BackBufferResizing();
    void BackBufferResized();

    void Animate( double deltaTime );
    void Render();
    void UpdateAverageFrameTime( double deltaTime );

    void BeginFrame();
    void Present();
};

#endif
