#ifndef SCRIPTCOMPONENT_H_INCLUDED
#define SCRIPTCOMPONENT_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <map>

#include "RScriptFile.h"
#include "Utils/LuaMacros.h"

#include "../Scene/Component.h"
#include "../Scene/Events/Subscription.h"
#include "../Scene/Events/ObjectClickedEvent.h"


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

		Subscription _objectClickedSubscription;

		std::map<std::string, sol::function> _customFunctions;

		void onAttachedToScenObject() override;

		void setupScriptEnvironment();

		void onObjectClicked(const ObjectClickedEvent& event);

	public:
		ScriptComponent(RScriptFile* scriptFile, sol::state* luaState, bool loadAfterCreate = true);
		~ScriptComponent();

		inline RScriptFile* getScriptFile() { return _scriptFile; }

		LUAF inline bool isInitialized() { return _isInitialized; }

		LUAF void exportFunction(const std::string& name, sol::function function);
		LUAF sol::function getFunction(const std::string& name);

		void loadScript(bool setUpEnvironmentAndCallInitCallback = true);
		void reloadScriptFromResource();

		void changedTransform() override;
		void update(float deltaTime) override;

};


#endif // SCRIPTCOMPONENT_H_INCLUDED
