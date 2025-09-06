// Generated file
#include "PhysicalBodyLuaBindings.h"

#include "../../../Physics/PhysicalBody.hpp"


namespace PhysicalBodyLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<PhysicalBody>("PhysicalBody",
            sol::base_classes, sol::bases<Component>(),

            "getPhysicalBodyType", &PhysicalBody::getPhysicalBodyType,
            "getObjectsCollidesWith", &PhysicalBody::getObjectsCollidesWith,
            "getObjectsBeginCollision", &PhysicalBody::getObjectsBeginCollision,
            "getObjectsEndCollision", &PhysicalBody::getObjectsEndCollision



        );
    }
}
