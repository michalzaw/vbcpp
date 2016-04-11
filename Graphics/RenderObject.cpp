#include "RenderObject.h"


RenderObject::RenderObject(Model* model, std::string name)
    : _model(model),
    _name(name),
    _isActive(true)
{

}

RenderObject::~RenderObject()
{
    printf("*** RenderObject: Destruktor\n");

    //if (_model)
    //{
    //    delete _model;
    //    _model = 0;
    //}
}

void RenderObject::SetModel(Model* model)
{
    _model = model;
}


void RenderObject::SetTransform(Transform& transform)
{
    _transform = transform;
}


void RenderObject::SetIsActive(bool isActive)
{
    _isActive = isActive;
}


Model* RenderObject::GetModel()
{
    return _model;

}


Transform* RenderObject::GetTransform()
{

    return &_transform;
}


std::string RenderObject::GetName()
{
    return _name;
}


bool RenderObject::IsActive()
{
    return _isActive;
}
