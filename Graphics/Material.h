#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED


#include <glm/glm.hpp>

#include "RShader.h"
#include <memory>
using namespace std;

#include "../Utils/RTexture.h"

class Material
{
    public:
        Material()
        : diffuseTexture(0), normalmapTexture(0), shininess(0), transparency(0), _shader(0) {}
        virtual ~Material()
        { }

    // ! Jakie parametry !
    std::string name;

    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;

    GLuint diffuseTexture;
    GLuint normalmapTexture;

    float shininess;
    float transparency;

    glm::vec2 offset;
    glm::vec2 scale;

    RShader* _shader;
};


#endif // MATERIAL_H_INCLUDED
