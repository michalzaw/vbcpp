// Generated file
#include "GearboxLuaBindings.h"

#include "../../../Utils/Gearbox.h"


namespace GearboxLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Gearbox>("Gearbox",

            "currentGear", &Gearbox::currentGear,
            "currentRatio", &Gearbox::currentRatio,
            "shiftUp", &Gearbox::shiftUp,
            "shiftDown", &Gearbox::shiftDown,
            "setNeutral", &Gearbox::setNeutral



        );
    }
}
