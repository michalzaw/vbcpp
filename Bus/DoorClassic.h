#ifndef DOORCLASSIC_H_INCLUDED
#define DOORCLASSIC_H_INCLUDED


#include "Door.h"

#include "../Physics/ConstraintHinge.hpp"


class DoorClassic : public Door
{
    public:
        DoorClassic(RStaticModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hingeBusToArm, ConstraintHinge* hingeArmToDoor,
                    SoundComponent* openDoor, SoundComponent* closeDoor, RotationDir rotationDir = ERD_CW, char group = 1);

        virtual ~DoorClassic();

        ConstraintHinge* getConstraintBusToArm();
        ConstraintHinge* getConstraintArmToDoor();

        void open() override;
        void close() override;
        void setLoose() override;

    protected:
        ConstraintHinge*        _hingeBusToArm;
        ConstraintHinge*        _hingeArmToDoor;
        RotationDir             _rotationDir;
};


#endif // DOORCLASSIC_H_INCLUDED
