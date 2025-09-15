#ifndef IRENDERPASS_HPP
#define IRENDERPASS_HPP

#include <cstdint>

#include "App/Application.hpp"
#include "Core/Graphics.hpp"

class Application;

class IRenderPass
{
public:
    explicit IRenderPass( Application* app ) : m_App( app ) {}
    virtual ~IRenderPass() = default;

    virtual void Render() {}
    virtual void Animate( const float& deltaTime ) {}
    virtual void BackBufferResizing() {}
    virtual void BackBufferResized( const uint32_t width, const uint32_t height ) {}

    // Event functions
    virtual bool ProcessEvent( const SDL_Event& event ) { return false; }
    virtual bool OnKeyboardEvent( const SDL_KeyboardEvent& event ) { return false; }
    virtual bool OnTextInputEvent( const SDL_TextInputEvent& event ) { return false; }
    virtual bool OnMouseButtonEvent( const SDL_MouseButtonEvent& event ) { return false; }
    virtual bool OnMouseMotionEvent( const SDL_MouseMotionEvent& event ) { return false; }
    virtual bool OnMouseWheelEvent( const SDL_MouseWheelEvent& event ) { return false; }

    Application* GetApplication() { return m_App; }

private:
    Application* m_App;
};

#endif
