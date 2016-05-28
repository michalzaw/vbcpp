#include "RenderObject.h"


RenderObject::RenderObject(Model* model, std::string name)
    : _model(model),
    _physicalBody(0),
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


void RenderObject::SetPhysicalBody(PhysicalBody* body)
{
    _physicalBody = body;
}


void RenderObject::Update()
{
    /*
    btTransform trans;
    boxBody1->getTransform(trans);
    object1->GetTransform()->SetPosition(glm::vec3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));
    object1->GetTransform()->SetRotation(trans.getRotation().getX(),trans.getRotation().getY(),trans.getRotation().getZ(),trans.getRotation().getW());
    */

    if (IsActive())
    {
        if ( _physicalBody && _physicalBody->getMass() > 0)
        {
            btTransform transf;
            _physicalBody->getTransform(transf);

            _transform.SetPosition(glm::vec3(transf.getOrigin().getX(),transf.getOrigin().getY(),transf.getOrigin().getZ()));
            _transform.SetRotation(transf.getRotation().getX(),transf.getRotation().getY(),transf.getRotation().getZ(),transf.getRotation().getW());
        }
    }
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
