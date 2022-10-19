#ifndef PHYSICALBODYCONVEXHULL_HPP_INCLUDED
#define PHYSICALBODYCONVEXHULL_HPP_INCLUDED

#include "../Utils/Helpers.hpp"
#include "PhysicalBody.hpp"
#include "../Graphics/Vertex.h"

class PhysicalBodyConvexHull : public PhysicalBody
{
    public:
        PhysicalBodyConvexHull(Vertex* vertices, unsigned int vertexCount, btScalar mass, bool centerOfMassOffset = false, btVector3 centerOfMassOffsetValue = btVector3(0.0f, 0.0f, 0.0f));
        PhysicalBodyConvexHull(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, bool centerOfMassOffset = false, btVector3 centerOfMassOffsetValue = btVector3(0.0f, 0.0f, 0.0f));
        PhysicalBodyConvexHull(std::vector<glm::vec3> vertices, btScalar mass, bool centerOfMassOffset = false, btVector3 centerOfMassOffsetValue = btVector3(0.0f, 0.0f, 0.0f));

        virtual ~PhysicalBodyConvexHull();

    protected:
        std::vector<glm::vec3> _verticesVector;

        Vertex* _vertices;

        glm::vec3* _verticesvec;
        unsigned int _vertexCount;

        void updateBody();
};

#endif // PHYSICALBODYCONVEXHULL_HPP_INCLUDED
