#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED


#include <glm/glm.hpp>


struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

};


#endif // VERTEX_H_INCLUDED
