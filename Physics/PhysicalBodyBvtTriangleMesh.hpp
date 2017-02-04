#ifndef PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
#define PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED

#include "../Utils/Helpers.hpp"
#include "PhysicalBody.hpp"
#include "../Graphics/Model.h"
#include "../Utils/RModel.h"

class PhysicalBodyBvtTriangleMesh : public PhysicalBody
{
    public:
        PhysicalBodyBvtTriangleMesh(RModel* model, btVector3 pos, btVector3 rot);
        virtual ~PhysicalBodyBvtTriangleMesh();

    private:
        RModel*  _model;

        btTriangleMesh* buildTriangleMesh();
        void updateBody();
};

#endif // PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
