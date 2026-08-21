// Generated file
#include "GeneratedLuaBindings.h"

#include "AIAgentLuaBindings.h"
#include "AnimationStateLuaBindings.h"
#include "BusStopComponentLuaBindings.h"
#include "CurrentRouteBusStopsStatsDataLuaBindings.h"
#include "CurrentRouteDataLuaBindings.h"
#include "DisplayComponentLuaBindings.h"
#include "DisplayTextLuaBindings.h"
#include "GameClockLuaBindings.h"
#include "GameLogicSystemLuaBindings.h"
#include "MaterialLuaBindings.h"
#include "ModelNodeLuaBindings.h"
#include "PhysicalBodyLuaBindings.h"
#include "PhysicalBodyWheelLuaBindings.h"
#include "RenderObjectLuaBindings.h"
#include "ScriptComponentLuaBindings.h"
#include "SkeletalAnimationComponentLuaBindings.h"
#include "SoundComponentLuaBindings.h"
#include "TimeLuaBindings.h"
#include "TransformLuaBindings.h"
#include "TransitSystemLuaBindings.h"


namespace GeneratedLuaBindings
{
    void bind(sol::state* lua)
    {
        AIAgentLuaBindings::bind(lua);
        AnimationStateLuaBindings::bind(lua);
        BusStopComponentLuaBindings::bind(lua);
        CurrentRouteBusStopsStatsDataLuaBindings::bind(lua);
        CurrentRouteDataLuaBindings::bind(lua);
        DisplayComponentLuaBindings::bind(lua);
        DisplayTextLuaBindings::bind(lua);
        GameClockLuaBindings::bind(lua);
        GameLogicSystemLuaBindings::bind(lua);
        MaterialLuaBindings::bind(lua);
        ModelNodeLuaBindings::bind(lua);
        PhysicalBodyLuaBindings::bind(lua);
        PhysicalBodyWheelLuaBindings::bind(lua);
        RenderObjectLuaBindings::bind(lua);
        ScriptComponentLuaBindings::bind(lua);
        SkeletalAnimationComponentLuaBindings::bind(lua);
        SoundComponentLuaBindings::bind(lua);
        TimeLuaBindings::bind(lua);
        TransformLuaBindings::bind(lua);
        TransitSystemLuaBindings::bind(lua);
    }
}
