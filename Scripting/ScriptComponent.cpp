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
		validateFunctionResult(result);
	}
}


void ScriptComponent::validateFunctionResult(sol::protected_function_result& result)
{
	if (!result.valid())
	{
		sol::error error = result;
		LOG_ERROR("LUA Script error [" + _scriptFile->getPath() + "][" + sol::to_string(result.status()) + "]: " + error.what());
	}
}


void ScriptComponent::onAttachedToScenObject()
{
	if (!_isInitialized)
	{
		LOG_DEBUG("Script is not initialized. Skip setupScriptEnvironment and init callback.");
	}

	setupScriptEnvironment();

	if (_objectClickedFunction.valid())
	{
		_objectClickedSubscription = getSceneObject()->getEvents().subscribe<ObjectClickedEvent>(&ScriptComponent::onObjectClicked, this);
	}

	if (_initFunction.valid())
	{
		sol::protected_function_result result = _initFunction();
		validateFunctionResult(result);
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

	// callback
	_objectClickedFunction = _scriptEnvironment["onObjectClicked"];
}


void ScriptComponent::onObjectClicked(const ObjectClickedEvent& event)
{
	if (_objectClickedFunction.valid())
	{
		sol::protected_function_result result = _objectClickedFunction();
		validateFunctionResult(result);
	}
}


void ScriptComponent::exportFunction(const std::string& name, sol::function function)
{
	_customFunctions[name] = function;
}


sol::function ScriptComponent::getFunction(const std::string& name)
{
	auto result = _customFunctions.find(name);
	if (result != _customFunctions.end())
	{
		return result->second;
	}
	else
	{
		LOG_ERROR("LUA Script error [" + _scriptFile->getPath() + "] Cannot find function with name: " + name);
		return sol::nil;
	}
}


void ScriptComponent::loadScript(bool setUpEnvironmentAndCallInitCallback/* = true*/)
{
	sol::load_result loadResult = _luaState->load(_scriptFile->getScript());
	if (loadResult.valid())
	{
		sol::protected_function loadedScript = loadResult;
		sol::set_environment(_scriptEnvironment, loadedScript);

		sol::protected_function_result result = loadedScript.call();
		validateFunctionResult(result);
	}
	else
	{
		sol::error error = loadResult;
		LOG_ERROR("LUA Script error [" + _scriptFile->getPath() + "][" + sol::to_string(loadResult.status()) + "]: " + error.what());
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
		validateFunctionResult(result);
	}
}


void ScriptComponent::update(float deltaTime)
{
	if (_updateFunction.valid())
	{
		sol::protected_function_result result = _updateFunction(deltaTime);
		validateFunctionResult(result);
	}
}
