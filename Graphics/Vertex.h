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


#define MAX_BONE_INFLUENCE 4


struct AnimatedVertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    int       boneIds[MAX_BONE_INFLUENCE];
    float     weights[MAX_BONE_INFLUENCE];


    AnimatedVertex() {}

    AnimatedVertex(const MeshMender::Vertex& meshMenderVertex)
    {
        position = meshMenderVertex.pos.GLMvec();
        texCoord = glm::vec2(meshMenderVertex.s, meshMenderVertex.t);
        normal = meshMenderVertex.normal.GLMvec();
        tangent = meshMenderVertex.tangent.GLMvec();
        bitangent = meshMenderVertex.binormal.GLMvec();

        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            boneIds[i] = -1;
            weights[i] = -1;
        }
    }

};


enum class VertexType
{
    STANDARD,
    ANIMATED,

    VERTEX_TYPES_COUNT
};


namespace VerticesUtils
{
    template <typename VertType>
    VertexType getVertexType()
    {
        if (std::is_same<VertType, Vertex>::value)
        {
            return VertexType::STANDARD;
        }
        else if (std::is_same<VertType, AnimatedVertex>::value)
        {
            return VertexType::ANIMATED;
        }
        else
        {
            return VertexType::VERTEX_TYPES_COUNT;
        }
    }

    template <typename VertType>
    void getVerticesPositionsArray(unsigned char* vertices, unsigned int verticesCount, std::vector<glm::vec3>& outVerticesPositions)
    {
        VertType* vert = reinterpret_cast<VertType*>(vertices);
        for (int j = 0; j < verticesCount; ++j)
        {
            outVerticesPositions.push_back(vert[j].position);
        }
    }

    glm::vec3 getVertexPosition(unsigned char* vertices, VertexType vertexType, unsigned int index);
}


#endif // VERTEX_H_INCLUDED
