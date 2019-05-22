#ifndef MODELNODE_H_INCLUDED
#define MODELNODE_H_INCLUDED


#include "Transform.h"

#include "../Utils/RStaticModel.h"


class ModelNode
{
    private:
        std::string _name;

        Transform _transform;

        StaticModelMesh*    _meshes;
        unsigned int        _meshesCount;

        ModelNode*          _parent;
        ModelNode**         _children;
        unsigned int        _childrenCount;

    public:
        ModelNode(StaticModelNode* staticModelNode, ModelNode* parent = NULL);
        ~ModelNode();

        std::string getName();
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
