#ifndef PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
#define PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED

#include "../Utils/Helpers.hpp"
#include "PhysicalBody.hpp"
#include "../Graphics/Model.h"

class PhysicalBodyBvtTriangleMesh : public PhysicalBody
{
    public:
        PhysicalBodyBvtTriangleMesh(Model* model, btVector3 pos);
        virtual ~PhysicalBodyBvtTriangleMesh();

    private:
        Model*  _model;

        btTriangleMesh* buildTriangleMesh();
        void updateBody();
};

#endif // PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
