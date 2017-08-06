#include "QuadTree.h"

QuadTree::QuadTree(glm::vec3 size)
{
    size /= 2.0f;

    _root = new QuadTreeNode(AABB(-size, size));
}


QuadTree::~QuadTree()
{
    delete _root;
}


void QuadTree::addObject(RenderObject* object)
{
    _root->addObject(object);
}


void QuadTree::addLight(Light* light)
{
    _root->addLight(light);
}


void QuadTree::getObjectsInFrustum(std::list<RenderObject*>* objectsList, Frustum& frustum)
{
    _root->getObjectsInFrustum(objectsList, frustum, false);
}
