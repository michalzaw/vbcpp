#include "SceneManagerLuaBindings.h"

#include "../../Scene/SceneManager.h"

#include "../../Utils/ResourceManager.h"


namespace SceneManagerLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<SceneManager>("SceneManager",
			"getSceneObject", sol::resolve<SceneObject* (const std::string&)>(&SceneManager::getSceneObject),
			"addNewObject", [](SceneManager& sceneManager, const std::string& objectName, const glm::vec3& position) {
				RObject* objectDefinition = ResourceManager::getInstance().loadRObject(objectName);
				SceneObject* newObject = RObjectLoader::createSceneObjectFromRObject(objectDefinition, objectName, position, glm::vec3(0.0f, 0.0f, 0.0f), &sceneManager);

				return newObject;
			}
		);
	}
}
