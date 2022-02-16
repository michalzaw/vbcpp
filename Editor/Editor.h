#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED


#include <memory>

#include "EditorContext.h"
#include "EditorGUI.h"

#include "../Graphics/OGLDriver.h"
#include "../Graphics/Renderer.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Window/Window.h"

/*
class Editor
{
    private:
        std::unique_ptr<Window> _window;

        PhysicsManager* _physicsManager;
        SoundManager* _soundManager;
        SceneManager* _sceneManager;

		//std::shared_ptr<EditorContext> _editorContext;
        std::unique_ptr<EditorGUI> _editorGUI;

        bool _cameraActive;
        CameraFPS* _camera;
        SceneObject* _cameraObject;

        SceneObject* _axisObject;

		bool _addObjectMode;
		RObject* _objectToAdd;

        bool createWindow();
        void initializeEngineSubsystems();
        void clearScene();

		bool calculateMousePositionInWorldspaceUsingBulletRaycasting(glm::vec3 rayStart, glm::vec3 rayEnd, glm::vec3& position);

    public:
        Editor();
        ~Editor();

        Window* getWindow();

        void run();

        void processInput(double deltaTime);

		void mouseButtonCallback(int button, int action, int mods);

		void keyCallback(int key, int scancode, int action, int mods);

		void charCallback(unsigned int c);

		void changeWindowSizeCallback(int width, int height);

		void changeFramebufferSizeCallback(int width, int height);
};
*/

namespace vbEditor
{
	void setSelectedSceneObject(SceneObject* object);
	bool createWindow();
	void initializeEngineSubsystems();
	void clearScene();
	void shutDown();

	void drawMainMenu();
	void ShowTransformGizmo();
	void showRoadTools();
	void showPolygonEditTool();
	void updateRoads(float deltaTime);
	void addSceneObject();

	bool getGUIhasFocus();

	void renderGUI();

	void run();

	void loadMapData();
	void saveMapData();

} // namespace

#endif // EDITOR_H_INCLUDED
