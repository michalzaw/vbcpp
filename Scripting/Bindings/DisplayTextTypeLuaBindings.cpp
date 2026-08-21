#include "DisplayTextTypeLuaBindings.h"

#include "../../Graphics/DisplayText.h"

#include "../Utils/LuaUtils.h"


namespace DisplayTextTypeLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_enum("DisplayTextType",
            LUA_MAP_ENUM(ONLY_HEAD),
            LUA_MAP_ENUM(ONE_LINE),
			LUA_MAP_ENUM(TWO_LINE),
            LUA_MAP_ENUM(TWO_LINE_FIRST_BIG),
            LUA_MAP_ENUM(TWO_LINE_SECOND_BIG)
		);
	}
}
