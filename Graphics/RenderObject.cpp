#include "RenderObject.h"

#include "GraphicsManager.h"

RenderObject::RenderObject(RModel* model)
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


void RenderObject::SetModel(RModel* model)
{
    _model = model;
}


RModel* RenderObject::GetModel()
{
    return _model;

}
