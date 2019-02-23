#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED


#include <glm/glm.hpp>

#include "RShader.h"
#include "../Utils/RTexture.h"
#include <memory>
using namespace std;

#include "../Utils/RTexture.h"

class Material
{
    public:
        Material()
        : diffuseTexture(NULL), normalmapTexture(NULL), glassTexture(NULL), shininess(0), transparency(0), shader(SOLID_MATERIAL) {}
        virtual ~Material()
        { }

    // ! Jakie parametry !
    std::string name;

    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;

    RTexture* diffuseTexture;
    RTexture* normalmapTexture;
    RTexture* glassTexture;

    float shininess;
    float transparency;

    glm::vec2 offset;
    glm::vec2 scale;

    //RShader* _shader;
    ShaderType shader;

};


#endif // MATERIAL_H_INCLUDED
