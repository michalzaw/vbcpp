#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"
#include "../Utils/RModel.h"

#include "../Scene/Component.h"


class RenderObject : public Component
{
    protected:
        //Model* _model;
        RModel* _model;

    public:
        RenderObject(RModel* model = NULL);
        virtual ~RenderObject();

        //void SetModel(Model* model);

        //Model* GetModel();

        void setModel(RModel* model);

        RModel* getModel();

};


#endif // RENDEROBJECT_H_INCLUDED
