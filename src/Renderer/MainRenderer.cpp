#include "Renderer/MainRenderer.hpp"

MainRenderer::MainRenderer(Application* app, UIData& ui) :
    Super(app), m_UI(ui) {

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

    // OpenGL 3.3
//    {
//        glGenVertexArrays(1, &m_VAO);
//        glBindVertexArray(m_VAO);
//
//        glGenBuffers(1, &m_VBO);
//        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
//
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
//
//        glGenBuffers(1, &m_EBO);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);
//    }

    // OpenGL 4.5
    int binding_index = 2;
    int attribute_index = 0;
    int size = 3;
    int offset = 0;
    int stride = size * sizeof(float);
    {
        glCreateVertexArrays(1, &m_VAO);

        glCreateBuffers(1, &m_VBO);
        glNamedBufferStorage(m_VBO, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_MAP_WRITE_BIT);
        glVertexArrayVertexBuffer(m_VAO, binding_index, m_VBO, offset, stride);
        glVertexArrayAttribFormat(m_VAO, attribute_index, size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_VAO, attribute_index, binding_index);
        glEnableVertexArrayAttrib(m_VAO, attribute_index);

        glCreateBuffers(1, &m_EBO);
        glNamedBufferStorage(m_EBO, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_MAP_WRITE_BIT);
        glVertexArrayElementBuffer(m_VAO, m_EBO);
    }
}

void MainRenderer::Animate(float deltaTime) {

}

void MainRenderer::Render() {
    int windowWidth, windowHeight;
    GetApplication()->GetWindowSize(windowWidth, windowHeight);

    glViewport(0, 0, static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight));
    glClearColor(m_UI.clearColor.r, m_UI.clearColor.b, m_UI.clearColor.a, m_UI.clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->Start();
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}