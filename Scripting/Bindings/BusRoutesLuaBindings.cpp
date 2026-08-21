// Generated file
#include "BusRoutesLuaBindings.h"

#include "../../../Game/Transit/BusRoutes.h"

#include "../Utils/LuaUtils.h"


namespace BusRoutesLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_enum("BusRouteDisplayType",
            LUA_MAP_ENUM(RDT_ONE_LINE),
            LUA_MAP_ENUM(RDT_TWO_LINE),
            LUA_MAP_ENUM(RDT_TWO_LINE_FIRST_BIG),
            LUA_MAP_ENUM(RDT_TWO_LINE_SECOND_BIG)
        );

        lua->new_usertype<BusRoute>("BusRoute",
            "id", &BusRoute::id,
            "direction", &BusRoute::direction,
            "type", &BusRoute::type,
            "head", &BusRoute::head,
            "line1", &BusRoute::line1,
            "line2", &BusRoute::line2
        );
        
        lua->new_usertype<BusLine>("BusLine",
            "id", &BusLine::id,
            "number", &BusLine::number,
            "routes", &BusLine::routes
        );
        
        lua->new_usertype<BusRoutes>("BusRoutes",
            "fileName", &BusRoutes::fileName,
            "description", &BusRoutes::description,
            "lines", &BusRoutes::lines
        );
    }
}
