#include "TransformCompletedEventTypeLuaBindings.h"

#include "../../Game/TransformCompletedEvent.h"

#include "../Utils/LuaUtils.h"


namespace TransformCompletedEventTypeLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_enum("TransformCompletedEventType",
            LUA_MAP_ENUM_CLASS(TransformCompletedEventType, TRANSLATION),
            LUA_MAP_ENUM_CLASS(TransformCompletedEventType, ROTATION),
            LUA_MAP_ENUM_CLASS(TransformCompletedEventType, ROTATION_LOCAL),
            LUA_MAP_ENUM_CLASS(TransformCompletedEventType, SCALE)
		);
	}
}
