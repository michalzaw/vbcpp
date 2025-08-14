#include "ScriptingManager.h"


ScriptingManager::ScriptingManager()
{
	_luaState = new sol::state; // todo: add panic handler
	_luaState->open_libraries(sol::lib::base);
	// todo: lua->set_exception_handler(&my_exception_handler);
}


ScriptComponent* ScriptingManager::addScript(const std::string& fileName)
{
	ScriptComponent* script = new ScriptComponent(fileName, _luaState);

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
