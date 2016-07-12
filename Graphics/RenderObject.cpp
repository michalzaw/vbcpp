#include "RenderObject.h"


RenderObject::RenderObject(Model* model)
    : Component(CT_RENDER_OBJECT),
    _model(model)
{
    #ifdef _DEBUG_MODE
        printf("*** RenderObject: Konstruktor\n");
    #endif // _DEBUG_MODE
}

RenderObject::~RenderObject()
{
    #ifdef _DEBUG_MODE
        printf("*** RenderObject: Destruktor\n");
    #endif // _DEBUG_MODE
}


void RenderObject::SetModel(Model* model)
{
    _model = model;
}


Model* RenderObject::GetModel()
{
    return _model;

}
