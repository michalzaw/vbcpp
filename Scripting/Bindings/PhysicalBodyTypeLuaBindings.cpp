#include "PhysicalBodyTypeLuaBindings.h"

#include "../../Physics/PhysicalBody.hpp"

#include "../Utils/LuaUtils.h"


namespace PhysicalBodyTypeLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_enum("PhysicalBodyType",
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, BOX),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, BVT_TRIANGLE_MESH),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, CONVEX_HULL),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, CYLINDER),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, GHOST),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, RAYCAST_VEHICLE),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, SPHERE),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, STATIC_PLANE),
            LUA_MAP_ENUM_CLASS(PhysicalBodyType, WHEEL)
		);
	}
}
