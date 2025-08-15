#ifndef SCRIPTCOMPONENT_H_INCLUDED
#define SCRIPTCOMPONENT_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "RScriptFile.h"

#include "../Scene/Component.h"


class ScriptComponent final : public Component
{
	private:
		sol::state* _luaState;

		sol::environment _scriptEnvironment;

		RScriptFile* _scriptFile;

	public:
		ScriptComponent(RScriptFile* scriptFile, sol::state* luaState);

		void update(float deltaTime) override;

};


#endif // SCRIPTCOMPONENT_H_INCLUDED
