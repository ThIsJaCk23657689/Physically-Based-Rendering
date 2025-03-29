#ifndef UIDATA_HPP
#define UIDATA_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace UI
{
struct BaseNode
{
    std::string name;
    glm::vec3 position = glm::vec3( 0.0f );
};

struct Camera : public BaseNode
{
    glm::vec3 target = { 0.0, 0.0, -1.0 };
    glm::vec3 up = { 0.0, 1.0, 0.0 };
    float fovY = 45.0f;
};

struct Light : public BaseNode
{
};

struct Project
{
    void AddCamera( Camera& camera ) { m_kCameras.push_back( camera ); }

    std::vector< Camera > m_kCameras;
    std::vector< Light > m_kLights;
    std::vector< BaseNode > m_kObjects;
};
};  // namespace UI

struct UIData
{
    bool showUI = true;
    bool showDemoWindow = true;
    glm::vec4 clearColor = { 0.45f, 0.55f, 0.60f, 1.0f };

    UI::Project project;
    UIData()
    {
        UI::Camera camera;
        camera.name = "Default Camera";
        camera.position = { 0.0, 0.0, 5.0 };
        camera.target = { 0.0, 0.0, 4.0 };

        project.AddCamera( camera );
    }
};

#endif
