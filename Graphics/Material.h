#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED


#include <glm/glm.hpp>

#include "Shader.h"


struct Material
{
    // ! Jakie parametry !
    std::string name;

    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;

    GLuint diffuseTexture;

    float shininess;
    float transparency;

    glm::vec2 offset;
    glm::vec2 scale;

    Shader* shader;

};


#endif // MATERIAL_H_INCLUDED
