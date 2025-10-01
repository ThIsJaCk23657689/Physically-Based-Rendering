#include "Scene/Light.hpp"

void Light::FillLightConstants( LightConstants& lightConstants ) const
{
    lightConstants.color = color;
}
