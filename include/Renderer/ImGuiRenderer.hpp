#ifndef IMGUIRENDERER_HPP
#define IMGUIRENDERER_HPP

#include <array>
#include <vector>
#include <memory>
#include <filesystem>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "App/Application.hpp"
#include "IRenderPass.hpp"

class ImGuiRenderer : public IRenderPass {
public:
    ImGuiRenderer(Application* app);
    ~ImGuiRenderer();
    bool Init();
    bool LoadFont(const std::filesystem::path& fontFile, float fontSize);

    void Render() override;
    void Animate(const float& deltaTime) override;
    void BackBufferResizing() override;

    // Event functions
    bool ProcessEvent(const SDL_Event& event) override;
    bool OnKeyboardEvent(const SDL_KeyboardEvent& event) override;
    bool OnTextInputEvent(const SDL_TextInputEvent& event) override;
    bool OnMouseButtonEvent(const SDL_MouseButtonEvent& event) override;
    bool OnMouseMotionEvent(const SDL_MouseMotionEvent& event) override;
    bool OnMouseWheelEvent(const SDL_MouseWheelEvent& event) override;

protected:
    virtual void BuildUI() = 0;

    void BeginFullScreenWindow();
    void DrawScreenCenteredText(const char* text);
    void EndFullScreenWindow();
};

#endif
