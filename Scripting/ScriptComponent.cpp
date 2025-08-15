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


void ScriptComponent::onAttachedToScenObject()
{
	setupScriptEnvironment();

	if (_initFunction.valid())
	{
		sol::protected_function_result result = _initFunction();
		if (!result.valid())
		{
			sol::error error = result;
			LOG_ERROR(error.what());
		}
	}
}


void ScriptComponent::setupScriptEnvironment()
{
	// global varibles
	_scriptEnvironment["sceneManager"] = getSceneObject()->getSceneManager();

	// this/self
	_scriptEnvironment["self"] = this;

	// callbacks
	_initFunction = _scriptEnvironment["onInit"];
	_changeTransformFunction = _scriptEnvironment["onChangeTransform"];
	_updateFunction = _scriptEnvironment["onUpdate"];
}


void ScriptComponent::changedTransform()
{
	if (_changeTransformFunction.valid())
	{
		sol::protected_function_result result = _changeTransformFunction();
		if (!result.valid())
		{
			sol::error error = result;
			LOG_ERROR(error.what());
		}
	}
}


void ScriptComponent::update(float deltaTime)
{
	if (_updateFunction.valid())
	{
		sol::protected_function_result result = _updateFunction(deltaTime);
		if (!result.valid())
		{
			sol::error error = result;
			LOG_ERROR(error.what());
		}
	}
}
