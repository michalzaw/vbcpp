#include "ScriptingManager.h"

#include "Bindings/LuaBindings.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"


ScriptingManager::ScriptingManager()
{
	_luaState = new sol::state; // todo: add panic handler
	_luaState->open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);
	// todo: lua->set_exception_handler(&my_exception_handler);


	LuaBindings::bind(_luaState);
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


void ScriptingManager::reloadAllScripts()
{
	for (ScriptComponent* script : _scripts)
	{
		script->reloadScriptFromResource();
	}
}


void ScriptingManager::update(float deltaTime)
{
	for (ScriptComponent* script : _scripts)
	{
		script->update(deltaTime);
	}
}
