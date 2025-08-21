#include "ComponentLuaBindings.h"

#include "../../Scene/SceneObject.h"


#define MAP_ENUM(enumValue) #enumValue, enumValue


namespace ComponentLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<Component>("Component",
			"setSceneObject", &Component::setSceneObject,
			"setIsActive", &Component::setIsActive,
			"getType", &Component::getType,
			"getSceneObject", &Component::getSceneObject,
			"isActive", &Component::isActive
		);

		lua->new_enum("ComponentType",
            MAP_ENUM(CT_RENDER_OBJECT),
            MAP_ENUM(CT_CAMERA),
            MAP_ENUM(CT_LIGHT),
            MAP_ENUM(CT_PHYSICAL_BODY),
            MAP_ENUM(CT_TREE_COMPONENT),
            MAP_ENUM(CT_SOUND),
            MAP_ENUM(CT_GRASS),
            MAP_ENUM(CT_ROAD_OBJECT),
            MAP_ENUM(CT_CROSSROAD),
            MAP_ENUM(CT_ROAD_INTERSECTION),
            MAP_ENUM(CT_TERRAIN),
            MAP_ENUM(CT_SKY),
            MAP_ENUM(CT_ENVIRONMENT_CAPTURE_COMPONENT),
            MAP_ENUM(CT_MIRROR),
            MAP_ENUM(CT_CLICKABLE_OBJECT),
            MAP_ENUM(CT_SHAPE_POLYGON),
            MAP_ENUM(CT_PREFAB),
            MAP_ENUM(CT_SKELETAL_ANIMATION),
            MAP_ENUM(CT_SKELETAL_ANIMATION_2),
            MAP_ENUM(CT_SKELETAL_ANIMATION_HELPER),
            MAP_ENUM(CT_BEZIER_CURVE),

            MAP_ENUM(CT_SCRIPT),

            MAP_ENUM(CT_INTERNAL_HELPER),

            MAP_ENUM(CT_BUS_STOP),
            MAP_ENUM(CT_DISPLAY),
            MAP_ENUM(CT_CAMERA_CONTROL),
            MAP_ENUM(CT_PATH),
            MAP_ENUM(CT_AI_AGENT),
            MAP_ENUM(CT_AI_AGENT_VEHICLE),
            MAP_ENUM(CT_STOP_COMPONENT),
            MAP_ENUM(CT_TRAFFIC_LIGHTS),
            MAP_ENUM(CT_BUS_START_POINT)
		);
	}
}
