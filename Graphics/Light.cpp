#include "Light.h"


Light::Light(LightType type)
    : Component(CT_LIGHT),
    _lightType(type),
    _color(1.0f, 1.0f, 1.0f), _ambientIntensity(0.5f), _diffuseIntensity(1.0f),
    _cutoff(0.0f)
{


}


Light::Light(LightType type, glm::vec3 color, float ambientIntensity, float diffuseIntensity)
    : Component(CT_LIGHT),
    _lightType(type),
    _color(color), _ambientIntensity(ambientIntensity), _diffuseIntensity(diffuseIntensity),
    _cutoff(0.0f)
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


void Light::SetAttenuation(float constant, float linear, float exp)
{
    _attenuation.constant = constant;
    _attenuation.linear = linear;
    _attenuation.exp = exp;
}


void Light::SetAttenuation(LightAttenuation attenuation)
{
    _attenuation = attenuation;
}


void Light::SetCutoff(float cutoff)
{
    _cutoff = cutoff;
}


LightType Light::GetLightType()
{
    return _lightType;
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

    dir = glm::normalize(_objectTransform->GetNormalMatrix() * dir);

    return glm::vec3(dir);
}


glm::vec3 Light::GetPosition()
{
    return _objectTransform->GetPosition();
}


LightAttenuation& Light::GetAttenuation()
{
    return _attenuation;
}


float Light::GetCutoff()
{
    return _cutoff;
}
