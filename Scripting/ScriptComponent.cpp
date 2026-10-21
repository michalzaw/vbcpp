#include "ScriptComponent.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/ResourceManager.h"


ScriptComponent::ScriptComponent(RScriptFile* scriptFile, sol::state* luaState, bool loadAfterCreate/* = true*/)
	: Component(CT_SCRIPT),
	_luaState(luaState),
	_scriptEnvironment(*luaState, sol::create, luaState->globals()),
	_scriptFile(scriptFile),
	_isInitialized(false)
{
	_customMembers = luaState->create_table();

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


void ScriptComponent::loadScript(RScriptFile* scriptFile)
{
	sol::load_result loadResult = _luaState->load(scriptFile->getScript());
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
		LOG_ERROR("LUA Script error [" + scriptFile->getPath() + "][" + sol::to_string(loadResult.status()) + "]: " + error.what());
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

	if (_objectLoadedFunction.valid())
	{
		_objectLoadedSubscription = getSceneObject()->getEvents().subscribe<ObjectLoadedEvent>(&ScriptComponent::onObjectLoaded, this);
	}

	if (_transformCompletedFunction.valid())
	{
		_transformCompletedSubscription = getSceneObject()->getEvents().subscribe<TransformCompletedEvent>(&ScriptComponent::onTransformCompleted, this);
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

	// events
	_objectClickedFunction = _scriptEnvironment["onObjectClicked"];
	_objectLoadedFunction = _scriptEnvironment["onObjectLoaded"];
	_transformCompletedFunction = _scriptEnvironment["onTransformCompleted"];

	// coroutines api
	_scriptEnvironment.set_function("runCoroutine", [this](sol::function function, sol::variadic_args args) { runCoroutine(function, args); });
}


void ScriptComponent::runCoroutine(sol::function function, sol::variadic_args args)
{
	sol::thread thread = sol::thread::create(_scriptEnvironment.lua_state());
	sol::coroutine coroutine = sol::coroutine(thread.state(), function);

	sol::protected_function_result result = coroutine(args);
	validateFunctionResult(result);

	if (coroutine.status() == sol::call_status::yielded)
	{
		float timeToResume = result;

		_coroutines.push_back({ thread, coroutine, timeToResume });
	}
}


void ScriptComponent::updateCoroutines(float deltaTime)
{
	for (std::list<LuaCoroutine>::iterator i = _coroutines.begin(); i != _coroutines.end();)
	{
		sol::coroutine& coroutine = i->coroutine;

		i->timeToResume -= deltaTime;

		if (i->timeToResume <= 0.0f)
		{
			sol::protected_function_result result = coroutine();
			validateFunctionResult(result);

			if (coroutine.status() == sol::call_status::ok)
			{
				i = _coroutines.erase(i);
			}
			else
			{
				if (coroutine.status() == sol::call_status::yielded)
				{
					float timeToResume = result;
					i->timeToResume = timeToResume;
				}

				++i;
			}
		}
		else
		{
			++i;
		}
	}
}


void ScriptComponent::onObjectClicked(const ObjectClickedEvent& event)
{
	if (_objectClickedFunction.valid())
	{
		sol::protected_function_result result = _objectClickedFunction(event);
		validateFunctionResult(result);
	}
}


void ScriptComponent::onObjectLoaded(const ObjectLoadedEvent& event)
{
	if (_objectLoadedFunction.valid())
	{
		sol::protected_function_result result = _objectLoadedFunction(event);
		validateFunctionResult(result);
	}
}


void ScriptComponent::onTransformCompleted(const TransformCompletedEvent& event)
{
	if (_transformCompletedFunction.valid())
	{
		sol::protected_function_result result = _transformCompletedFunction(event);
		validateFunctionResult(result);
	}
}


void ScriptComponent::addCustomMember(sol::object key, sol::object value)
{
	_customMembers[key] = value;
}


sol::object ScriptComponent::getCustomMember(sol::object key)
{
	return _customMembers[key];
}


void ScriptComponent::loadScript(bool setUpEnvironmentAndCallInitCallback/* = true*/)
{
	_scriptEnvironment.set_function("include", [this](const std::string& fileName) {
			std::string scriptPath = FilesHelper::getPathToDirectoryFromFileName(_scriptFile->getPath());

			RScriptFile* includedScript = ResourceManager::getInstance().loadScriptFile(scriptPath + fileName);

			loadScript(includedScript);
		});

	loadScript(_scriptFile);

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

	updateCoroutines(deltaTime);
}
