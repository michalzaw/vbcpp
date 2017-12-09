#include "PhysicalBodyWheel.h"


PhysicalBodyWheel::PhysicalBodyWheel(btRaycastVehicle* vehicle, int index)
    : PhysicalBody(0), _vehicle(vehicle), _index(index)
{
    _steeringValue = _vehicle->getSteeringValue(_index);
}

PhysicalBodyWheel::~PhysicalBodyWheel()
{

}


float PhysicalBodyWheel::getSteeringValue()
{
    return _steeringValue;
}


void PhysicalBodyWheel::setSteeringValue(float angle)
{
    _steeringValue = angle;

    _vehicle->setSteeringValue(_steeringValue, _index);
}


btWheelInfo& PhysicalBodyWheel::getWheelInfo()
{
    return _vehicle->getWheelInfo(_index);
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
