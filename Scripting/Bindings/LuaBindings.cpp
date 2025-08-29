#include "LuaBindings.h"

#include "ComponentLuaBindings.h"
#include "GlmLuaBindings.h"
#include "InputSystemLuaBindings.h"
#include "LoggerLuaBindings.h"
#include "SceneManagerLuaBindings.h"
#include "SceneObjectLuaBindings.h"


namespace LuaBindings
{
	void bind(sol::state* lua)
	{
		ComponentLuaBindings::bind(lua);
		GlmLuaBindings::bind(lua);
		InputSystemLuaBindings::bind(lua);
		LoggerLuaBindings::bind(lua);
		SceneManagerLuaBindings::bind(lua);
		SceneObjectLuaBindings::bind(lua);
	}
}
