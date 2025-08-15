#include "ComponentLuaBindings.h"

#include "../../Scene/SceneObject.h"


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
	}
}
