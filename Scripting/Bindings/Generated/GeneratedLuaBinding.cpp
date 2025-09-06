// Generated file
#include "GeneratedLuaBindings.h"

#include "MaterialLuaBindings.h"
#include "ModelNodeLuaBindings.h"
#include "PhysicalBodyWheelLuaBindings.h"
#include "RenderObjectLuaBindings.h"
#include "ScriptComponentLuaBindings.h"
#include "SoundComponentLuaBindings.h"
#include "TransformLuaBindings.h"


namespace GeneratedLuaBindings
{
    void bind(sol::state* lua)
    {
        MaterialLuaBindings::bind(lua);
        ModelNodeLuaBindings::bind(lua);
        PhysicalBodyWheelLuaBindings::bind(lua);
        RenderObjectLuaBindings::bind(lua);
        ScriptComponentLuaBindings::bind(lua);
        SoundComponentLuaBindings::bind(lua);
        TransformLuaBindings::bind(lua);
    }
}
