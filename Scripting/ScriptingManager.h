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

};


#endif // SCRIPTINGMANAGER_H_INCLUDED
