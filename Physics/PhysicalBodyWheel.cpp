#include "PhysicalBodyWheel.h"


PhysicalBodyWheel::PhysicalBodyWheel(btRaycastVehicle* vehicle, int index)
    : PhysicalBody(2), _vehicle(vehicle), _index(index)
{

}

PhysicalBodyWheel::~PhysicalBodyWheel()
{

}


void PhysicalBodyWheel::update()
{
    if (_object->isActive())
    {
        btTransform transf;
        _vehicle->updateWheelTransform(_index, true);
        transf = _vehicle->getWheelInfo(_index).m_worldTransform;

        _isUpdateTransformFromObject = false;
        _object->setPosition(glm::vec3(transf.getOrigin().getX(),transf.getOrigin().getY(),transf.getOrigin().getZ()));
        _object->setRotationQuaternion(transf.getRotation().getX(),transf.getRotation().getY(),transf.getRotation().getZ(),transf.getRotation().getW());
        _isUpdateTransformFromObject = true;
    }
}
