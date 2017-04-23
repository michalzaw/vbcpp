#ifndef PHYSICALBODYCONVEXHULL_HPP_INCLUDED
#define PHYSICALBODYCONVEXHULL_HPP_INCLUDED

#include "../Utils/Helpers.hpp"
#include "PhysicalBody.hpp"
#include "../Graphics/Vertex.h"

class PhysicalBodyConvexHull : public PhysicalBody
{
    public:
        PhysicalBodyConvexHull(Vertex* vertices, unsigned int vertexCount, btScalar mass);
        PhysicalBodyConvexHull(glm::vec3* vertices, unsigned int vertexCount, btScalar mass);

        virtual ~PhysicalBodyConvexHull();

    protected:
        Vertex* _vertices;

        glm::vec3* _verticesvec;
        unsigned int _vertexCount;

        void updateBody();
};

#endif // PHYSICALBODYCONVEXHULL_HPP_INCLUDED
