#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <string>
#include <glm/glm.hpp>

enum struct LightType
{
    BASIC       = 0,
    DIRECTIONAL = 1,
    POINT       = 2,
    SPOT        = 3
};

struct LightConstants
{
    glm::vec3 color;
};

class Light
{
public:
    std::string name;
    // std::shared_ptr< IShadowMap > parent = nullptr;
    glm::vec3 color = { 1.0f, 1.0f, 1.0f };

    virtual LightType GetLightType() const { return LightType::BASIC; }
    virtual void FillLightConstants( LightConstants& lightConstants ) const;

};

#endif
