#include "ScriptComponent.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Utils/InputSystem.h"


ScriptComponent::ScriptComponent(RScriptFile* scriptFile, sol::state* luaState, bool loadAfterCreate/* = true*/)
	: Component(CT_SCRIPT),
	_luaState(luaState),
	_scriptEnvironment(*luaState, sol::create, luaState->globals()),
	_scriptFile(scriptFile),
	_isInitialized(false)
{
	if (loadAfterCreate)
	{
		loadScript(false);
	}
}


ScriptComponent::~ScriptComponent()
{
	if (_destroyFunction.valid())
	{
		sol::protected_function_result result = _destroyFunction();
		if (!result.valid())
		{
			sol::error error = result;
			LOG_ERROR(error.what());
		}
	}
}


void ScriptComponent::onAttachedToScenObject()
{
	if (!_isInitialized)
	{
		LOG_DEBUG("Script is not initialized. Skip setupScriptEnvironment and init callback.");
	}

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
	// this/self
	_scriptEnvironment["self"] = this;

	// callbacks
	_initFunction = _scriptEnvironment["onInit"];
	_changeTransformFunction = _scriptEnvironment["onChangeTransform"];
	_updateFunction = _scriptEnvironment["onUpdate"];
	_destroyFunction = _scriptEnvironment["onDestroy"];
}


void ScriptComponent::loadScript(bool setUpEnvironmentAndCallInitCallback/* = true*/)
{
	// todo: _luaState->load()

	sol::protected_function_result result = _luaState->script(_scriptFile->getScript(), _scriptEnvironment);
	if (!result.valid())
	{
		sol::error error = result;
		LOG_ERROR(error.what());
	}

	_isInitialized = true;

	if (setUpEnvironmentAndCallInitCallback)
	{
		onAttachedToScenObject();
	}
}


void ScriptComponent::reloadScriptFromResource()
{
	loadScript();
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
