#include "Light.h"

#include "../Scene/SceneObject.h"


Light::Light(LightType type)
    : Component(CT_LIGHT),
    _lightType(type),
    _color(1.0f, 1.0f, 1.0f), _ambientIntensity(0.5f), _diffuseIntensity(1.0f),
    _cutoff(0.0f), _cutoffCos(1.0f)
{


}


Light::Light(LightType type, glm::vec3 color, float ambientIntensity, float diffuseIntensity)
    : Component(CT_LIGHT),
    _lightType(type),
    _color(color), _ambientIntensity(ambientIntensity), _diffuseIntensity(diffuseIntensity),
    _cutoff(0.0f), _cutoffCos(1.0f)
{


}


Light::~Light()
{

}


void Light::setColor(glm::vec3 color)
{
    _color = color;
}


void Light::setAmbientIntensity(float intensity)
{
    _ambientIntensity = intensity;
}


void Light::setDiffuseIntensity(float intensity)
{
    _diffuseIntensity = intensity;
}


void Light::setAttenuation(float constant, float linear, float exp)
{
    _attenuation.constant = constant;
    _attenuation.linear = linear;
    _attenuation.exp = exp;
}


void Light::setAttenuation(LightAttenuation attenuation)
{
    _attenuation = attenuation;
}


void Light::setCutoff(float cutoff)
{
    _cutoff = cutoff;

    _cutoffCos = cosf(_cutoff);
}


LightType Light::getLightType()
{
    return _lightType;
}


glm::vec3 Light::getColor()
{
    return _color;
}


float Light::getAmbientIntensity()
{
    return _ambientIntensity;
}


float Light::getDiffiseIntenisty()
{
    return _diffuseIntensity;
}


glm::vec3 Light::getDirection()
{
    glm::vec4 dir(1.0f, 0.0f, 0.0f, 0.0f);

    //dir = glm::normalize(_objectTransform->GetNormalMatrix() * dir);

    //return glm::vec3(dir);

    //return glm::normalize(glm::vec3(getGlobalTransform()->getNormalMatrix() * dir));;
    float verticalAngle = getGlobalTransform()->getRotation().x;
    float horizontalAngle = getGlobalTransform()->getRotation().y;

    glm::vec4 vec(
                    cos(verticalAngle) * sin(horizontalAngle),
                    sin(verticalAngle),
                    cos(verticalAngle) * cos(horizontalAngle),
                    0.0f
                     );

    if (_object->hasParent())
        vec = _object->getParent()->getGlobalTransform()->getNormalMatrix() * vec;

    return glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
}


glm::vec3 Light::getPosition()
{
    glm::vec4 pos = getGlobalTransform()->getTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return glm::vec3(pos.x, pos.y, pos.z);
    //return getGlobalTransform()->getPosition();
}


LightAttenuation& Light::getAttenuation()
{
    return _attenuation;
}


float Light::getCutoff()
{
    return _cutoff;
}


float Light::getCutoffCos()
{
    return _cutoffCos;
}
