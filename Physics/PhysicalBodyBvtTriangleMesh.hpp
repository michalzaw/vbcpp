#ifndef PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
#define PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED

#include  <utility>

#include "../Utils/Helpers.hpp"
#include "PhysicalBody.hpp"
#include "../Graphics/Model.h"
#include "../Utils/RStaticModel.h"

enum class PhysicalBodyBvtTriangleMeshMode
{
    MODELS,
    VERTICES

};

class PhysicalBodyBvtTriangleMesh : public PhysicalBody
{
    VBCPP_COMPONENT(PhysicalBodyBvtTriangleMesh, CT_PHYSICAL_BODY)

    public:
        PhysicalBodyBvtTriangleMesh(RStaticModel* model);
        PhysicalBodyBvtTriangleMesh(const std::list<RStaticModel*>& models);
        PhysicalBodyBvtTriangleMesh(std::vector<glm::vec3>&& vertices);
        virtual ~PhysicalBodyBvtTriangleMesh();

    private:
        std::list<RStaticModel*>  _models;
        std::vector<glm::vec3> _vertices;

        PhysicalBodyBvtTriangleMeshMode _mode;

        void addModelNodeToTriangleMesh(btTriangleMesh* triMesh, StaticModelNode* staticModelNode, unsigned int primitiveType, glm::mat4 parentTransform);
        btTriangleMesh* buildTriangleMeshInModelMode();
        btTriangleMesh* buildTriangleMeshInVerticesMode();
        void updateBody();
};

#endif // PHYSICALBODYBVTTRIANGLEMESH_HPP_INCLUDED
