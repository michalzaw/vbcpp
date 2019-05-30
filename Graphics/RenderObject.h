#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"
#include "ModelNode.h"
#include "AABB.h"

#include "../Utils/RStaticModel.h"

#include "../Scene/Component.h"


class RenderObject : public Component
{
    protected:
        RStaticModel* _model;
        ModelNode*    _modelRootNode;
        Material*     _materials;

        bool _isCastShadows;

        AABB _aabb;
        bool _isCalculatedAABB;

        void calculateNewAABB();

    public:
        RenderObject(RStaticModel* model = NULL);
        virtual ~RenderObject();

        void setModel(RStaticModel* model);
        RStaticModel* getModel();
        ModelNode* getModelRootNode();

        Material* getMaterial(unsigned int index);

        void setIsCastShadows(bool isCastShadows);
        bool isCastShadows();

        AABB* getAABB();

        virtual void changedTransform();

};


#endif // RENDEROBJECT_H_INCLUDED
