#include "LuaBindings.h"

#include "SceneManagerLuaBindings.h"
#include "SceneObjectLuaBindings.h"


namespace LuaBindings
{
	void bind(sol::state* lua)
	{
		SceneManagerLuaBindings::bind(lua);
		SceneObjectLuaBindings::bind(lua);
	}
}
