#include "Renderer/MainRenderer.hpp"
#include "glm/glm.hpp"

MainRenderer::MainRenderer(Application* app, UIData& ui) :
    Super(app), m_UI(ui) {

    m_Graphics = GetApplication()->GetGraphics();

    // Texture: TextureCache
    m_TextureCache = std::make_shared<TextureCache>(m_Graphics);

    // Shaders: ShaderFactory
    m_Shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // CommonPasses

    // ShadowMap (CascadedShadowMap) FrameBufferFactory

    // Camera

    // Objects
//    m_Vertices = {
//        // position             // texcoord
//        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
//        0.5f, -0.5f, 0.0f,      2.0f, 0.0f,
//        0.5f,  0.5f, 0.0f,      2.0f, 2.0f,
//        -0.5f,  0.5f, 0.0f,     0.0f, 2.0f,
//    };
//
//    m_Indices = {
//        0, 1, 2,
//        0, 2, 3,
//    };
//
//    // OpenGL 4.5
//    int binding_index = 0;
//    int offset = 0;
//    int stride = 5 * sizeof(float);
//    {
//        glCreateBuffers(1, &m_VBO);
//        glNamedBufferStorage(m_VBO, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_MAP_WRITE_BIT);
//
//        glCreateBuffers(1, &m_EBO);
//        glNamedBufferStorage(m_EBO, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_MAP_WRITE_BIT);
//
//        glCreateVertexArrays(1, &m_VAO);
//        glVertexArrayVertexBuffer(m_VAO, binding_index, m_VBO, offset, stride);
//        glVertexArrayElementBuffer(m_VAO, m_EBO);
//
//        glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
//        glVertexArrayAttribBinding(m_VAO, 0, binding_index);
//        glEnableVertexArrayAttrib(m_VAO, 0);
//
//        glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
//        glVertexArrayAttribBinding(m_VAO, 1, binding_index);
//        glEnableVertexArrayAttrib(m_VAO, 1);
//    }


    // Loaded Scene
    SetAsynchronousLoadingEnabled(true);
    SetCurrentScene("The World of Rick Roll");

    // Load EnvironmentMap
    m_Texture = m_TextureCache->LoadTextureFromFile("assets/textures/rickroll.jpg", false);
    // m_TextureCache->LoadTextureFromFileDeferred("assets/textures/rickroll.jpg", false);
}

void MainRenderer::SceneLoaded() {
    Super::SceneLoaded();
    m_Scene->CreateRenderingResources(m_Graphics);
}

void MainRenderer::Animate(float deltaTime) {

}

void MainRenderer::RenderScene() {
    int windowWidth, windowHeight;
    GetApplication()->GetWindowSize(windowWidth, windowHeight);
    float aspect = windowWidth / static_cast<float>(windowHeight);

    m_Graphics->SetViewport(0, 0, windowWidth, windowHeight);
    m_Graphics->ClearCache(m_UI.clearColor);

    m_Shader->Start();
    m_Shader->SetInt("MySampler", 0);

    auto model = glm::mat4(1.0f);
    auto view = glm::lookAt(glm::vec3(0, 0, 2), {0, 0, 0}, {0, 1, 0});
    auto projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);

    m_Shader->SetMat4("Model", model);
    m_Shader->SetMat4("View", view);
    m_Shader->SetMat4("Projection", projection);

    // Temp
    const auto& instances = m_Scene->GetMeshInstances();
    for (const auto& instance : instances)
    {
        const auto& vao = instance->mesh->buffers->vertexArray->GetID();
        const auto& indexCount = instance->mesh->numIndices;

        m_Graphics->DrawIndexed(vao, m_Texture->texture->GetID(), indexCount);
    }
}