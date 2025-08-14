#include "ScriptComponent.h"


ScriptComponent::ScriptComponent(const std::string& fileName, sol::state* luaState)
	: Component(CT_SCRIPT),
	_luaState(luaState)
{
	_luaState->script_file(fileName);
}


void ScriptComponent::update(float deltaTime)
{

}
