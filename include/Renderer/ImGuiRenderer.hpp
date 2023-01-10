#ifndef IMGUIRENDERER_HPP
#define IMGUIRENDERER_HPP

#include <array>
#include <vector>
#include <memory>
#include <filesystem>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "App/Application.hpp"
#include "IRenderPass.hpp"

class ImGuiRenderer : public IRenderPass {
public:
    ImGuiRenderer(Application* app);
    ~ImGuiRenderer();
    bool Init();
    bool LoadFont(const std::filesystem::path& fontFile, float fontSize);

    void Animate(float deltaTime) override;
    void Render() override;
    void BackBufferResizing() override;

protected:
    virtual void BuildUI() = 0;

    void BeginFullScreenWindow();
    void DrawScreenCenteredText(const char* text);
    void EndFullScreenWindow();

};

#endif
