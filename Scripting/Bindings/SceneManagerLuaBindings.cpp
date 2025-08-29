#include "SceneManagerLuaBindings.h"

#include "../../Scene/SceneManager.h"


namespace SceneManagerLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<SceneManager>("SceneManager",
			"getSceneObject", sol::resolve<SceneObject* (const std::string&)>(&SceneManager::getSceneObject)
		);
	}
}
