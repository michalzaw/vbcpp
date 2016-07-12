#include "Light.h"


Light::Light()
    : Component(CT_LIGHT),
    _color(1.0f, 1.0f, 1.0f), _ambientIntensity(0.5f), _diffuseIntensity(1.0f),
    /*_direction(0.0f, 0.0f, 1.0f),*/ _isActive(true)
{


}


Light::Light(glm::vec3 color, float ambientIntensity, float diffuseIntensity/*, glm::vec3 direction*/)
    : Component(CT_LIGHT),
    _color(color), _ambientIntensity(ambientIntensity), _diffuseIntensity(diffuseIntensity),
    /*_direction(0.0f, 0.0f, 1.0f),*/ _isActive(true)
{


}


Light::~Light()
{

}


void Light::SetColor(glm::vec3 color)
{
    _color = color;
}


void Light::SetAmbientIntensity(float intensity)
{
    _ambientIntensity = intensity;
}


void Light::SetDiffuseIntensity(float intensity)
{
    _diffuseIntensity = intensity;
}

/*
void Light::SetDirection(glm::vec3 direction)
{
    _direction = glm::normalize(direction);
}
*/

void Light::SetIsActive(bool isActive)
{
    _isActive = isActive;
}


glm::vec3 Light::GetColor()
{
    return _color;
}


float Light::GetAmbientIntensity()
{
    return _ambientIntensity;
}


float Light::GetDiffiseIntenisty()
{
    return _diffuseIntensity;
}


glm::vec3 Light::GetDirection()
{
    glm::vec4 dir(1.0f, 0.0f, 0.0f, 0.0f);

    dir = _objectTransform->GetNormalMatrix() * dir;

    return glm::vec3(dir);
}


bool Light::IsActive()
{
    return _isActive;
}
