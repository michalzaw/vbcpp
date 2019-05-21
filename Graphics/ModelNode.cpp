#include "ModelNode.h"


ModelNode::ModelNode(StaticModelNode* staticModelNode, ModelNode* parent)
{
    _name = staticModelNode->name;
    _transformMatrix = staticModelNode->transformMatrix;

    _meshes = staticModelNode->meshes;
    _meshesCount = staticModelNode->meshesCount;

    _parent = parent;
    _childrenCount = staticModelNode->children.size();
    _children = new ModelNode*[_childrenCount];

    for (int i = 0; i < _childrenCount; ++i)
    {
        _children[i] = new ModelNode(staticModelNode->children[i], this);
    }
}


ModelNode::~ModelNode()
{
    if (_children)
    {
        for (int i = 0; i < _childrenCount; ++i)
        {
            delete _children[i];
        }

        delete[] _children;
    }
}


std::string ModelNode::getName()
{
    return _name;
}


glm::mat4& ModelNode::getTransformMatrix()
{
    return _transformMatrix;
}


StaticModelMesh* ModelNode::getMesh(unsigned int i)
{
    return &_meshes[i];
}


unsigned int ModelNode::getMeshesCount()
{
    return _meshesCount;
}


ModelNode* ModelNode::getParent()
{
    return _parent;
}


ModelNode** ModelNode::getChildren()
{
    return _children;
}


unsigned int ModelNode::getChildrenCount()
{
    return _childrenCount;
}
