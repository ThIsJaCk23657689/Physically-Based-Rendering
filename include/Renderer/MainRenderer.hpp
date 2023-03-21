#ifndef MAINRENDERER_HPP
#define MAINRENDERER_HPP

#include <vector>

#include "Core/Shader.hpp"
#include "Renderer/SceneRenderer.hpp"
#include "UIData.hpp"

class MainRenderer : public SceneRenderer {
public:
    MainRenderer(Application* app, UIData& ui);
    void Animate(float deltaTime) override;
    void Render() override;

private:
    typedef SceneRenderer Super;
    UIData m_UI;

    // Temp
    std::shared_ptr<Shader> m_Shader = nullptr;
    unsigned int m_VAO, m_VBO, m_EBO, m_Sampler;
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::shared_ptr<LoadedTexture> m_Texture;

};

#endif