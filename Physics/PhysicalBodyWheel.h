#ifndef PHYSICALBODYWHEEL_H_INCLUDED
#define PHYSICALBODYWHEEL_H_INCLUDED


#include "PhysicalBody.hpp"

#include "../Scripting/Utils/LuaMacros.h"


class PhysicalBodyWheel : public PhysicalBody
{
    VBCPP_COMPONENT(PhysicalBodyWheel, CT_PHYSICAL_BODY)

    public:
        PhysicalBodyWheel(btRaycastVehicle* vehicle, int index);
        virtual ~PhysicalBodyWheel();

        LUAF float getSteeringValue();
        LUAF void setSteeringValue(float angle);

        LUAF void setBrake(float brake)
        {
            _vehicle->setBrake(brake, _index);
        }

        LUAF void applyEngineForce(float force)
        {
            _vehicle->applyEngineForce(force, _index);
        }

        LUAF btWheelInfo& getWheelInfo();

    private:
        btRaycastVehicle* _vehicle;
        int _index;

        float _steeringValue;

        virtual void update();
        virtual void changedTransform()
        {

        }

};


#endif // PHYSICALBODYWHEEL_H_INCLUDED
