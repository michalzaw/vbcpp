#ifndef PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
#define PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED

#include "../Utils/Helpers.hpp"
#include "PhysicalBody.hpp"
#include "../Graphics/Model.h"
#include "../Utils/RStaticModel.h"

class PhysicalBodyBvtTriangleMesh : public PhysicalBody
{
    public:
        PhysicalBodyBvtTriangleMesh(RStaticModel* model);
        PhysicalBodyBvtTriangleMesh(const std::list<RStaticModel*>& model);
        virtual ~PhysicalBodyBvtTriangleMesh();

    private:
        std::list<RStaticModel*>  _models;

        void addModelNodeToTriangleMesh(btTriangleMesh* triMesh, StaticModelNode* staticModelNode, unsigned int primitiveType, glm::mat4 parentTransform);
        btTriangleMesh* buildTriangleMesh();
        void updateBody();
};

#endif // PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
