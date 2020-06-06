#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED


#include "QuadTreeNode.h"


class QuadTree
{
    private:
        QuadTreeNode* _root;

    public:
        QuadTree(glm::vec3 size);
        ~QuadTree();

        void addObject(RenderObject* object);
        void addLight(Light* light);

        void getObjectsInFrustum(std::list<RenderObject*>* objectsList, Frustum& frustum);

};


#endif // QUADTREE_H_INCLUDED
