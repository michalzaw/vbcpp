#include "Editor.h"

#include "Tools/RoadManipulator.h"

//#include "../Bus/BusLoader.h"

#include "../Game/Directories.h"

#include "../Scene/SceneLoader.h"
#include "../Scene/SceneSaver.h"

#include "../Utils/RaycastingUtils.h"
#include "../Utils/FilesHelper.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imGizmo.h"
#include "../ImGui/imRoadTools.h"
#include "glm/gtc/type_ptr.hpp"

#include "EditorContext.h"
#include "Windows/OpenDialogWindow.h"
#include "Windows/SceneGraphWindow.h"
#include "Windows/ObjectPropertiesWindow.h"
#include "Windows/MapInfoWindow.h"

//std::list<Editor*> editorInstances;

//Editor* editorInstance = nullptr;

void editorMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    //for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    //{
    //    if ((*i)->getWindow()->getWindow() == window)
    //    {
    //        (*i)->mouseButtonCallback(button, action, mods);
    //        break;
    //    }
    //}
	//editorInstance->mouseButtonCallback(button, action, mods);
}


void editorKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    //{
    //    if ((*i)->getWindow()->getWindow() == window)
    //    {
    //        (*i)->keyCallback(key, scancode, action, mods);
    //        break;
    //    }
    //}
	//editorInstance->keyCallback(key, scancode, action, mods);
}


void editorCharCallback(GLFWwindow* window, unsigned int c)
{
    //for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    //{
    //    if ((*i)->getWindow()->getWindow() == window)
    //    {
    //        (*i)->charCallback(c);
    //        break;
    //    }
    //}
	//editorInstance->charCallback(c);
}


void editorChangeWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    //for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    //{
    //    if ((*i)->getWindow()->getWindow() == window)
    //    {
    //        (*i)->changeWindowSizeCallback(width, height);
    //        break;
    //    }
    //}
	//editorInstance->changeWindowSizeCallback(width, height);
}


void editorFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    //{
    //    if ((*i)->getWindow()->getWindow() == window)
    //    {
    //        (*i)->changeFramebufferSizeCallback(width, height);
    //        break;
    //    }
    //}
	//editorInstance->changeFramebufferSizeCallback(width, height);
}

