#include "ScriptingManager.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"


ScriptingManager::ScriptingManager()
{
	_luaState = new sol::state; // todo: add panic handler
	_luaState->open_libraries(sol::lib::base);
	// todo: lua->set_exception_handler(&my_exception_handler);


	_luaState->new_usertype<SceneObject>("SceneObject",
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

	_luaState->new_usertype<SceneManager>("SceneManager",
		"getSceneObject", sol::resolve<SceneObject* (const std::string&)>(&SceneManager::getSceneObject)
		);
}


ScriptComponent* ScriptingManager::addScript(RScriptFile* scriptFile)
{
	ScriptComponent* script = new ScriptComponent(scriptFile, _luaState);

	_scripts.push_back(script);

	return script;
}


void ScriptingManager::removeScript(ScriptComponent* script)
{
	for (std::list<ScriptComponent*>::iterator i = _scripts.begin(); i != _scripts.end(); ++i)
	{
		if (*i == script)
		{
			i = _scripts.erase(i);

			delete script;

			return;
		}
	}
}


void ScriptingManager::update(float deltaTime)
{
	for (ScriptComponent* script : _scripts)
	{
		script->update(deltaTime);
	}
}
