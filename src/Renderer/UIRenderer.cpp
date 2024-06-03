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
        if (ImGui::BeginTabBar("TabBar")) {
            if (ImGui::BeginTabItem("General")) {
                ImGui::Checkbox("Demo Window", &m_UI.showDemoWindow);
                ImGui::Text("SceneRenderer average %.3f ms/frame (%.1 FPS)",
                            1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Camera")) {
                const auto& camera = m_Renderer->GetMainCamera();
                ImGui::Text("Position = (%.2f, %.2f, %.2f)", camera->m_Position.x, camera->m_Position.y, camera->m_Position.z);
                ImGui::Text("Velocity = (%.2f, %.2f, %.2f)", camera->m_Velocity.x, camera->m_Velocity.y, camera->m_Velocity.z);
                ImGui::Text("Front = (%.2f, %.2f, %.2f)", camera->m_Front.x, camera->m_Front.y, camera->m_Front.z);
                ImGui::Text("Right = (%.2f, %.2f, %.2f)", camera->m_Right.x, camera->m_Right.y, camera->m_Right.z);
                ImGui::Text("Up = (%.2f, %.2f, %.2f)", camera->m_Up.x, camera->m_Up.y, camera->m_Up.z);
                ImGui::Text("Pitch = %.2f deg", camera->m_Pitch);
                ImGui::Text("Yaw = %.2f deg", camera->m_Yaw);
                ImGui::Text("Fov = %2.f deg", camera->m_VerticalFov);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}