#include "ScriptComponentLuaBindings.h"

#include "../ScriptComponent.h"


namespace ScriptComponentLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<ScriptComponent>("ScriptComponent",
			sol::base_classes, sol::bases<Component>()
		);
	}
}
