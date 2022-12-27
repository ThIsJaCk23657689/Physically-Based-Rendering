#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <vector>
#include <memory>

#include "UIData.hpp"
#include "App/AppConfig.hpp"
#include "App/Application.hpp"
#include "Core/Shader.hpp"

class PBR : public Application
{
public:
    PBR(ContextManager* contextManager, UIData& ui) :
        Super(contextManager), m_UI(ui) {

        // Shaders
        m_Shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

        // Objects
        m_Vertices = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
        };

        m_Indices = {
            0, 1, 2,
            0, 2, 3,
        };

        unsigned int vbo, ebo;
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);
    }

    virtual void Animate(float deltaTime) override {

    }

    virtual void Render() override {
        int windowWidth, windowHeight;
        GetContextManager()->GetWindowSize(windowWidth, windowHeight);

        glViewport(0, 0, static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight));
        glClearColor(m_UI.clearColor.r, m_UI.clearColor.b, m_UI.clearColor.a, m_UI.clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui Frame
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplSDL2_NewFrame();
//        ImGui::NewFrame();

        m_Shader->Start();
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

//        if (m_UI.showDemoWindow) {
//            ImGui::ShowDemoWindow();
//        }
//        {
//            ImGui::Begin("Hello World!");
//            ImGui::Checkbox("Demo Window", &m_UI.showDemoWindow);
//            ImGui::Text("Application average %.3f ms/frame (%.1 FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//            ImGui::End();
//        }
//        ImGui::Render();
    }

private:
    typedef Application Super;
    UIData m_UI;

    // Temp
    std::shared_ptr<Shader> m_Shader = nullptr;
    unsigned int m_VAO;
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;

};

int main(int argc, char** argv) {
    AppConfig config;
    config.width = 1280;
    config.height = 720;
    config.title = "Physically Based Rendering";

    UIData uiData;

    auto* contextManager = new ContextManager();
    if (!contextManager->CreateWindow(config)) {
        return -1;
    }

    {
        auto demo = std::make_shared<PBR>(contextManager, uiData);

        contextManager->AddRenderPassToBack(demo.get());
        contextManager->RunMainLoop();
    }

    contextManager->Shutdown();
    delete contextManager;

    return 0;
}