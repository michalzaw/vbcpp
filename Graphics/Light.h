#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include <cstdio>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class Light : public Component
{
    private:
        glm::vec3 _color;
        float _ambientIntensity;
        float _diffuseIntensity;

        //glm::vec3 _direction;

        bool _isActive;

    public:
        Light();
        Light(glm::vec3 color, float ambientIntensity, float diffuseIntensity/*, glm::vec3 direction*/);
        virtual ~Light();

        void SetColor(glm::vec3 color);
        void SetAmbientIntensity(float intensity);
        void SetDiffuseIntensity(float intensity);
        //void SetDirection(glm::vec3 direction);
        void SetIsActive(bool isActive);

        glm::vec3 GetColor();
        float GetAmbientIntensity();
        float GetDiffiseIntenisty();
        glm::vec3 GetDirection();
        bool IsActive();

};


#endif // LIGHT_H_INCLUDED
