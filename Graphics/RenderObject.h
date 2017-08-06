#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"
#include "AABB.h"

#include "../Utils/RModel.h"

#include "../Scene/Component.h"


class RenderObject : public Component
{
    protected:
        RModel* _model;

        AABB _aabb;
        bool _isCalculatedAABB;

        void calculateNewAABB();

    public:
        RenderObject(RModel* model = NULL);
        virtual ~RenderObject();

        void setModel(RModel* model);
        RModel* getModel();

        AABB* getAABB();

        virtual void changedTransform();

};


#endif // RENDEROBJECT_H_INCLUDED
