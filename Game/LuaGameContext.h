#ifndef LUAGAMECONFIG_H_INCLUDED
#define LUAGAMECONFIG_H_INCLUDED


class ScriptingManager;
class SceneManager;
class SceneObject;


class LuaGameContext final
{
	private:
		ScriptingManager* _scriptingManger;

		void initializeConstGlobalObjects();

	public:
		LuaGameContext(ScriptingManager* scriptingManger);
		~LuaGameContext();

		void setSceneManager(SceneManager* sceneManager);
		void setActiveCamera(SceneObject* activeCameraObject);
		void setActiveBus(SceneObject* activeBusObject);

};


#endif // LUAGAMECONFIG_H_INCLUDED
