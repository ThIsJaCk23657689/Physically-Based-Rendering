#include "App/Application.hpp"

#include <glad/glad.h>

#include <cassert>
#include <mutex>

#include "Renderer/IRenderPass.hpp"
#include "Utility/Log.hpp"

static std::recursive_mutex g_contextManagerMutex;
static int g_SDLInitCalls = 0;

bool Application::CreateContextAndWindow( const AppConfig& config )
{
    Log::ShowMe();

    // Call SDL Init only once
    {
        std::lock_guard< std::recursive_mutex > lock( g_contextManagerMutex );
        if ( ++g_SDLInitCalls == 1 )
        {
            // Initialize SDL2
            if ( !SDL_Init( SDL_INIT_VIDEO ) )
            {
                Log::Error( "Oops! Failed to initialize SDL3. :(\nError: SDL_Init(): %s", SDL_GetError() );
                return false;
            }
            Log::Info( "Initialize SDL3 successfully." );
        }
    }

    this->m_Config = config;

    // Request an OpenGL context (and setting CORE mode)
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
#ifdef __APPLE__
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
#endif
#ifdef __linux__
    SDL_SetHint( SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0" );
#endif

    // Setting Back Buffers
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // Anti Aliasing ( Off )
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 0 );
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 0 );

    // Hardware Acceleration
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );

    // Create window with graphics context
    if ( config.fullScreenEnabled )
    {
        const auto windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIDDEN;
        m_Window = SDL_CreateWindow( config.title.c_str(),
                                     0,
                                     0,
                                     windowFlags );
    }
    else
    {
        const auto windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
        m_Window = SDL_CreateWindow( config.title.c_str(),
                                     ( int ) config.width,
                                     ( int ) config.height,
                                     windowFlags );
    }
    if ( !m_Window )
    {
        Log::Error( "Failed to create SDL3 window.\n Error SDL_CreateWindow(): %s", SDL_GetError() );
        return false;
    }
    Log::Info( "Create a SDL3 window successfully." );

    if ( config.fullScreenEnabled )
    {
        SDL_SetWindowPosition( m_Window, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED );
    }
    else
    {
        SDL_SetWindowPosition( m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
    }

    SDL_SetWindowMinimumSize( m_Window, 400, 300 );

    int bufferWidth, bufferHeight;
    SDL_GetWindowSize( m_Window, &bufferWidth, &bufferHeight );
    m_Config.width = bufferWidth;
    m_Config.height = bufferHeight;

    // Create GL Context
    m_GLContext = SDL_GL_CreateContext( m_Window );
    if ( !m_GLContext )
    {
        Log::Error( "Failed to create SDL3 window.\nError: SDL_GL_CreateContext(): %s", SDL_GetError() );
        return false;
    }
    Log::Info( "Create OpenGL context successfully." );
    SDL_GL_MakeCurrent( m_Window, m_GLContext );

    // Initialize glad
    if ( !gladLoadGLLoader( ( GLADloadproc ) SDL_GL_GetProcAddress ) )
    {
        Log::Error( "Oh No! Failed to initialize glad." );
        return false;
    }
    Log::Info( "Initialize glad successfully." );

    // Create Graphics
    m_Graphics = GraphicsHandle::Create( new Graphics() );

    // Output OpenGL Info
    Log::Info( "OpenGL Version:\t%s", glGetString( GL_VERSION ) );
    Log::Info( "GLSL Version:\t%s", glGetString( GL_SHADING_LANGUAGE_VERSION ) );
    Log::Info( "GPU Renderer:\t%s", glGetString( GL_RENDERER ) );
    Log::Info( "GPU Vendor:\t%s", glGetString( GL_VENDOR ) );

    // Enable vsync
    SDL_GL_SetSwapInterval( m_Config.vsyncEnabled );
    SDL_ShowWindow( m_Window );

    // Setting the mouse mode
    // SDL_SetHintWithPriority( SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE );

    // reset the back buffer size state to enforce resize event
    m_Config.width = 0;
    m_Config.height = 0;
    UpdateWindowSize();

    return true;
}

void Application::AddRenderPassToFront( IRenderPass* pRenderPass )
{
    m_RenderPasses.remove( pRenderPass );
    m_RenderPasses.push_front( pRenderPass );

    pRenderPass->BackBufferResizing();
    pRenderPass->BackBufferResized( m_Config.width, m_Config.height );
}

void Application::AddRenderPassToBack( IRenderPass* pRenderPass )
{
    m_RenderPasses.remove( pRenderPass );
    m_RenderPasses.push_back( pRenderPass );

    pRenderPass->BackBufferResizing();
    pRenderPass->BackBufferResized( m_Config.width, m_Config.height );
}

void Application::RemoveRenderPass( IRenderPass* pRenderPass ) {}

void Application::BackBufferResizing()
{
    for ( auto it : m_RenderPasses )
    {
        it->BackBufferResizing();
    }
}

void Application::BackBufferResized()
{
    for ( auto it : m_RenderPasses )
    {
        it->BackBufferResized( m_Config.width, m_Config.height );
    }
}

void Application::Animate( double deltaTime )
{
    for ( auto it : m_RenderPasses )
    {
        it->Animate( static_cast< float >( deltaTime ) );
    }
}

void Application::Render()
{
    BeginFrame();

    for ( auto it : m_RenderPasses )
    {
        it->Render();
    }
}

