#ifndef MODELNODE_H_INCLUDED
#define MODELNODE_H_INCLUDED


#include "Transform.h"

#include "../Utils/RStaticModel.h"


class ModelNode
{
    private:
        std::string _name;

		AABB* _aabb;

        Transform _transformNode;
        Transform _transform;
        glm::mat4 _transformMatrix;
        glm::mat4 _normalMatrix;

        StaticModelMesh*    _meshes;
        unsigned int        _meshesCount;

        ModelNode*          _parent;
        ModelNode**         _children;
        unsigned int        _childrenCount;

    public:
        ModelNode(StaticModelNode* staticModelNode, ModelNode* parent = NULL);
        ~ModelNode();

        std::string getName();
		AABB* getAABB();
        Transform& getTransformNode();
        Transform& getTransform();
        glm::mat4& getTransformMatrix();
        glm::mat4& getNormalMatrix();

        StaticModelMesh* getMesh(unsigned int i);
        unsigned int getMeshesCount();

        ModelNode* getParent();
        ModelNode** getChildren();
        unsigned int getChildrenCount();

};


#endif // MODELNODE_H_INCLUDED
