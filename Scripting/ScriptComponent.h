#ifndef SCRIPTCOMPONENT_H_INCLUDED
#define SCRIPTCOMPONENT_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <map>
#include <list>

#include "RScriptFile.h"
#include "Utils/LuaMacros.h"

#include "../Game/TransformCompletedEvent.h"

#include "../Scene/Component.h"
#include "../Scene/Events/Subscription.h"
#include "../Scene/Events/ObjectClickedEvent.h"
#include "../Scene/Events/ObjectLoadedEvent.h"


struct LuaCoroutine final
{
	sol::thread thread;
	sol::coroutine coroutine;

	float timeToResume;

};


class ScriptComponent final : public Component
{
	VBCPP_COMPONENT(ScriptComponent, CT_SCRIPT)

	private:
		sol::state* _luaState;

		sol::environment _scriptEnvironment;

		RScriptFile* _scriptFile;

		bool _isInitialized;

		sol::function _initFunction;
		sol::function _changeTransformFunction;
		sol::function _updateFunction;
		sol::function _destroyFunction;
		sol::function _objectClickedFunction;
		sol::function _objectLoadedFunction;
		sol::function _transformCompletedFunction;

		Subscription _objectClickedSubscription;
		Subscription _objectLoadedSubscription;
		Subscription _transformCompletedSubscription;

		sol::table _customMembers;
		std::list<LuaCoroutine> _coroutines;

		void loadScript(RScriptFile* scriptFile);

		void validateFunctionResult(sol::protected_function_result& result);

		void onAttachedToScenObject() override;

		void setupScriptEnvironment();

		void runCoroutine(sol::function function, sol::variadic_args args);
		void updateCoroutines(float deltaTime);

		void onObjectClicked(const ObjectClickedEvent& event);
		void onObjectLoaded(const ObjectLoadedEvent& event);
		void onTransformCompleted(const TransformCompletedEvent& event);

	public:
		ScriptComponent(RScriptFile* scriptFile, sol::state* luaState, bool loadAfterCreate = true);
		~ScriptComponent();

		inline RScriptFile* getScriptFile() { return _scriptFile; }

		LUAF inline bool isInitialized() { return _isInitialized; }

		void addCustomMember(sol::object key, sol::object value);
		sol::object getCustomMember(sol::object key);

		void loadScript(bool setUpEnvironmentAndCallInitCallback = true);
		void reloadScriptFromResource();

		void changedTransform() override;
		void update(float deltaTime) override;

};


#endif // SCRIPTCOMPONENT_H_INCLUDED
