#ifndef SCRIPTCOMPONENT_H_INCLUDED
#define SCRIPTCOMPONENT_H_INCLUDED


#include "../Scene/Component.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


class ScriptComponent final : public Component
{
	private:
		sol::state* _luaState;

	public:
		ScriptComponent(const std::string& fileName, sol::state* luaState);

		void update(float deltaTime) override;

};


#endif // SCRIPTCOMPONENT_H_INCLUDED
