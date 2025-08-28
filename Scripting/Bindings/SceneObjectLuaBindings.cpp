#include "SceneObjectLuaBindings.h"

#include "../../Scene/SceneObject.h"

#include "../../Graphics/RenderObject.h"

#include "Generated/ComponentsGetters.h"


namespace SceneObjectLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<SceneObject>("SceneObject",
			COMPONENTS_GETTERS,
			"setName", &SceneObject::setName,
			"setIsActive", &SceneObject::setIsActive,
			"getName", &SceneObject::getName,
			"getId", &SceneObject::getId,
			"isActive", &SceneObject::isActive,
			"setFlags", &SceneObject::setFlags,
			"addFlag", &SceneObject::addFlag,
			"getFlags", &SceneObject::getFlags,
			"getComponent", sol::resolve<Component*(unsigned int)>(&SceneObject::getComponent),
			"getComponentByType", sol::resolve<Component*(ComponentType)>(&SceneObject::getComponent),
			"getComponentsCount", &SceneObject::getComponentsCount,
			"setPosition", sol::overload(
				sol::resolve<void(const glm::vec3&)>(&SceneObject::setPosition),
				sol::resolve<void(float, float, float)>(&SceneObject::setPosition)),
			"setRotation", sol::overload(
				sol::resolve<void(const glm::vec3&)>(&SceneObject::setRotation),
				sol::resolve<void(float, float, float)>(&SceneObject::setRotation)),
			"setScale", sol::overload(sol::resolve<void(const glm::vec3&)>(&SceneObject::setScale),
				sol::resolve<void(float, float, float)>(&SceneObject::setScale),
				sol::resolve<void(float)>(&SceneObject::setScale)),
			"move", sol::overload(
				sol::resolve<void(const glm::vec3&)>(&SceneObject::move),
				sol::resolve<void(float, float, float)>(&SceneObject::move)),
			"rotate", sol::overload(
				sol::resolve<void(const glm::vec3&)>(&SceneObject::rotate),
				sol::resolve<void(float, float, float)>(&SceneObject::rotate)),
			"scale", sol::overload(
				sol::resolve<void(float, float, float)>(&SceneObject::scale),
				sol::resolve<void(const glm::vec3&)>(&SceneObject::scale)),
			"getPosition", &SceneObject::getPosition,
			"getRotation", &SceneObject::getRotation,
			"getScale", &SceneObject::getScale
		);
	}
}
