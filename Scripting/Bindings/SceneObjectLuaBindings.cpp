#include "SceneObjectLuaBindings.h"

#include "../../Scene/SceneObject.h"


namespace SceneObjectLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<SceneObject>("SceneObject",
			"getName", &SceneObject::getName,
			"setPosition", sol::resolve<void(float, float, float)>(&SceneObject::setPosition),
			"setRotation", sol::resolve<void(float, float, float)>(&SceneObject::setRotation),
			"setScale", sol::resolve<void(float, float, float)>(&SceneObject::setScale),
			"move", sol::resolve<void(float, float, float)>(&SceneObject::move),
			"rotate", sol::resolve<void(float, float, float)>(&SceneObject::rotate),
			"scale", sol::resolve<void(float, float, float)>(&SceneObject::scale),
			"getPosition", &SceneObject::getPosition,
			"getRotation", &SceneObject::getRotation,
			"getScale", &SceneObject::getScale,
			"getLocalTransformMatrix", &SceneObject::getLocalTransformMatrix
		);
	}
}
