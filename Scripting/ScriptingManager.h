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

	public:
		ScriptingManager();

		ScriptComponent* addScript(const std::string& fileName);

		void removeScript(ScriptComponent* script);

		void update(float deltaTime);

};


#endif // SCRIPTINGMANAGER_H_INCLUDED
