// Generated file
#include "BusRaycastLuaBindings.h"

#include "../../../Bus/BusRaycast.h"


namespace BusRaycastLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<BusRaycast>("BusRaycast",
            sol::base_classes, sol::bases<Bus>(),

            "doorOpen", [](BusRaycast* self, int door) { self->doorOpen(door); },
            "doorClose", [](BusRaycast* self, int door) { self->doorClose(door); },
            "doorOpenClose", [](BusRaycast* self, int door) { self->doorOpenClose(door); },
            "doorGroupOpen", [](BusRaycast* self, int groupIndex) { self->doorGroupOpen(groupIndex); },
            "doorGroupClose", [](BusRaycast* self, int groupIndex) { self->doorGroupClose(groupIndex); },
            "doorGroupOpenClose", [](BusRaycast* self, int groupIndex) { self->doorGroupOpenClose(groupIndex); },
            "doorBlock", [](BusRaycast* self, int door) { self->doorBlock(door); },
            "doorUnblock", [](BusRaycast* self, int door) { self->doorUnblock(door); }



        );
    }
}
