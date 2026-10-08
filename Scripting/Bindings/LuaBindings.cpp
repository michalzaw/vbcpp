#include "LuaBindings.h"

#include "BusRoutesLuaBindings.h"
#include "ComponentLuaBindings.h"
#include "DisplayTextTypeLuaBindings.h"
#include "GlmLuaBindings.h"
#include "InputSystemLuaBindings.h"
#include "LoggerLuaBindings.h"
#include "PhysicalBodyTypeLuaBindings.h"
#include "SceneManagerLuaBindings.h"
#include "SceneObjectLuaBindings.h"
#include "ScheduleLuaBindings.h"
#include "ScriptComponentLuaBindingsExtension.h"
#include "TransformCompletedEventTypeLuaBindings.h"


namespace LuaBindings
{
	void bind(sol::state* lua)
	{
		BusRoutesLuaBindings::bind(lua);
		ComponentLuaBindings::bind(lua);
		DisplayTextTypeLuaBindings::bind(lua);
		GlmLuaBindings::bind(lua);
		InputSystemLuaBindings::bind(lua);
		LoggerLuaBindings::bind(lua);
		PhysicalBodyTypeLuaBindings::bind(lua);
		SceneManagerLuaBindings::bind(lua);
		SceneObjectLuaBindings::bind(lua);
		ScheduleLuaBindings::bind(lua);
		ScriptComponentLuaBindingsExtension::bind(lua);
		TransformCompletedEventTypeLuaBindings::bind(lua);
	}
}
