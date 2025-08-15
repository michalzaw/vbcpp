#include "ScriptComponent.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"


ScriptComponent::ScriptComponent(RScriptFile* scriptFile, sol::state* luaState)
	: Component(CT_SCRIPT),
	_luaState(luaState),
	_scriptEnvironment(*luaState, sol::create, luaState->globals()),
	_scriptFile(scriptFile)
{
	// todo: _luaState->load()

	sol::protected_function_result result = _luaState->script(_scriptFile->getScript(), _scriptEnvironment);
	if (!result.valid())
	{
		sol::error error = result;
		LOG_ERROR(error.what());
	}
}


void ScriptComponent::update(float deltaTime)
{
	_scriptEnvironment["sceneManager"] = getSceneObject()->getSceneManager();

	sol::function updateFunction = _scriptEnvironment["update"];
	if (updateFunction.valid())
	{
		sol::protected_function_result result = updateFunction(deltaTime);
		if (!result.valid())
		{
			sol::error error = result;
			LOG_ERROR(error.what());
		}
	}

}
