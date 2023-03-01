#include "Renderer/MainRenderer.hpp"
#include "Utility/ImageLoader.hpp"

#include "glm/glm.hpp"

MainRenderer::MainRenderer(Application* app, UIData& ui) :
    Super(app), m_UI(ui) {

    // Shaders
    m_Shader = std::make_shared<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // Objects
    m_Vertices = {
        // position             // texcoord
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     2.0f, 0.0f,
         0.5f,  0.5f, 0.0f,     2.0f, 2.0f,
        -0.5f,  0.5f, 0.0f,     0.0f, 2.0f,
    };

    m_Indices = {
        0, 1, 2,
        0, 2, 3,
    };

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("assets/textures/rickroll.jpg", &width, &height, &channels, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // OpenGL 3.3
    {
//        glGenTextures(1, &m_Texture);
//        glBindTexture(GL_TEXTURE_2D, m_Texture);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // OpenGL 4.5
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);
        glTextureStorage2D(m_Texture, 1, GL_RGB8, width, height);
        glTextureSubImage2D(m_Texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

//        glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTextureParameteri(m_Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTextureParameteri(m_Texture, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

        glGenerateMipmap(m_Texture);

        glCreateSamplers(1, &m_Sampler);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindSampler(0, m_Sampler);
    }


    stbi_image_free(data);




    // OpenGL 4.5
    int binding_index = 0;
    int offset = 0;
    int stride = 5 * sizeof(float);
    {
        glCreateVertexArrays(1, &m_VAO);

        glCreateBuffers(1, &m_VBO);
        glNamedBufferStorage(m_VBO, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_MAP_WRITE_BIT);
        glVertexArrayVertexBuffer(m_VAO, binding_index, m_VBO, offset, stride);

        glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
        glVertexArrayAttribBinding(m_VAO, 0, binding_index);
        glVertexArrayAttribBinding(m_VAO, 1, binding_index);
        glEnableVertexArrayAttrib(m_VAO, 0);
        glEnableVertexArrayAttrib(m_VAO, 1);

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
    float aspect = windowWidth / static_cast<float>(windowHeight);

    glViewport(0, 0, static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight));
    glClearColor(m_UI.clearColor.r, m_UI.clearColor.b, m_UI.clearColor.a, m_UI.clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->Start();
    m_Shader->SetInt("MySampler", 0);

    auto model = glm::mat4(1.0f);
    auto view = glm::lookAt(glm::vec3(0, 0, 2), {0, 0, 0}, {0, 1, 0});
    auto projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);

    m_Shader->SetMat4("Model", model);
    m_Shader->SetMat4("View", view);
    m_Shader->SetMat4("Projection", projection);

    glBindVertexArray(m_VAO);
    glBindTextureUnit(0, m_Texture);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_Texture);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
}