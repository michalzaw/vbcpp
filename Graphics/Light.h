#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include <cstdio>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


enum LightType
{
    LT_DIRECTIONAL,
    LT_POINT,
    LT_SPOT

};


struct LightAttenuation
{
    float constant;
    float linear;
    float exp;

    LightAttenuation(float c = 0.0f, float l = 0.0f, float e = 0.0f)
        : constant(c), linear(l), exp(e) {}

};


class Light : public Component
{
    private:
        LightType _lightType;

        glm::vec3 _color;
        float _ambientIntensity;
        float _diffuseIntensity;

        LightAttenuation _attenuation;

        float _cutoff;

    public:
        Light(LightType type);
        Light(LightType type, glm::vec3 color, float ambientIntensity, float diffuseIntensity);
        ~Light();

        void SetColor(glm::vec3 color);
        void SetAmbientIntensity(float intensity);
        void SetDiffuseIntensity(float intensity);
        void SetAttenuation(float constant, float linear, float exp);
        void SetAttenuation(LightAttenuation attenuation);
        void SetCutoff(float cutoff);

        LightType           GetLightType();
        glm::vec3           GetColor();
        float               GetAmbientIntensity();
        float               GetDiffiseIntenisty();
        glm::vec3           GetDirection();
        glm::vec3           GetPosition();
        LightAttenuation&   GetAttenuation();
        float               GetCutoff();

};


#endif // LIGHT_H_INCLUDED
