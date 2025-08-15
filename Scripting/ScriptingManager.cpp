#include "ScriptingManager.h"

#include "Bindings/LuaBindings.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"


ScriptingManager::ScriptingManager()
	: _enabled(true)
{
	_luaState = new sol::state; // todo: add panic handler
	_luaState->open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);
	// todo: lua->set_exception_handler(&my_exception_handler);


	LuaBindings::bind(_luaState);
}


ScriptingManager::~ScriptingManager()
{
	for (std::list<ScriptComponent*>::iterator i = _scripts.begin(); i != _scripts.end(); ++i)
	{
		delete* i;
	}

	_scripts.clear();
	_unloadedScripts.clear();
}


void ScriptingManager::setEnabled(bool enabled)
{
	_enabled = enabled;

	if (enabled)
	{
		for (ScriptComponent* script : _unloadedScripts)
		{
			script->loadScript();
		}

		_unloadedScripts.clear();
	}
}


bool ScriptingManager::isEnabled()
{
	return _enabled;
}


ScriptComponent* ScriptingManager::addScript(RScriptFile* scriptFile)
{
	ScriptComponent* script = new ScriptComponent(scriptFile, _luaState, _enabled);

	_scripts.push_back(script);

	if (!_enabled)
	{
		_unloadedScripts.push_back(script);
	}

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
	if (_enabled)
	{
		for (ScriptComponent* script : _scripts)
		{
			script->reloadScriptFromResource();
		}
	}
	else
	{
		LOG_INFO("Scripting system are disabled. Scripts are not reloaded.");
	}
}


void ScriptingManager::update(float deltaTime)
{
	if (_enabled)
	{
		for (ScriptComponent* script : _scripts)
		{
			if (script->isActive())
			{
				script->update(deltaTime);
			}
		}
	}
}
