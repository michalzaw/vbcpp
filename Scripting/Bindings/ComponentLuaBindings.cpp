#include "ComponentLuaBindings.h"

#include "../Utils/LuaUtils.h"

#include "../../Scene/SceneObject.h"


namespace ComponentLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<Component>("Component",
			"setIsActive", &Component::setIsActive,
			"getType", &Component::getType,
			"getSceneObject", &Component::getSceneObject,
			"isActive", &Component::isActive
		);

		lua->new_enum("ComponentType",
            LUA_MAP_ENUM(CT_RENDER_OBJECT),
            LUA_MAP_ENUM(CT_CAMERA),
            LUA_MAP_ENUM(CT_LIGHT),
            LUA_MAP_ENUM(CT_PHYSICAL_BODY),
            LUA_MAP_ENUM(CT_TREE_COMPONENT),
            LUA_MAP_ENUM(CT_SOUND),
            LUA_MAP_ENUM(CT_GRASS),
            LUA_MAP_ENUM(CT_ROAD_OBJECT),
            LUA_MAP_ENUM(CT_CROSSROAD),
            LUA_MAP_ENUM(CT_ROAD_INTERSECTION),
            LUA_MAP_ENUM(CT_TERRAIN),
            LUA_MAP_ENUM(CT_SKY),
            LUA_MAP_ENUM(CT_ENVIRONMENT_CAPTURE_COMPONENT),
            LUA_MAP_ENUM(CT_MIRROR),
            LUA_MAP_ENUM(CT_CLICKABLE_OBJECT),
            LUA_MAP_ENUM(CT_SHAPE_POLYGON),
            LUA_MAP_ENUM(CT_PREFAB),
            LUA_MAP_ENUM(CT_SKELETAL_ANIMATION),
            LUA_MAP_ENUM(CT_SKELETAL_ANIMATION_2),
            LUA_MAP_ENUM(CT_SKELETAL_ANIMATION_HELPER),
            LUA_MAP_ENUM(CT_BEZIER_CURVE),

            LUA_MAP_ENUM(CT_SCRIPT),

            LUA_MAP_ENUM(CT_INTERNAL_HELPER),

            LUA_MAP_ENUM(CT_BUS_STOP),
            LUA_MAP_ENUM(CT_DISPLAY),
            LUA_MAP_ENUM(CT_CAMERA_CONTROL),
            LUA_MAP_ENUM(CT_PATH),
            LUA_MAP_ENUM(CT_AI_AGENT),
            LUA_MAP_ENUM(CT_AI_AGENT_VEHICLE),
            LUA_MAP_ENUM(CT_STOP_COMPONENT),
            LUA_MAP_ENUM(CT_TRAFFIC_LIGHTS),
            LUA_MAP_ENUM(CT_BUS_START_POINT)
		);
	}
}
