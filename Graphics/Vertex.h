#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED


#include <glm/glm.hpp>


struct Vertex
{
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

};


#endif // VERTEX_H_INCLUDED
