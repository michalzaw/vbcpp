#include "GeneratedLuaBindings.h"

#include "PhysicalBodyWheelLuaBindings.h"
#include "RenderObjectLuaBindings.h"
#include "ScriptComponentLuaBindings.h"


namespace GeneratedLuaBindings
{
    void bind(sol::state* lua)
    {
        PhysicalBodyWheelLuaBindings::bind(lua);
        RenderObjectLuaBindings::bind(lua);
        ScriptComponentLuaBindings::bind(lua);
    }
}
