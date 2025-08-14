#include "ScriptComponent.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"


ScriptComponent::ScriptComponent(RScriptFile* scriptFile, sol::state* luaState)
	: Component(CT_SCRIPT),
	_luaState(luaState),
	_scriptFile(scriptFile)
{
	// todo: _luaState->load()
	_luaState->script(_scriptFile->getScript());
}


void ScriptComponent::update(float deltaTime)
{
	(*_luaState)["sceneManager"] = getSceneObject()->getSceneManager();

	sol::function updateFunction = (*_luaState)["update"];
	if (updateFunction.valid())
	{
		updateFunction(deltaTime);
	}

}
