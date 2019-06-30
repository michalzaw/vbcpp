#include "DoorClassic.h"


DoorClassic::DoorClassic(RStaticModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hingeBusToArm, ConstraintHinge* hingeArmToDoor,
            SoundComponent* openDoor, SoundComponent* closeDoor, RotationDir rotationDir, char group)
    : Door(model, body, openDoor, closeDoor, group), _hingeBusToArm(hingeBusToArm), _hingeArmToDoor(hingeArmToDoor), _rotationDir(rotationDir)
{
}


DoorClassic::~DoorClassic()
{
}


ConstraintHinge* DoorClassic::getConstraintBusToArm()
{
    return _hingeBusToArm;
}


ConstraintHinge* DoorClassic::getConstraintArmToDoor()
{
    return _hingeArmToDoor;
}


void DoorClassic::open()
{
    int direction;
    if (_rotationDir == ERD_CW)
        direction = 1;
    else if (_rotationDir == ERD_CCW)
        direction = -1;

    _hingeBusToArm->getBulletConstraint()->enableAngularMotor(true, 2.1f * direction, 0.15f);
    _hingeArmToDoor->getBulletConstraint()->enableAngularMotor(true, -4.2f * direction, 0.3f);
    _state = EDS_OPENING;

    Door::open();
}


void DoorClassic::close()
{
    int direction;
    if (_rotationDir == ERD_CW)
        direction = 1;
    else if (_rotationDir == ERD_CCW)
        direction = -1;

    _hingeBusToArm->getBulletConstraint()->enableAngularMotor(true, -2.1f * direction, 0.15f);
    _hingeArmToDoor->getBulletConstraint()->enableAngularMotor(true, 4.2f * direction, 0.3f);
    _state = EDS_CLOSING;

    Door::close();
}


void DoorClassic::setLoose()
{
    _hingeBusToArm->getBulletConstraint()->enableAngularMotor(false, 0.0f, 0.0f);
    _hingeArmToDoor->getBulletConstraint()->enableAngularMotor(false, 0.0f, 0.0f);
}
