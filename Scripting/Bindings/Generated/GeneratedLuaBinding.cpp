#include "GeneratedLuaBindings.h"

#include "PhysicalBodyWheelLuaBindings.h"
#include "RenderObjectLuaBindings.h"


namespace GeneratedLuaBindings
{
    void bind(sol::state* lua)
    {
        PhysicalBodyWheelLuaBindings::bind(lua);
        RenderObjectLuaBindings::bind(lua);
    }
}
