#include "LuaBindings.h"

#include "ComponentLuaBindings.h"
#include "LoggerLuaBindings.h"
#include "SceneManagerLuaBindings.h"
#include "SceneObjectLuaBindings.h"
#include "ScriptComponentLuaBindings.h"


namespace LuaBindings
{
	void bind(sol::state* lua)
	{
		ComponentLuaBindings::bind(lua);
		LoggerLuaBindings::bind(lua);
		SceneManagerLuaBindings::bind(lua);
		SceneObjectLuaBindings::bind(lua);
		ScriptComponentLuaBindings::bind(lua);
	}
}
