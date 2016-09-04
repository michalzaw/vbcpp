#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"

#include "../Scene/Component.h"


class RenderObject : public Component
{
    private:
        Model* _model;

    public:
        RenderObject(Model* model = NULL);
        virtual ~RenderObject();

        void SetModel(Model* model);

        Model* GetModel();

};


#endif // RENDEROBJECT_H_INCLUDED
