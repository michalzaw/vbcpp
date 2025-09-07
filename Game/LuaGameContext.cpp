#include "LuaGameContext.h"

#include "../Scene/SceneManager.h"

#include "../Scripting/ScriptingManager.h"

#include "../Utils/InputSystem.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/RObjectLoader.h"


LuaGameContext::LuaGameContext(ScriptingManager* scriptingManger)
	: _scriptingManger(scriptingManger)
{
	initializeConstGlobalObjects();
}


LuaGameContext::~LuaGameContext()
{

}


void LuaGameContext::initializeConstGlobalObjects()
{
	_scriptingManger->setGlobalVariable("input", &InputSystem::getInstance());
}


void LuaGameContext::setSceneManager(SceneManager* sceneManager)
{
	_scriptingManger->setGlobalVariable("sceneManager", sceneManager);
}

void LuaGameContext::setActiveCamera(SceneObject* activeCameraObject)
{
	_scriptingManger->setGlobalVariable("activeCamera", activeCameraObject);
}


void LuaGameContext::setActiveBus(SceneObject* activeBusObject)
{
	_scriptingManger->setGlobalVariable("activeBus", activeBusObject);
}
