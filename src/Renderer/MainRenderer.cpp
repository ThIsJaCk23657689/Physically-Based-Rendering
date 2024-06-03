#include "Renderer/MainRenderer.hpp"
#include "glm/glm.hpp"

MainRenderer::MainRenderer(Application* app, UIData& ui) : Super(app), m_UI(ui) {
    m_Graphics = GetApplication()->GetGraphics();

    // Texture: TextureCache
    m_TextureCache = std::make_shared<TextureCache>(m_Graphics);

    // Shaders: ShaderFactory
    m_Shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // CommonPasses

    // ShadowMap (CascadedShadowMap) FrameBufferFactory

    m_Camera = std::make_unique<FPSCamera>();
    m_Camera->SetMoveSpeed(3.0f);

    // Loaded Scene
    SetAsynchronousLoadingEnabled(true);
    SetCurrentScene("The World of Rick Roll");

    // Load EnvironmentMap
    m_Texture = m_TextureCache->LoadTextureFromFile("assets/textures/rickroll.jpg", false);
    // m_TextureCache->LoadTextureFromFileDeferred("assets/textures/rickroll.jpg", false);
}

void MainRenderer::SetCurrentScene(const std::string& sceneName) {
    if (m_CurrentSceneName == sceneName) {
        return;
    }

    m_CurrentSceneName = sceneName;
    BeginLoadingScene();
}

bool MainRenderer::SetupView() {
    return true;
}

void MainRenderer::CreateRenderPasses() {}

void MainRenderer::RenderScene() {
    int windowWidth, windowHeight;
    GetApplication()->GetWindowSize(windowWidth, windowHeight);
    float aspect = windowWidth / static_cast<float>(windowHeight);

    m_Graphics->SetViewport(0, 0, windowWidth, windowHeight);
    m_Graphics->ClearCache(m_UI.clearColor);

    m_Shader->Start();
    m_Shader->SetInt("MySampler", 0);

    auto model = glm::mat4(1.0f);
    auto view = m_Camera->GetWorldToViewMatrix();
    auto projection = glm::perspective(glm::radians(m_Camera->m_VerticalFov), aspect, 0.1f, 1000.0f);

    m_Shader->SetMat4("Model", model);
    m_Shader->SetMat4("View", view);
    m_Shader->SetMat4("Projection", projection);

    // Temp
    const auto& instances = m_Scene->GetMeshInstances();
    for (const auto& instance : instances) {
        const auto& vao = instance->mesh->buffers->vertexArray;
        const auto& indexCount = instance->mesh->numIndices;

        m_Graphics->DrawIndexed(vao, m_Texture->texture, indexCount);
    }
}

void MainRenderer::RenderSplashScreen() {
    GetApplication()->SetVsyncEnabled(true);
}

bool MainRenderer::LoadScene() {
    Scene* scene = new Scene();

    // load ui scene data to create real scene object
    if (scene->Load(m_UI.project)) {
        m_Scene = std::unique_ptr<Scene>(scene);
        return true;
    }

    return false;
}

void MainRenderer::SceneLoaded() {
    Super::SceneLoaded();
    m_Scene->CreateRenderingResources(m_Graphics);

    // Draw Strategy

    // Process with lights

    // Process with Camera
    m_Camera->LookAt(m_Scene->DefaultCamera->position, m_Scene->DefaultCamera->target);
    m_Camera->SetVerticalFov(m_Scene->DefaultCamera->fovY);
}

void MainRenderer::SceneUnloading() {}

void MainRenderer::Animate(const float& deltaTime) {
    m_Camera->Animate(deltaTime);
}

bool MainRenderer::OnKeyboardEvent(const SDL_KeyboardEvent& event) {
    m_Camera->OnKeyboardEvent(event);
    return true;
}

bool MainRenderer::OnMouseButtonEvent(const SDL_MouseButtonEvent& event) {
    m_Camera->OnMouseButtonEvent(event);
    return true;
}

bool MainRenderer::OnMouseMotionEvent(const SDL_MouseMotionEvent& event) {
    m_Camera->OnMouseMotionEvent(event);
    return true;
}

bool MainRenderer::OnMouseWheelEvent(const SDL_MouseWheelEvent& event) {
    m_Camera->OnMouseWheelEvent(event);
    return true;
}
