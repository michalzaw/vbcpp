#include "PhysicalBodyWheelLuaBindings.h"

#include "../../../Physics/PhysicalBodyWheel.h"


namespace PhysicalBodyWheelLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<PhysicalBodyWheel>("PhysicalBodyWheel",
            sol::base_classes, sol::bases<PhysicalBody>(),

            "getSteeringValue", &PhysicalBodyWheel::getSteeringValue,
            "setSteeringValue", &PhysicalBodyWheel::setSteeringValue,
            "setBrake", &PhysicalBodyWheel::setBrake,
            "applyEngineForce", &PhysicalBodyWheel::applyEngineForce,
            "getWheelInfo", &PhysicalBodyWheel::getWheelInfo


        );
    }
}
