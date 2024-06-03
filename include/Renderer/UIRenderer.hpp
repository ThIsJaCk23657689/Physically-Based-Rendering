#ifndef UIRENDERER_HPP
#define UIRENDERER_HPP

#include <deque>
#include <memory>
#include <cstdio>

#include "Renderer/ImGuiRenderer.hpp"
#include "UIData.hpp"

class Application;
class MainRenderer;

class UIRenderer : public ImGuiRenderer {
public:
    UIRenderer(Application* app, std::shared_ptr<MainRenderer> renderer, UIData& ui) :
        ImGuiRenderer(app), m_Renderer(renderer), m_UI(ui) {}

protected:
    void BuildUI() override;

private:
    std::shared_ptr<MainRenderer> m_Renderer;
    UIData& m_UI;
    std::deque<float> m_FrameTimeList;
};

#endif
