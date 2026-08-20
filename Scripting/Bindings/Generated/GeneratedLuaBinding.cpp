// Generated file
#include "GeneratedLuaBindings.h"

#include "AIAgentLuaBindings.h"
#include "AnimationStateLuaBindings.h"
#include "DisplayComponentLuaBindings.h"
#include "DisplayTextLuaBindings.h"
#include "MaterialLuaBindings.h"
#include "ModelNodeLuaBindings.h"
#include "PhysicalBodyLuaBindings.h"
#include "PhysicalBodyWheelLuaBindings.h"
#include "RenderObjectLuaBindings.h"
#include "ScriptComponentLuaBindings.h"
#include "SkeletalAnimationComponentLuaBindings.h"
#include "SoundComponentLuaBindings.h"
#include "TransformLuaBindings.h"


namespace GeneratedLuaBindings
{
    void bind(sol::state* lua)
    {
        AIAgentLuaBindings::bind(lua);
        AnimationStateLuaBindings::bind(lua);
        DisplayComponentLuaBindings::bind(lua);
        DisplayTextLuaBindings::bind(lua);
        MaterialLuaBindings::bind(lua);
        ModelNodeLuaBindings::bind(lua);
        PhysicalBodyLuaBindings::bind(lua);
        PhysicalBodyWheelLuaBindings::bind(lua);
        RenderObjectLuaBindings::bind(lua);
        ScriptComponentLuaBindings::bind(lua);
        SkeletalAnimationComponentLuaBindings::bind(lua);
        SoundComponentLuaBindings::bind(lua);
        TransformLuaBindings::bind(lua);
    }
}