/*
Editor::Editor()
    : _cameraActive(false),
	_addObjectMode(false), _objectToAdd(nullptr)
{
    if (!createWindow())
    {
        return;
    }

    initializeEngineSubsystems();

	//_editorContext = std::make_shared<EditorContext>();
	//_editorContext->availableObjects = FilesHelper::getDirectoriesList(GameDirectories::OBJECTS);

    _editorGUI.reset(new EditorGUI(_window.get(), _sceneManager));

    //editorInstances.push_back(this);
	editorInstance = this;
}


Editor::~Editor()
{
    _physicsManager->stop();

    _soundManager->drop();
    _physicsManager->drop();
    delete _sceneManager;

	_window.release();
    //editorInstances.remove(this);
}


bool Editor::createWindow()
{
    _window = std::make_unique<Window>();
    if (!_window->createWindow(1400, 900, 10, 40, false, true))
    {
        return false;
    }
    _window->setWindowTitle("vbcpp - editor");

    glfwSetMouseButtonCallback(_window->getWindow(), editorMouseButtonCallback);
    glfwSetKeyCallback(_window->getWindow(), editorKeyCallback);
    glfwSetCharCallback(_window->getWindow(), editorCharCallback);
    glfwSetWindowSizeCallback(_window->getWindow(), editorChangeWindowSizeCallback);
    glfwSetFramebufferSizeCallback(_window->getWindow(), editorFramebufferSizeCallback);

    return true;
}


void Editor::initializeEngineSubsystems()
{
    OGLDriver::getInstance().initialize();

    Renderer& renderer = Renderer::getInstance();
	renderer.setMsaaAntialiasing(true);
	renderer.setMsaaAntialiasingLevel(4);
	renderer.setBloom(false);
	renderer.setIsShadowMappingEnable(false);
	renderer.init(_window->getWidth(), _window->getHeight());
	renderer.setDayNightRatio(1.0f);
	renderer.setAlphaToCoverage(true);
	renderer.setExposure(1.87022f);
	renderer.t = 0;

    _physicsManager = new PhysicsManager;
    _soundManager = new SoundManager;
    _sceneManager = new SceneManager(_physicsManager, _soundManager);

    clearScene();
}


void Editor::clearScene()
{
    _sceneManager->clearScene();

    _cameraObject = _sceneManager->addSceneObject("editor#cam1");
    _camera = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000);
    _cameraObject->addComponent(_camera);
    _camera->setRotationSpeed(0.01f);
    _camera->setMoveSpeed(8.0f);

    GraphicsManager::getInstance().setCurrentCamera(_camera);
    _soundManager->setActiveCamera(_camera);

}


bool Editor::calculateMousePositionInWorldspaceUsingBulletRaycasting(glm::vec3 rayStart, glm::vec3 rayDir, glm::vec3& position)
{
	glm::vec3 rayEnd = rayStart + (rayDir * 1000);
	btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(rayStart.x, rayStart.y, rayStart.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z));

	rayCallback.m_collisionFilterMask = COL_BUS | COL_DOOR | COL_ENV | COL_TERRAIN | COL_WHEEL;
	rayCallback.m_collisionFilterGroup = COL_ENV;

	_sceneManager->getPhysicsManager()->getDynamicsWorld()->rayTest(btVector3(rayStart.x, rayStart.y, rayStart.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z),
		rayCallback);

	if (rayCallback.hasHit())
	{
		position = glm::vec3(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z());

		return true;
	}

	return false;
}


Window* Editor::getWindow()
{
    return _window.get();
}


void Editor::run()
{
    Renderer& renderer = Renderer::getInstance();

    double lastXPos, lastYPos;
    double xPos, yPos;

    // Time calculation variables
    double deltaTime = 0.0;
    double accumulator = 0.0;

    const double TIME_STEP = 1/60.0f;
    const double MAX_ACCUMULATED_TIME = 1.0;

    double timePhysicsCurr;
    double timePhysicsPrev;
    timePhysicsPrev = timePhysicsCurr = glfwGetTime();

    //how long ago FPS counter was updated
    double lastFPSupdate = timePhysicsCurr;

    int nbFrames = 0;

    while (_window->isOpened())
    {
        // Time
        nbFrames++;

        timePhysicsCurr = glfwGetTime();
        deltaTime = timePhysicsCurr - timePhysicsPrev;
        timePhysicsPrev = timePhysicsCurr;

        deltaTime = std::max(0.0, deltaTime);
        accumulator += deltaTime;
        accumulator = clamp(accumulator, 0.0, MAX_ACCUMULATED_TIME);

        if ( timePhysicsCurr - lastFPSupdate >= 1.0f )
        {
            nbFrames = 0;
            lastFPSupdate += 1.0f;
        }


        // input
        if (!_editorGUI->GUIhasFocus())
        {
            processInput(deltaTime);

            glfwGetCursorPos(_window->getWindow(), &xPos, &yPos);

            if (_cameraActive)
            {
                double dx = (xPos - lastXPos);
                double dy = (yPos - lastYPos);

                if(_camera)
                    _cameraObject->rotate(-dy * _camera->getRotationSpeed(), -dx * _camera->getRotationSpeed(), 0.0f);
            }

            lastXPos = xPos;
            lastYPos = yPos;
        }


        // custom events from GUI
        while (_editorGUI->hasNextEvent())
        {
            EditorEvent event = _editorGUI->getNextEvent();

			SceneLoader sceneLoader(_sceneManager);
			SceneSaver sceneSaver(_sceneManager);

            switch (event.type)
            {
                case EET_NEW_CLICK:
                    clearScene();
                    _editorGUI->setSelectedSceneObject(nullptr);
                    break;

                case EET_OPEN_CLICK:
                    clearScene();
                    _editorGUI->setSelectedSceneObject(nullptr);

					sceneLoader.loadMap(event.params);
					//_editorContext->currentMapInfo.name = event.params;
					//_editorContext->currentMapInfo.author = sceneLoader.getLoadedSceneDescription().author;
                    break;

				case EET_SAVE_CLICK:
					//sceneSaver.saveMap(event.params, SceneDescription(_editorContext->currentMapInfo.author));
					break;

				case EET_ADD_OBJECT:
					_addObjectMode = true;
					_objectToAdd = ResourceManager::getInstance().loadRObject(event.params);

					break;

				case EET_SELECTED_OBJECT_DELETE_CLICK:
					_sceneManager->removeSceneObject(_editorGUI->getSelectedSceneObject());
					_editorGUI->setSelectedSceneObject(nullptr);

					break;
            }
        }


        // fixed update
        while ( accumulator > TIME_STEP )
        {
            accumulator -= TIME_STEP;

            //if (_editorGUI->getSelectedSceneObject() != nullptr)
            //{
            //    _axisObject->setPosition(_editorGUI->getSelectedSceneObject()->getPosition());
            //}
        }


        // rendering
        renderer.renderAll();

        _editorGUI->draw();

        _window->swapBuffers();
        _window->updateEvents();
    }
}


void Editor::processInput(double deltaTime)
{
    if (_cameraActive)
    {
        if (glfwGetKey( _window->getWindow(), GLFW_KEY_W ) == GLFW_PRESS)
        {
            _camera->moveForward(deltaTime);
        }

        if (glfwGetKey( _window->getWindow(), GLFW_KEY_S ) == GLFW_PRESS)
        {
            _camera->moveBackward(deltaTime);
        }

        if (glfwGetKey( _window->getWindow(), GLFW_KEY_D ) == GLFW_PRESS)
        {
            _camera->strafeRight(deltaTime);
        }

        if (glfwGetKey( _window->getWindow(), GLFW_KEY_A ) == GLFW_PRESS)
        {
            _camera->strafeLeft(deltaTime);
        }
    }
}


void Editor::mouseButtonCallback(int button, int action, int mods)
{
    if (_editorGUI->GUIhasFocus())
    {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        _cameraActive = true;
    }

     if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        _cameraActive = false;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
		double xpos, ypos;
		glfwGetCursorPos(_window->getWindow(), &xpos, &ypos);
		ypos = _window->getHeight() - ypos;

		glm::vec3 rayStart;
		glm::vec3 rayDir;
		calculateRay(xpos, ypos, _camera, rayStart, rayDir);

		if (_addObjectMode)
		{
			glm::vec3 hitPosition;
			if (calculateMousePositionInWorldspaceUsingBulletRaycasting(rayStart, rayDir, hitPosition))
			{
				SceneObject* newObject = RObjectLoader::createSceneObjectFromRObject(_objectToAdd, _objectToAdd->getName(), hitPosition, glm::vec3(0.0f, 0.0f, 0.0f), _sceneManager);

				//_editorGUI->setSelectedSceneObject(newObject);
			}
		}
		else
		{
			// collision with render objects
			SceneObject* selectedObject = nullptr;
			float d = std::numeric_limits<float>::max();

			std::list<RenderObject*>& renderObjects = GraphicsManager::getInstance().getRenderObjects();
			for (std::list<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i)
			{
				RenderObject* renderObject = *i;
				AABB* aabb = renderObject->getModel()->getAABB();
				glm::mat4 modelMatrix = renderObject->getSceneObject()->getGlobalTransformMatrix();
				float distance;
				if (isRayIntersectOBB(rayStart, rayDir, *aabb, modelMatrix, distance))
				{
					if (distance > 0.0f && distance < d)
					{
						selectedObject = renderObject->getSceneObject();
						d = distance;
					}
				}
			}

			//_editorGUI->setSelectedSceneObject(selectedObject);
		}
    }
}


void Editor::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if (_addObjectMode)
			_addObjectMode = false;
	}

}


void Editor::charCallback(unsigned int c)
{

}


void Editor::changeWindowSizeCallback(int width, int height)
{
    if (_camera)
        _camera->setWindowDimensions(width, height);
}




*/

