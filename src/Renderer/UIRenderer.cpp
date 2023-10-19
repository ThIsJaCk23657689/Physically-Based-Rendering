#include "Renderer/UIRenderer.hpp"

void UIRenderer::BuildUI() {
    if (!m_UI.showUI) {
        return;
    }

    const auto& io = ImGui::GetIO();

    if (m_Renderer->IsSceneLoading()) {
        BeginFullScreenWindow();

        char messageBuffer[256];
        std::snprintf(messageBuffer, std::size(messageBuffer), "Loading, please wait...\n");

        DrawScreenCenteredText(messageBuffer);
        EndFullScreenWindow();

        return;
    }

    if (m_UI.showDemoWindow) {
        ImGui::ShowDemoWindow();
    }
    {
        ImGui::Begin("Hello World!");
        ImGui::Checkbox("Demo Window", &m_UI.showDemoWindow);
        ImGui::Text("SceneRenderer average %.3f ms/frame (%.1 FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();
    }
}