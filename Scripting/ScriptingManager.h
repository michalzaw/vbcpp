#ifndef SCRIPTINGMANAGER_H_INCLUDED
#define SCRIPTINGMANAGER_H_INCLUDED


#include <list>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "ScriptComponent.h"


class ScriptingManager final
{
	private:
		sol::state* _luaState;

		std::list<ScriptComponent*> _scripts;
		std::list<ScriptComponent*> _unloadedScripts;

		bool _enabled;

	public:
		ScriptingManager();
		~ScriptingManager();

		void setEnabled(bool enabled);
		bool isEnabled();

		ScriptComponent* addScript(RScriptFile* scriptFile);

		void removeScript(ScriptComponent* script);

		void reloadAllScripts();

		void update(float deltaTime);

		template<typename TYPE>
		void setGlobalVariable(const std::string& name, const TYPE& value);

};


template<typename TYPE>
void ScriptingManager::setGlobalVariable(const std::string& name, const TYPE& value)
{
	(*_luaState)[name] = value;
}


#endif // SCRIPTINGMANAGER_H_INCLUDED