void Application::UpdateAverageFrameTime( double deltaTime )
{
    m_FrameTimeSum += deltaTime;
    m_NumberOfAccumulatedFrames += 1;

    if ( m_FrameTimeSum > m_AverageTimeUpdateInterval && m_NumberOfAccumulatedFrames > 0 )
    {
        m_AverageFrameTime = m_FrameTimeSum / static_cast< double >( m_NumberOfAccumulatedFrames );
        m_NumberOfAccumulatedFrames = 0;
        m_FrameTimeSum = 0.0;
    }
}

void Application::RunMainLoop()
{
    m_PreviousFrameTime = static_cast< float >( SDL_GetTicks() ) / 1000.0f;

    while ( !m_ShouldClose )
    {
        // Handle events
        HandleEvents();

        UpdateWindowSize();
        double currentTime = static_cast< float >( SDL_GetTicks() ) / 1000.0f;
        double deltaTime = currentTime - m_PreviousFrameTime;

        if ( m_WindowVisible )
        {
            Animate( deltaTime );
            Render();
            Present();
        }

        UpdateAverageFrameTime( deltaTime );
        m_PreviousFrameTime = currentTime;
    }
}

void Application::GetWindowSize( int& width, int& height ) const
{
    width = static_cast< int >( m_Config.width );
    height = static_cast< int >( m_Config.height );
}

const AppConfig& Application::GetAppConfig()
{
    return m_Config;
}

void Application::HandleEvents()
{
    PollEvents();
    ProcessEvents();
}

void Application::PollEvents()
{
    m_Events.clear();

    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
        m_Events.push_back( event );
    }
}

void Application::ProcessEvents()
{
    for ( auto& event : m_Events )
    {
        for ( auto it = m_RenderPasses.crbegin(); it != m_RenderPasses.crend(); ++it )
        {
            ( *it )->ProcessEvent( event );
        }

        switch ( event.type )
        {
            case SDL_EVENT_QUIT:
                OnWindowCloseEvent();
                break;
            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_MOVED:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                OnWindowEvent( event.window );
                break;
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                OnKeyboardEvent( event.key );
                break;
            case SDL_EVENT_TEXT_INPUT:
                OnTextInputEvent( event.text );
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
                OnMouseButtonEvent( event.button );
                break;
            case SDL_EVENT_MOUSE_MOTION:
                OnMouseMotionEvent( event.motion );
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                OnMouseWheelEvent( event.wheel );
                break;
        }
    }
}

void Application::OnWindowCloseEvent()
{
    m_ShouldClose = true;
}

void Application::OnWindowEvent( const SDL_WindowEvent& event )
{
    if ( event.type == SDL_EVENT_WINDOW_RESIZED )
    {
        UpdateWindowSize();
    }
    else if ( event.type == SDL_EVENT_WINDOW_MOVED )
    {
    }
    else if ( event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
              event.windowID == SDL_GetWindowID( m_Window ) )
    {
        OnWindowCloseEvent();
    }
}

void Application::OnKeyboardEvent( const SDL_KeyboardEvent& event )
{
    if ( event.key == SDLK_UNKNOWN )
    {
        return;
    }

    // Press ESC key to exit the program (This event's priority is the first one.)
    if ( event.key == SDLK_ESCAPE )
    {
        m_ShouldClose = true;
    }

    for ( auto it = m_RenderPasses.crbegin(); it != m_RenderPasses.crend(); ++it )
    {
        bool result = ( *it )->OnKeyboardEvent( event );

        if ( result )
        {
            break;
        }
    }
}

void Application::OnTextInputEvent( const SDL_TextInputEvent& event )
{
    for ( auto it = m_RenderPasses.crbegin(); it != m_RenderPasses.crend(); ++it )
    {
        bool result = ( *it )->OnTextInputEvent( event );

        if ( result )
        {
            break;
        }
    }
}

void Application::OnMouseButtonEvent( const SDL_MouseButtonEvent& event )
{
    for ( auto it = m_RenderPasses.crbegin(); it != m_RenderPasses.crend(); ++it )
    {
        bool result = ( *it )->OnMouseButtonEvent( event );

        if ( result )
        {
            break;
        }
    }
}

void Application::OnMouseMotionEvent( const SDL_MouseMotionEvent& event )
{
    for ( auto it = m_RenderPasses.crbegin(); it != m_RenderPasses.crend(); ++it )
    {
        bool result = ( *it )->OnMouseMotionEvent( event );

        if ( result )
        {
            break;
        }
    }
}

void Application::OnMouseWheelEvent( const SDL_MouseWheelEvent& event )
{
    for ( auto it = m_RenderPasses.crbegin(); it != m_RenderPasses.crend(); ++it )
    {
        bool result = ( *it )->OnMouseWheelEvent( event );

        if ( result )
        {
            break;
        }
    }
}

void Application::UpdateWindowSize()
{
    int width, height;
    SDL_GetWindowSize( m_Window, &width, &height );

    if ( width == 0 || height == 0 )
    {
        // window is minimized
        m_WindowVisible = false;
        return;
    }

    m_WindowVisible = true;

    if ( m_Config.width != width || m_Config.height != height )
    {
        // window is not minimized, and the size is changed
        BackBufferResizing();
        m_Config.width = width;
        m_Config.height = height;
        BackBufferResized();
    }
}

void Application::Shutdown()
{
    SDL_GL_DestroyContext( m_GLContext );

    if ( m_Window )
    {
        SDL_DestroyWindow( m_Window );
        m_Window = nullptr;
    }

    // Call SDL Quit only once
    std::lock_guard< std::recursive_mutex > lock( g_contextManagerMutex );
    assert( g_SDLInitCalls > 0 );
    if ( --g_SDLInitCalls == 0 )
    {
        SDL_Quit();
    }
}

void Application::BeginFrame() {}

void Application::Present()
{
    SDL_GL_SwapWindow( m_Window );
}