namespace vbEditor
{
	enum ClickMode
	{
		CM_PICK_OBJECT,
		CM_ADD_OBJECT
	};

	Window window;
	int _windowWidth = 1024;
	int _windowHeight = 768;

	PhysicsManager* _physicsManager = nullptr;
	SoundManager* _soundManager = nullptr;
	SceneManager* _sceneManager = nullptr;

	CameraFPS* _camera = nullptr;
	SceneObject* _cameraObject = nullptr;
	SceneObject* _selectedSceneObject = nullptr;
	int roadActiveSegment = 0;
	int roadActivePoint = 0;

	static bool _showDemoWindow = false;
	static bool _showOpenDialogWindow = true;
	static bool _isCameraActive = false;
	static bool _showObjectPropertyEditor = true;
	static bool _showRoadTools = true;
	static bool _showCameraSettingsWindow = false;
	static bool _showMapInfoWindow = false;
	static bool _saveMap = false;
	static bool _addSceneObject = false;


	std::string windowTitle = "VBCPP - World Editor";

	std::vector<std::string> _availableMaps;// { "Demo", "Demo2", "Coœtam jeszcze" };
	std::vector<std::string> _availableObjects;

	MapInfo mapInfo = {"Unknown", "Unknown"};

	ClickMode _clickMode = CM_PICK_OBJECT;
	RObject* _objectToAdd = nullptr;

