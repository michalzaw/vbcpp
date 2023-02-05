#include "Vertex.h"


namespace VerticesUtils
{
    glm::vec3 getVertexPosition(unsigned char* vertices, VertexType vertexType, unsigned int index)
    {
        if (vertexType == VertexType::STANDARD)
        {
            Vertex* verts = reinterpret_cast<Vertex*>(vertices);
            return verts[index].position;
        }
        else if (vertexType == VertexType::ANIMATED)
        {
            AnimatedVertex* verts = reinterpret_cast<AnimatedVertex*>(vertices);
            return verts[index].position;
        }

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}