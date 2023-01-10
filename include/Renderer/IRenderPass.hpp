#ifndef IRENDER_HPP
#define IRENDER_HPP

#include <cstdint>

#include "App/Application.hpp"

class Application;

class IRenderPass {
public:
    IRenderPass(Application* app) : m_App(app) {}

    virtual void Render() {}
    virtual void Animate(float deltaTime) {}
    virtual void BackBufferResizing() {}
    virtual void BackBufferResized(const uint32_t width, const uint32_t height) {}

    // Event functions

    inline Application* GetApplication() { return m_App; }

private:
    Application* m_App;

};

#endif