	// TODO: move to separate file
	bool calculateMousePositionInWorldspaceUsingBulletRaycasting(glm::vec3 rayStart, glm::vec3 rayDir, glm::vec3& position)
	{
		glm::vec3 rayEnd = rayStart + (rayDir * 1000);
		btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(rayStart.x, rayStart.y, rayStart.z),
			btVector3(rayEnd.x, rayEnd.y, rayEnd.z));

		rayCallback.m_collisionFilterMask = COL_BUS | COL_DOOR | COL_ENV | COL_TERRAIN | COL_WHEEL;
		rayCallback.m_collisionFilterGroup = COL_ENV;

		_sceneManager->getPhysicsManager()->getDynamicsWorld()->rayTest(btVector3(rayStart.x, rayStart.y, rayStart.z),
			btVector3(rayEnd.x, rayEnd.y, rayEnd.z),
			rayCallback);

		if (rayCallback.hasHit())
		{
			position = glm::vec3(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z());

			return true;
		}

		return false;
	}

	void setSelectedSceneObject(SceneObject* object)
	{
		_selectedSceneObject = object;
		centerGraphView();
	}

	void mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		if (getGUIhasFocus())
		{
			return;
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			_isCameraActive = true;
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			_isCameraActive = false;
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			double xpos, ypos;
			glfwGetCursorPos(glfwWindow, &xpos, &ypos);
			ypos = window.getHeight() - ypos;

			glm::vec3 rayStart;
			glm::vec3 rayDir;
			calculateRay(xpos, ypos, _camera, rayStart, rayDir);

			if (_clickMode == CM_ADD_OBJECT)
			{
				glm::vec3 hitPosition;
				if (calculateMousePositionInWorldspaceUsingBulletRaycasting(rayStart, rayDir, hitPosition))
				{
					SceneObject* newObject = RObjectLoader::createSceneObjectFromRObject(_objectToAdd, _objectToAdd->getName(), hitPosition, glm::vec3(0.0f, 0.0f, 0.0f), _sceneManager);

					setSelectedSceneObject(newObject);
				}
			}
			else if (_clickMode == CM_PICK_OBJECT)
			{
				// collision with render objects
				SceneObject* selectedObject = nullptr;
				float d = std::numeric_limits<float>::max();

				std::list<RenderObject*>& renderObjects = GraphicsManager::getInstance().getRenderObjects();
				for (std::list<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i)
				{
					RenderObject* renderObject = *i;
					AABB* aabb = renderObject->getModel()->getAABB();
					glm::mat4 modelMatrix = renderObject->getSceneObject()->getGlobalTransformMatrix();
					float distance;
					if (isRayIntersectOBB(rayStart, rayDir, *aabb, modelMatrix, distance))
					{
						if (distance > 0.0f && distance < d && renderObject->getSceneObject()->getName() != "terrain")
						{
							selectedObject = renderObject->getSceneObject();
							d = distance;
						}
					}
				}

				setSelectedSceneObject(selectedObject);
			}
		}
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			if (_clickMode == CM_ADD_OBJECT)
			{
				_clickMode = CM_PICK_OBJECT;
			}
			std::cout << "AA\n";
			GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
			glfwSetCursor(window, cursor);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

	}

	void processInput(double deltaTime)
	{
	
		if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		{
			_camera->moveForward(deltaTime);
		}

		if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		{
			_camera->moveBackward(deltaTime);
		}

		if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		{
			_camera->strafeRight(deltaTime);
		}

		if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		{
			_camera->strafeLeft(deltaTime);
		}
	}

	void changeFramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height)
	{
		Renderer::getInstance().setWindowDimensions(width, height);
		window.setWindowSize(width, height);

	}

	void changeWindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		if (_camera)
			_camera->setWindowDimensions(width, height);
	}

	bool createWindow()
	{
		if (!window.createWindow(_windowWidth, _windowHeight, 10, 40, false, true, true))
		{
			return false;
		}
		window.setWindowTitle(windowTitle);

		glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
		glfwSetKeyCallback(window.getWindow(), keyCallback);
		glfwSetCharCallback(window.getWindow(), editorCharCallback);
		glfwSetWindowSizeCallback(window.getWindow(), changeWindowSizeCallback);
		glfwSetFramebufferSizeCallback(window.getWindow(), changeFramebufferSizeCallback);

		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 130");

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("fonts/arial.ttf", 13.0f);
		io.Fonts->AddFontDefault();

		return true;
	}

	void initializeEngineSubsystems()
	{
//#ifdef DEVELOPMENT_RESOURCES
		GameConfig::getInstance().loadDevelopmentConfig("devSettings.xml");
		ResourceManager::getInstance().setAlternativeResourcePath(GameConfig::getInstance().alternativeResourcesPath);
//#endif // DEVELOPMENT_RESOURCES

		GameConfig::getInstance().mode = GM_EDITOR;

		OGLDriver::getInstance().initialize();

		Renderer& renderer = Renderer::getInstance();
		renderer.setMsaaAntialiasing(true);
		renderer.setMsaaAntialiasingLevel(4);
		renderer.setBloom(false);
		renderer.setIsShadowMappingEnable(false);
		renderer.init(window.getWidth(), window.getHeight());
		renderer.setDayNightRatio(1.0f);
		renderer.setAlphaToCoverage(true);
		renderer.setExposure(1.87022f);
		renderer.t = 0;

		_physicsManager = new PhysicsManager;
		_soundManager = new SoundManager;
		_sceneManager = new SceneManager(_physicsManager, _soundManager);

		_cameraObject = _sceneManager->addSceneObject("editor#CameraFPS");
		_camera = GraphicsManager::getInstance().addCameraFPS(_windowWidth, _windowHeight, degToRad(58.0f), 0.1f, 1000);
		_cameraObject->addComponent(_camera);
		_camera->setRotationSpeed(0.01f);
		_camera->setMoveSpeed(50.0f);

		GraphicsManager::getInstance().setCurrentCamera(_camera);
		_soundManager->setActiveCamera(_camera);

		_availableMaps = FilesHelper::getDirectoriesList(GameDirectories::MAPS);
		_availableObjects = FilesHelper::getDirectoriesList(GameDirectories::OBJECTS);

		std::vector<std::string> availableMapsDev = FilesHelper::getDirectoriesList(GameConfig::getInstance().alternativeResourcesPath + GameDirectories::MAPS);
		std::vector<std::string> availableObjectsDev = FilesHelper::getDirectoriesList(GameConfig::getInstance().alternativeResourcesPath + GameDirectories::OBJECTS);
		_availableMaps.insert(_availableMaps.end(), availableMapsDev.begin(), availableMapsDev.end());
		_availableObjects.insert(_availableObjects.end(), availableObjectsDev.begin(), availableObjectsDev.end());

		_showOpenDialogWindow = true;
	}

	void clearScene()
	{

	}

	void drawMainMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New map...", NULL, &_showDemoWindow);
				ImGui::MenuItem("Open map...", NULL, &_showDemoWindow);
				ImGui::MenuItem("Save", NULL, &_saveMap);
				//ImGui::MenuItem("Save as...", "CTRL+SHIFT+S");
				ImGui::Separator();
				ImGui::MenuItem("Exit");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Map"))
			{
				ImGui::MenuItem("Map Description", NULL, &_showMapInfoWindow);
				ImGui::MenuItem("Add new Scene Object..", NULL, &_addSceneObject);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("Camera Settings", NULL, &_showCameraSettingsWindow);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				ImGui::MenuItem("Demo", NULL, &_showDemoWindow);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void renderGUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuizmo::BeginFrame();
		ImRoadTools::BeginFrame();
		RoadManipulator::BeginFrame();

		if (_clickMode == CM_ADD_OBJECT)
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

		drawMainMenu();

		if (_showDemoWindow)
			ImGui::ShowTestWindow();

		static int currentSelection = 0;
		if (_showOpenDialogWindow)
		{
			if (openMapDialog("Open map...", "Open", _availableMaps, currentSelection))
			{
				_showOpenDialogWindow = false;
				SceneLoader sceneLoader(_sceneManager);
				sceneLoader.loadMap(_availableMaps[currentSelection]);

				mapInfo.name = _availableMaps[currentSelection];
				mapInfo.author = sceneLoader.getLoadedSceneDescription().author;
			}
		}

		static int currentObjectSelection = 0;
		if (_addSceneObject)
		{
			if (openMapDialog("Add Scene Object...", "Add", _availableObjects, currentObjectSelection))
			{
				_addSceneObject = false;

				std::string objName = _availableObjects[currentObjectSelection];
				
				_clickMode = CM_ADD_OBJECT;
				_objectToAdd = ResourceManager::getInstance().loadRObject(objName);
			}
		}

		if (_saveMap)
		{
			//printf("Saving map\n");
			SceneSaver sceneSaver(_sceneManager);
			sceneSaver.saveMap(mapInfo.name, SceneDescription(mapInfo.author));
			_saveMap = false;
		}

		if (_showMapInfoWindow)
		{
			static MapInfo outputData;
			outputData.name = mapInfo.name;
			outputData.author = mapInfo.author;
			if (MapInfoWindow(outputData, std::string("Update")))
			{
				mapInfo.name = outputData.name;
				mapInfo.author = outputData.author;
				_showMapInfoWindow = false;
			}
		}

		if (_showCameraSettingsWindow)
		{
			CameraFPS* cameraFPS = dynamic_cast<CameraFPS*>(GraphicsManager::getInstance().getCurrentCamera());
			if(cameraFPS)
				CameraSettingsWindow(cameraFPS);
		}

		if (_showObjectPropertyEditor)
		{
			SceneGraphWindow(_sceneManager->getSceneObjects());
			showObjectProperties();
		}

		if (_selectedSceneObject)
		{
			//ShowTransformGizmo();

			//if (_showRoadTools)
			//{
				RoadObject* roadComponent = dynamic_cast<RoadObject*>(_selectedSceneObject->getComponent(CT_ROAD_OBJECT));

				if (roadComponent)
					showRoadTools();
				else
					ShowTransformGizmo();
			//}
		}


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void run()
	{
		Renderer& renderer = Renderer::getInstance();

		double lastXPos, lastYPos;
		double xPos, yPos;

		// Time calculation variables
		double deltaTime = 0.0;
		double accumulator = 0.0;

		const double TIME_STEP = 1 / 60.0f;
		const double MAX_ACCUMULATED_TIME = 1.0;

		double timePhysicsCurr;
		double timePhysicsPrev;
		timePhysicsPrev = timePhysicsCurr = glfwGetTime();

		//how long ago FPS counter was updated
		double lastFPSupdate = timePhysicsCurr;

		int nbFrames = 0;

		while (window.isOpened())
		{
			// Time
			nbFrames++;

			timePhysicsCurr = glfwGetTime();
			deltaTime = timePhysicsCurr - timePhysicsPrev;
			timePhysicsPrev = timePhysicsCurr;

			deltaTime = std::max(0.0, deltaTime);
			accumulator += deltaTime;
			accumulator = clamp(accumulator, 0.0, MAX_ACCUMULATED_TIME);

			if (timePhysicsCurr - lastFPSupdate >= 1.0f)
			{
				std::string sTiming = toString(nbFrames);

				// Append the FPS value to the window title details
				std::string newWindowTitle = windowTitle + " | FPS: " + sTiming;
				window.setWindowTitle(newWindowTitle);

				nbFrames = 0;
				lastFPSupdate += 1.0f;
			}

			
			// input
			if (!getGUIhasFocus())
			{
			processInput(deltaTime);
				

				glfwGetCursorPos(window.getWindow(), &xPos, &yPos);

				if (_isCameraActive)
				{
					double dx = (xPos - lastXPos);
					double dy = (yPos - lastYPos);

					if (_camera)
						_cameraObject->rotate(static_cast<float>(-dy * _camera->getRotationSpeed()),
												static_cast<float>(-dx * _camera->getRotationSpeed()), 
													0.0f);
				}

				lastXPos = xPos;
				lastYPos = yPos;
			}
			

			// fixed update
			while (accumulator > TIME_STEP)
			{
				accumulator -= TIME_STEP;

			}


			// rendering
			renderer.renderAll();
			renderGUI();

			window.swapBuffers();
			window.updateEvents();
		}
	}

	void shutDown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		_physicsManager->stop();

		_soundManager->drop();
		_physicsManager->drop();
		delete _sceneManager;
	}

	bool getGUIhasFocus()
	{
		return (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard);
	}                    

	void loadMapData()
	{

	}

	void saveMapData()
	{

	}

	void ShowTransformGizmo()
	{

		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		//static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		/*
		if (ImGui::IsKeyPressed(69)) // E key
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(82)) // R key
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(84)) // T key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		*/
		if (ImGui::RadioButton("Global", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
		ImGui::SameLine();
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;

		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		/*
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];

		glm::mat4 objMat = glm::mat4(1.);
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(obj->getGlobalTransformMatrix()), matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation, 3);
		ImGui::InputFloat3("Rt", matrixRotation, 3);
		ImGui::InputFloat3("Sc", matrixScale, 3);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(obj->getGlobalTransformMatrix()));
		*/
		/*
		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		*/

		static bool useSnap(false);

		if (ImGui::IsKeyPressed(80))
			useSnap = !useSnap;
		ImGui::Checkbox("", &useSnap);

		ImGui::SameLine();

		//vec_t snap;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };

		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			//snap = config.mSnapTranslation;
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			//snap = config.mSnapRotation;
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			//snap = config.mSnapScale;
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}

		ImGui::Checkbox("Bound Sizing", &boundSizing);
		if (boundSizing)
		{
			ImGui::PushID(3);
			ImGui::Checkbox("", &boundSizingSnap);
			ImGui::SameLine();
			ImGui::InputFloat3("Snap", boundsSnap);
			ImGui::PopID();
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(glm::value_ptr(_camera->getViewMatrix()), glm::value_ptr(_camera->getProjectionMatrix()),
			mCurrentGizmoOperation, mCurrentGizmoMode,
			glm::value_ptr(_selectedSceneObject->getLocalTransformMatrix()),
			NULL,
			useSnap ? &snap[0] : NULL,
			boundSizing ? bounds : NULL,
			boundSizingSnap ? boundsSnap : NULL
		);
	}

	void showRoadTools()
	{
		RoadObject* roadComponent = dynamic_cast<RoadObject*>(_selectedSceneObject->getComponent(CT_ROAD_OBJECT));

		ImGuiIO& io = ImGui::GetIO();
		RoadManipulator::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		RoadManipulator::Manipulate(_camera->getViewMatrix(), _camera->getProjectionMatrix(),
			_selectedSceneObject->getLocalTransformMatrix(),
			roadComponent->getSegments());

		static float timer = 0.0f;
		timer += 1.0f / 60.0f;

		if (timer >= 0.2f)
		{
			roadComponent->buildModel();
			timer = 0.0f;
		}

		roadActiveSegment = RoadManipulator::GetActiveSegment();
		roadActivePoint = RoadManipulator::GetActivePoint();
	}

	void addSceneObject()
	{

	}

} // namespace