#include "LuaBindings.h"

#include "ComponentLuaBindings.h"
#include "InputSystemLuaBindings.h"
#include "LoggerLuaBindings.h"
#include "SceneManagerLuaBindings.h"
#include "SceneObjectLuaBindings.h"
#include "ScriptComponentLuaBindings.h"


namespace LuaBindings
{
	void bind(sol::state* lua)
	{
		ComponentLuaBindings::bind(lua);
		InputSystemLuaBindings::bind(lua);
		LoggerLuaBindings::bind(lua);
		SceneManagerLuaBindings::bind(lua);
		SceneObjectLuaBindings::bind(lua);
		ScriptComponentLuaBindings::bind(lua);
	}
}
