#ifndef IRENDER_HPP
#define IRENDER_HPP

#include <cstdint>

#include "App/Application.hpp"
#include "Core/Graphics.hpp"

class Application;

class IRenderPass {
public:
    IRenderPass(Application* app) : m_App(app) {}

    virtual void Render() {}
    virtual void Animate(float deltaTime) {}
    virtual void BackBufferResizing() {}
    virtual void BackBufferResized(const uint32_t width, const uint32_t height) {}

    // Event functions
    virtual bool ProcessEvent(const SDL_Event& event) { return false; }
    virtual bool OnKeyboardEvent(const SDL_KeyboardEvent& event) { return false; }
    virtual bool OnTextInputEvent(const SDL_TextInputEvent& event) { return false; }
    virtual bool OnMouseButtonEvent(const SDL_MouseButtonEvent& event) { return false; }
    virtual bool OnMouseMotionEvent(const SDL_MouseMotionEvent& event) { return false; }
    virtual bool OnMouseWheelEvent(const SDL_MouseWheelEvent& event) { return false; }

    inline Application* GetApplication() { return m_App; }

private:
    Application* m_App;

};

#endif