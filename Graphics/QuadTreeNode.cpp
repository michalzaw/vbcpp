#include "QuadTreeNode.h"


QuadTreeNode::QuadTreeNode(AABB aabb)
    : _aabb(aabb),
    _parent(NULL),
    _children{NULL, NULL, NULL, NULL}
{

}


bool QuadTreeNode::addObjectToChild(RenderObject* object)
{

    for (int i = 0; i < 4; ++i)
    {
        if (isAABBInAABB(_children[i]->_aabb, *object->getAABB()))
        {
            _children[i]->addObject(object);

            return true;
        }
    }

    return false;
}


QuadTreeNode* QuadTreeNode::getParent()
{
    return _parent;
}


QuadTreeNode* QuadTreeNode::getChild(int index)
{
    return _children[index];
}


void QuadTreeNode::addObject(RenderObject* object)
{
    if (_objects.size() >= MAX_OBJECTS_PER_NODE)
    {
        if (_children[0] != NULL)
        {
            bool isAddedToChild = addObjectToChild(object);

            if (!isAddedToChild)
            {
                _objects.push_back(object);
            }
        }
        else
        {
            splitNode();
            addObject(object);
        }
    }
    else
    {
        _objects.push_back(object);
    }
}


void QuadTreeNode::addLight(Light* light)
{
    _lights.push_back(light);
}


void QuadTreeNode::getObjectsInFrustum(std::list<RenderObject*>* objectsList, Frustum& frustum, bool allObjects)
{
    if (allObjects || isAABBInFrustum(frustum, _aabb))
    {
        for (std::list<RenderObject*>::iterator i = _objects.begin(); i != _objects.end(); ++i)
        {
            if (isAABBIntersectFrustum(frustum, *(*i)->getAABB()))
                objectsList->push_back(*i);
        }

        for (int i = 0; i < 4; ++i)
        {
            if (_children[i] != NULL)
                _children[i]->getObjectsInFrustum(objectsList, frustum, true);
        }
    }
    else if (isAABBIntersectFrustum(frustum, _aabb))
    {
        for (std::list<RenderObject*>::iterator i = _objects.begin(); i != _objects.end(); ++i)
        {
            if (isAABBIntersectFrustum(frustum, *(*i)->getAABB()))
                objectsList->push_back(*i);
        }

        for (int i = 0; i < 4; ++i)
        {
            if (_children[i] != NULL)
                _children[i]->getObjectsInFrustum(objectsList, frustum, false);
        }
    }
}


void QuadTreeNode::splitNode()
{
    glm::vec3 center = _aabb.getCenterPosition();
    glm::vec3 min = _aabb.getMinCoords();
    glm::vec3 max = _aabb.getMaxCoords();

    _children[0] = new QuadTreeNode(AABB(min, glm::vec3(center.x, max.y, center.z)));
    _children[1] = new QuadTreeNode(AABB(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, max.y, center.z)));
    _children[2] = new QuadTreeNode(AABB(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, max.y, max.z)));
    _children[3] = new QuadTreeNode(AABB(glm::vec3(center.x, min.y, center.z), max));

    _children[0]->_parent = this;
    _children[1]->_parent = this;
    _children[2]->_parent = this;
    _children[3]->_parent = this;

    for (std::list<RenderObject*>::iterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        if (addObjectToChild(*i))
        {
            i = _objects.erase(i);
            if (i == _objects.end())
                break;
        }
    }
}
