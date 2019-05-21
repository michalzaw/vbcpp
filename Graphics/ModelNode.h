#ifndef MODELNODE_H_INCLUDED
#define MODELNODE_H_INCLUDED


#include "../Utils/RStaticModel.h"


class ModelNode
{
    private:
        std::string _name;

        glm::mat4 _transformMatrix;

        StaticModelMesh*    _meshes;
        unsigned int        _meshesCount;

        ModelNode*          _parent;
        ModelNode**         _children;
        unsigned int        _childrenCount;

    public:
        ModelNode(StaticModelNode* staticModelNode, ModelNode* parent = NULL);
        ~ModelNode();

        std::string getName();
        glm::mat4& getTransformMatrix();

        StaticModelMesh* getMesh(unsigned int i);
        unsigned int getMeshesCount();

        ModelNode* getParent();
        ModelNode** getChildren();
        unsigned int getChildrenCount();

};


#endif // MODELNODE_H_INCLUDED
