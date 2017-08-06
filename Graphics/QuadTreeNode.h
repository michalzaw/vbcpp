#ifndef QUADTREENODE_H_INCLUDED
#define QUADTREENODE_H_INCLUDED


#include <list>

#include "AABB.h"
#include "RenderObject.h"
#include "Light.h"
#include "Frustum.h"
#include "RenderData.h"
#include "../Utils/Collision.h"


class QuadTreeNode
{
    private:
        static const int MAX_OBJECTS_PER_NODE = 16;

        AABB _aabb;
        QuadTreeNode* _parent;
        QuadTreeNode* _children[4];

        std::list<RenderObject*> _objects;
        std::list<Light*> _lights;

        bool addObjectToChild(RenderObject* object);

    public:
        QuadTreeNode(AABB aabb);

        QuadTreeNode* getParent();
        QuadTreeNode* getChild(int index);

        void addObject(RenderObject* object);
        void addLight(Light* light);

        void getObjectsInFrustum(std::list<RenderObject*>* objectsList, Frustum& frustum, bool allObjects);

        void splitNode();

};


#endif // QUADTREENODE_H_INCLUDED
