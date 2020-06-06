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
        virtual ~PhysicalBodyBvtTriangleMesh();

    private:
        RStaticModel*  _model;

        void addModelNodeToTriangleMesh(btTriangleMesh* triMesh, StaticModelNode* staticModelNode, glm::mat4 parentTransform);
        btTriangleMesh* buildTriangleMesh();
        void updateBody();
};

#endif // PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
