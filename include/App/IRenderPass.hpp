#ifndef IRENDER_HPP
#define IRENDER_HPP

#include <cstdint>
#include "App/ContextManager.hpp"

class ContextManager;

class IRenderPass {
public:
    IRenderPass(ContextManager* contextManager) : m_ContextManager(contextManager) {}

    virtual void Render() {}
    virtual void Animate(float deltaTime) {}
    virtual void BackBufferResizing() {}
    virtual void BackBufferResized(const uint32_t width, const uint32_t height) {}

    // Event functions

    inline ContextManager* GetContextManager() { return m_ContextManager; }

private:
    ContextManager* m_ContextManager;

};

#endif