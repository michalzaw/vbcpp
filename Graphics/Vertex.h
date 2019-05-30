#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED


#include <glm/glm.hpp>

#include "NVMeshMender/NVMeshMender.h"
#include "Vector3.h"


struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    Vertex() {}

    Vertex(const MeshMender::Vertex& meshMenderVertex)
    {
        position = meshMenderVertex.pos.GLMvec();
        texCoord = glm::vec2(meshMenderVertex.s, meshMenderVertex.t);
        normal = meshMenderVertex.normal.GLMvec();
        tangent = meshMenderVertex.tangent.GLMvec();
        bitangent = meshMenderVertex.binormal.GLMvec();
    }

};


#endif // VERTEX_H_INCLUDED
