#ifndef UIDATA_HPP
#define UIDATA_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

namespace UI
{
class Brahma;
class Brahma
{
public:
    std::string name;

private:
    Brahma* m_pParent = nullptr;
    std::vector< Brahma* > m_kChildren;

};

struct Akasha : public Brahma
{
    glm::vec3 position = glm::vec3( 0.0f );
};

struct Camera : public Akasha
{
    glm::vec3 target = { 0.0, 0.0, -1.0 };
    glm::vec3 up = { 0.0, 1.0, 0.0 };
    float fovY = 45.0f;
};

struct Mesh
{
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec2 > texCoords;
    std::vector< uint32_t > indices;
};

struct Material
{
    std::string name;
    glm::vec3 color = glm::vec3( 1.0f );
    std::string texturePath;
    bool bUseTexture = false;
};

struct Prop;

struct Light : public Akasha
{
    glm::vec3 color = { 1.0, 1.0, 1.0 };
    std::shared_ptr< Prop > gizmo = nullptr;
};

struct Prop : public Akasha
{
    glm::vec3 scale = glm::vec3( 1.0f );
    glm::vec3 rotation = glm::vec3( 0.0f );  // Euler angles in degrees

    std::shared_ptr< Material > material;
    std::shared_ptr< Mesh > mesh;
};

struct Project
{
public:
    [[nodiscard]] const std::vector< Camera >& GetCameras() const { return m_kCameras; }
    [[nodiscard]] const std::vector< Light >& GetLights() const { return m_kLights; }
    [[nodiscard]] const std::vector< Prop >& GetProps() const { return m_kProps; }

    void AddCamera( Camera& camera ) { m_kCameras.emplace_back( camera ); }
    void AddLight( Light& light ) { m_kLights.emplace_back( light ); }
    void AddObject( Prop& prop ) { m_kProps.emplace_back( prop ); }

private:
    std::vector< Camera > m_kCameras;
    std::vector< Light > m_kLights;
    std::vector< Prop > m_kProps;

};
};  // namespace UI

struct UIData
{
    bool showUI = true;
    bool showDemoWindow = false;
    glm::vec4 clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

    UI::Project project;
    UIData()
    {
        UI::Camera camera;
        camera.name = "Default Camera";
        camera.position = { 0.0, 0.0, 5.0 };
        camera.target = { 0.0, 0.0, 4.0 };
        project.AddCamera( camera );

        auto cube = std::make_shared< UI::Mesh >();
        cube->vertices = {
            { -0.5, -0.5,  0.5 },
            {  0.5, -0.5,  0.5 },
            {  0.5,  0.5,  0.5 },
            { -0.5,  0.5,  0.5 },

            {  0.5, -0.5,  0.5 },
            {  0.5, -0.5, -0.5 },
            {  0.5,  0.5, -0.5 },
            {  0.5,  0.5,  0.5 },

            {  0.5, -0.5, -0.5 },
            { -0.5, -0.5, -0.5 },
            { -0.5,  0.5, -0.5 },
            {  0.5,  0.5, -0.5 },

            { -0.5, -0.5, -0.5 },
            { -0.5, -0.5,  0.5 },
            { -0.5,  0.5,  0.5 },
            { -0.5,  0.5, -0.5 },

            { -0.5,  0.5,  0.5 },
            {  0.5,  0.5,  0.5 },
            {  0.5,  0.5, -0.5 },
            { -0.5,  0.5, -0.5 },

            {  0.5, -0.5,  0.5 },
            { -0.5, -0.5,  0.5 },
            { -0.5, -0.5, -0.5 },
            {  0.5, -0.5, -0.5 }
        };
        cube->normals = {
            {  0.0,  0.0,  1.0 },
            {  0.0,  0.0,  1.0 },
            {  0.0,  0.0,  1.0 },
            {  0.0,  0.0,  1.0 },

            {  1.0,  0.0,  0.0 },
            {  1.0,  0.0,  0.0 },
            {  1.0,  0.0,  0.0 },
            {  1.0,  0.0,  0.0 },

            {  0.0,  0.0, -1.0 },
            {  0.0,  0.0, -1.0 },
            {  0.0,  0.0, -1.0 },
            {  0.0,  0.0, -1.0 },

            { -1.0,  0.0,  0.0 },
            { -1.0,  0.0,  0.0 },
            { -1.0,  0.0,  0.0 },
            { -1.0,  0.0,  0.0 },

            {  0.0,  1.0,  0.0 },
            {  0.0,  1.0,  0.0 },
            {  0.0,  1.0,  0.0 },
            {  0.0,  1.0,  0.0 },

            {  0.0, -1.0,  0.0 },
            {  0.0, -1.0,  0.0 },
            {  0.0, -1.0,  0.0 },
            {  0.0, -1.0,  0.0 },
        };
        cube->texCoords = {
            { 0.0, 0.0 },
            { 1.0, 0.0 },
            { 1.0, 1.0 },
            { 0.0, 1.0 },

            { 0.0, 0.0 },
            { 1.0, 0.0 },
            { 1.0, 1.0 },
            { 0.0, 1.0 },

            { 0.0, 0.0 },
            { 1.0, 0.0 },
            { 1.0, 1.0 },
            { 0.0, 1.0 },

            { 0.0, 0.0 },
            { 1.0, 0.0 },
            { 1.0, 1.0 },
            { 0.0, 1.0 },

            { 0.0, 0.0 },
            { 1.0, 0.0 },
            { 1.0, 1.0 },
            { 0.0, 1.0 },

            { 0.0, 0.0 },
            { 1.0, 0.0 },
            { 1.0, 1.0 },
            { 0.0, 1.0 },
        };
        cube->indices = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 13, 14,
            12, 14, 15,
            16, 17, 18,
            16, 18, 19,
            20, 21, 22,
            20, 22, 23,
        };

        auto lightGizmo = std::make_shared< UI::Prop >();
        lightGizmo->name = "Light Gizmo";
        lightGizmo->scale = { 0.2f, 0.2f, 0.2f };
        lightGizmo->mesh = cube;
        lightGizmo->material = std::make_shared< UI::Material >( UI::Material {
            .name = "Light Gizmo Material",
            .color = { 1.0f, 1.0f, 1.0f },
            .bUseTexture = false,
        } );

        UI::Light light;
        light.name = "Default Light";
        light.position = { 2.0f, 1.0f, -1.0f };
        light.color = { 1.0, 1.0, 1.0 };
        light.gizmo = lightGizmo;
        project.AddLight( light );

        UI::Prop prop;
        prop.name = "RickRoll Cube";
        prop.mesh = cube;
        prop.material = std::make_shared< UI::Material >( UI::Material {
            .name = "RickRoll Material",
            .color = { 1.0f, 1.0f, 1.0f },
            .texturePath = "assets/textures/rickroll.jpg",
            .bUseTexture = true,
        } );
        project.AddObject( prop );
    }
};

#endif
