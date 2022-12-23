#include "Editor.h"

#include <future>

#include "Tools/RoadManipulator.h"
#include "Tools/AxisTool.h"

#include "Utils/RoadPolygonsGenerator.h"

//#include "../Bus/BusLoader.h"

#include "../Game/AIAgent.h"
#include "../Game/BusStartPoint.h"
#include "../Game/Directories.h"
#include "../Game/GameLogicSystem.h"
#include "../Game/PathComponent.h"

#include "../ImGuiInterface/ImGuiInterface.h"
#include "../ImGuiInterface/VariablesWindow.h"

#include "../Scene/SceneLoader.h"
#include "../Scene/SceneSaver.h"

#include "../Utils/RaycastingUtils.h"
#include "../Utils/FilesHelper.h"
#include "../Utils/ResourceDescription.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include "glm/gtc/type_ptr.hpp"

#include "EditorContext.h"
#include "Windows/OpenDialogWindow.h"
#include "Windows/SceneGraphWindow.h"
#include "Windows/ObjectPropertiesWindow.h"
#include "Windows/MapInfoWindow.h"
#include "Windows/MaterialEditorWindow.h"
#include "Windows/GenerateObjectsAlongCurveWindow.h"
#include "Windows/LoggerWindow.h"

#include "../Graphics/BezierCurve.h"
#include "../Graphics/ShapePolygonComponent.h"
#include "../Graphics/SkeletalAnimationComponent.h"
#include "../Graphics/SkeletalAnimationComponent2.h"

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
		CM_ADD_OBJECT,
		CM_ROAD_EDIT
	};

	struct RoadConnectionPoint
	{
		CrossroadComponent* crossroadComponent;
		int index;

		RoadIntersectionComponent* roadIntersectionComponent;

		RoadConnectionPoint(CrossroadComponent* crossroadComponent, int index)
			: crossroadComponent(crossroadComponent), index(index), roadIntersectionComponent(nullptr)
		{}

		RoadConnectionPoint(RoadIntersectionComponent* roadIntersectionComponent)
			: crossroadComponent(nullptr), index(-1), roadIntersectionComponent(roadIntersectionComponent)
		{}
	};

	Window window;
	int _windowWidth = 1024;
	int _windowHeight = 768;

	GraphicsManager* _graphicsManager = nullptr;
	PhysicsManager* _physicsManager = nullptr;
	SoundManager* _soundManager = nullptr;
	SceneManager* _sceneManager = nullptr;

	CameraFPS* _camera = nullptr;
	SceneObject* _cameraObject = nullptr;
	SceneObject* _selectedSceneObject = nullptr;
	int roadActiveSegment = 0;
	int roadActivePoint = 0;

	std::vector<RoadObject*> _selectedRoads;

	static bool _showDemoWindow = false;
	static bool _addRoadIntersectionDialogWindow = false;
	static bool _isCameraActive = false;
	static bool _showObjectPropertyEditor = true;
	static bool _showRoadTools = true;
	static bool _showCameraSettingsWindow = false;
	static bool _showMapInfoWindow = false;
	static bool _saveMap = false;
	static bool _addSceneObject = false;
	static bool _addRoadDialogWindow = false;
	static bool _addRoad2DialogWindow = false;
	bool _showMaterialEditorWindow = false;
	bool _showGenerateObjectsAlongCurveWindow = false;

	std::string windowTitle = "VBCPP - World Editor";

	ResourceDescription mapInfo = {"Unknown", "Unknown", "Unknown"};

	ClickMode _clickMode = CM_PICK_OBJECT;
	RObject* _objectToAddDefinition = nullptr;
	SceneObject* _objectToAdd = nullptr;

	ImGuiInterface* _imGuiInterface = nullptr;
	OpenDialogWindow* _openMapDialogWindow = nullptr;
	OpenDialogWindow* _addSceneObjectDialogWindow = nullptr;
	OpenDialogWindow* _selectRoadProfileDialogWindow = nullptr;

	Window* _backgroundWindow = nullptr;
	std::future<void> _loadingSceneFuture;
	bool _isLoading = false;
	SceneManager* _newSceneManager = nullptr;

	void setClickMode(ClickMode clickMode)
	{
		if (_clickMode == CM_ADD_OBJECT)
		{
			if (_objectToAdd != nullptr)
			{
				_sceneManager->removeSceneObject(_objectToAdd);
				_objectToAddDefinition = nullptr;
				_objectToAdd = nullptr;
			}
		}

		_clickMode = clickMode;

		if (_clickMode == CM_PICK_OBJECT)
		{
			GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
			glfwSetCursor(window.getWindow(), cursor);
			glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void setSelectedSceneObject(SceneObject* object)
	{
		_selectedSceneObject = object;
		centerGraphView();

		if (object != nullptr && (object->getComponent(CT_ROAD_OBJECT) != nullptr || object->getComponent(CT_SHAPE_POLYGON) != nullptr || object->getComponent(CT_BEZIER_CURVE) != nullptr))
		{
			setClickMode(CM_ROAD_EDIT); // todo: uzywamy tez dla polygon chociaz to nie jest droga, ale jego sposob edycji jest taki sam
		}
	}

	bool getCursorPositionIn3D(GLFWwindow* glfwWindow, glm::vec3& outCursorPosition)
	{
		double xpos, ypos;
		glfwGetCursorPos(glfwWindow, &xpos, &ypos);
		ypos = window.getHeight() - ypos;

		glm::vec3 rayStart;
		glm::vec3 rayDir;
		calculateRay(xpos, ypos, _camera, rayStart, rayDir);

		return _sceneManager->getPhysicsManager()->rayTest(rayStart, rayDir, COL_TERRAIN, COL_WHEEL, outCursorPosition);
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
			if (_clickMode == CM_ADD_OBJECT)
			{
				_objectToAdd->setFlags(SOF_NONE);
				setSelectedSceneObject(_objectToAdd);
				loadNewObjectToAdd();
			}
			else if (_clickMode == CM_ROAD_EDIT && glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				glm::vec3 hitPosition;
				if (getCursorPositionIn3D(glfwWindow, hitPosition))
				{
					RoadObject* roadObject = dynamic_cast<RoadObject*>(_selectedSceneObject->getComponent(CT_ROAD_OBJECT));
					if (roadObject != nullptr && roadObject->getRoadType() != RoadType::BEZIER_CURVES)
					{
						roadObject->addPoint(hitPosition);
					}

					ShapePolygonComponent* shapePolygonObject = dynamic_cast<ShapePolygonComponent*>(_selectedSceneObject->getComponent(CT_SHAPE_POLYGON));
					if (shapePolygonObject != nullptr)
					{
						shapePolygonObject->addPoint(hitPosition);
					}

					BezierCurve* bezierCurve = dynamic_cast<BezierCurve*>(_selectedSceneObject->getComponent(CT_BEZIER_CURVE));
					if (bezierCurve != nullptr)
					{
						bezierCurve->addPoint(hitPosition);
					}
				}
			}
			else if (_clickMode == CM_PICK_OBJECT || _clickMode == CM_ROAD_EDIT)
			{
				double xpos, ypos;
				glfwGetCursorPos(glfwWindow, &xpos, &ypos);
				ypos = window.getHeight() - ypos;

				glm::vec3 rayStart;
				glm::vec3 rayDir;
				calculateRay(xpos, ypos, _camera, rayStart, rayDir);

				// collision with render objects
				SceneObject* selectedObject = nullptr;
				float d = std::numeric_limits<float>::max();

				std::list<RenderObject*>& renderObjects = _graphicsManager->getRenderObjects();
				for (std::list<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i)
				{
					RenderObject* renderObject = *i;
					if (renderObject->getSceneObject()->getFlags() & SOF_NOT_SELECTABLE_ON_SCENE)
					{
						continue;
					}
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

				setSelectedSceneObject(selectedObject);
			}
		}
	}

	void mouseCursourPosCallback(GLFWwindow* glfwWindow, double xPosition, double yPosition)
	{
		if (_clickMode == CM_ADD_OBJECT && _objectToAdd != nullptr)
		{
			glm::vec3 mousePosition;
			if (getCursorPositionIn3D(glfwWindow, mousePosition))
			{
				if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				{
					mousePosition.x = round(mousePosition.x);
					mousePosition.z = round(mousePosition.z);
				}
				_objectToAdd->setPosition(mousePosition);
			}
		}
	}

	void mouseScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset)
	{
		if (_objectToAdd != nullptr)
		{
			if (yOffset > 0)
			{
				_objectToAdd->rotate(0.0f, degToRad(90.0f), 0.0f);
			}
			else if (yOffset < 0)
			{
				_objectToAdd->rotate(0.0f, -degToRad(90.0f), 0.0f);
			}
		}
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			setClickMode(CM_PICK_OBJECT);
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
		Logger::addCustomAppender([](LogLevel level, const std::string& logMessage)
			{
				LoggerWindow::addLog(level, logMessage + "\n");
			});

		if (!window.createWindow(_windowWidth, _windowHeight, 10, 40, false, true, false, true))
		{
			return false;
		}
		window.setWindowTitle(windowTitle);

		glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
		glfwSetCursorPosCallback(window.getWindow(), mouseCursourPosCallback);
		glfwSetScrollCallback(window.getWindow(), mouseScrollCallback);
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

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		LoggerWindow::init();

		return true;
	}

	void asyncLoadScene(const std::string& mapName, SceneManager* sceneManager, Window* backgroundWindow)
	{
		glfwMakeContextCurrent(backgroundWindow->getWindow());

		_cameraObject = sceneManager->addSceneObject("editor#CameraFPS");
		_cameraObject->setFlags(SOF_NOT_SELECTABLE | SOF_NOT_SERIALIZABLE);
		_camera = sceneManager->getGraphicsManager()->addCameraFPS(window.getWidth(), window.getHeight(), degToRad(58.0f), 0.1f, 1000);
		_cameraObject->addComponent(_camera);
		_camera->setRotationSpeed(0.01f);
		_camera->setMoveSpeed(50.0f);

		sceneManager->getGraphicsManager()->setCurrentCamera(_camera);
		sceneManager->getSoundManager()->setActiveCamera(_camera);


		SceneLoader sceneLoader(sceneManager);
		sceneLoader.loadMap(mapName);

		mapInfo.name = mapName;
		mapInfo.author = sceneLoader.getLoadedSceneDescription().author;

		glFinish();

		glfwMakeContextCurrent(nullptr);
	}

	void openMap(const std::string& mapName)
	{
		GraphicsManager* graphicsManager = new GraphicsManager;
		PhysicsManager* physicsManager = new PhysicsManager;
		SoundManager* soundManager = new SoundManager;
		_newSceneManager = new SceneManager(graphicsManager, physicsManager, soundManager);

		if (_backgroundWindow == nullptr)
		{
			_backgroundWindow = new Window;
			_backgroundWindow->createInvisibleWindow(&window);
		}

		_selectedSceneObject = nullptr;
		_selectedRoads.clear();

		setClickMode(CM_PICK_OBJECT);
		_objectToAddDefinition = nullptr;
		_objectToAdd = nullptr;
		
		_isLoading = true;
		_loadingSceneFuture = std::async(std::launch::async, &asyncLoadScene, mapName, _newSceneManager, _backgroundWindow);
	}

	void swapScenes(SceneManager* newSceneManager)
	{
		Renderer::getInstance().setGraphicsManager(newSceneManager->getGraphicsManager());
		Renderer::getInstance().rebuildStaticLighting();

		_physicsManager->stop();
		_soundManager->drop();
		_physicsManager->drop();
		delete _sceneManager;

		_graphicsManager = newSceneManager->getGraphicsManager();
		_physicsManager = newSceneManager->getPhysicsManager();
		_soundManager = newSceneManager->getSoundManager();
		_sceneManager = newSceneManager;
	}

	void initializeEngineSubsystems()
	{
		srand(static_cast<unsigned int>(time(NULL)));

#ifdef DEVELOPMENT_RESOURCES
		GameConfig::getInstance().loadDevelopmentConfig("devSettings.xml");
		ResourceManager::getInstance().setAlternativeResourcePath(GameConfig::getInstance().alternativeResourcesPath);
#endif // DEVELOPMENT_RESOURCES

		GameConfig::getInstance().mode = GM_EDITOR;

		OGLDriver::getInstance().initialize();

		_graphicsManager = new GraphicsManager;
		_physicsManager = new PhysicsManager;
		_soundManager = new SoundManager;
		_sceneManager = new SceneManager(_graphicsManager, _physicsManager, _soundManager);

		Renderer& renderer = Renderer::getInstance();
		renderer.setGraphicsManager(_graphicsManager);
		renderer.setMsaaAntialiasing(true);
		renderer.setMsaaAntialiasingLevel(4);
		renderer.setBloom(false);
		renderer.setIsShadowMappingEnable(true);
		renderer.init(window.getWidth(), window.getHeight());
		renderer.setDayNightRatio(1.0f);
		renderer.setAlphaToCoverage(true);
		renderer.setExposure(1.87022f);
		renderer.setToneMappingType(TMT_CLASSIC);
		renderer.t = 0;

		_cameraObject = _sceneManager->addSceneObject("editor#CameraFPS");
		_cameraObject->setFlags(SOF_NOT_SELECTABLE | SOF_NOT_SERIALIZABLE);
		_camera = _graphicsManager->addCameraFPS(_windowWidth, _windowHeight, degToRad(58.0f), 0.1f, 1000);
		_cameraObject->addComponent(_camera);
		_camera->setRotationSpeed(0.01f);
		_camera->setMoveSpeed(50.0f);

		_graphicsManager->setCurrentCamera(_camera);
		_soundManager->setActiveCamera(_camera);

		std::vector<std::string> mapsPaths = { GameDirectories::MAPS };
		std::vector<std::string> objectsPaths = { GameDirectories::OBJECTS };
		std::vector<std::string> roadProfilesPaths = { GameDirectories::ROAD_PROFILES };

#ifdef DEVELOPMENT_RESOURCES
		mapsPaths.push_back(GameConfig::getInstance().alternativeResourcesPath + GameDirectories::MAPS);
		objectsPaths.push_back(GameConfig::getInstance().alternativeResourcesPath + GameDirectories::OBJECTS);
		roadProfilesPaths.push_back(GameConfig::getInstance().alternativeResourcesPath + GameDirectories::ROAD_PROFILES);
#endif // DEVELOPMENT_RESOURCES

		_imGuiInterface = new ImGuiInterface(_sceneManager);
		_imGuiInterface->setIsOpen(true);

		_openMapDialogWindow = new OpenDialogWindow("Open map...", "Open", mapsPaths, _sceneManager);
		_openMapDialogWindow->setDefaultDirectoryFilter("scene.xml");
		_openMapDialogWindow->setDefaultDescriptionLoader("scene.xml", "Scene");
		_openMapDialogWindow->setOnOkClickCallback([](const std::string& currentSelection)
			{
				LOG_DEBUG(LOG_VARIABLE(currentSelection));
				openMap(currentSelection);
			});
		_imGuiInterface->addWindow(_openMapDialogWindow);

		*(_openMapDialogWindow->getOpenFlagPointer()) = true;


		_addSceneObjectDialogWindow = new OpenDialogWindow("Add Scene Object...", "Add", objectsPaths, _sceneManager);
		_addSceneObjectDialogWindow->setDefaultDirectoryFilter("object.xml");
		_addSceneObjectDialogWindow->setDefaultDescriptionLoader("object.xml", "Object");
		_imGuiInterface->addWindow(_addSceneObjectDialogWindow);


		_selectRoadProfileDialogWindow = new OpenDialogWindow("Select Road Profile...", "Select", roadProfilesPaths, _sceneManager);
		_selectRoadProfileDialogWindow->setDefaultDirectoryFilter("profile.xml");
		_selectRoadProfileDialogWindow->setDefaultDescriptionLoader("profile.xml", "Profile");
		_imGuiInterface->addWindow(_selectRoadProfileDialogWindow);
	}

	void loadNewObjectToAdd()
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		getCursorPositionIn3D(window.getWindow(), position);

		_objectToAdd = RObjectLoader::createSceneObjectFromRObject(_objectToAddDefinition, _objectToAddDefinition->getName(), position, glm::vec3(0.0f, 0.0f, 0.0f), _sceneManager);
		_objectToAdd->setFlags(SOF_NOT_SELECTABLE | SOF_NOT_SERIALIZABLE);
	}

	void addObject(const std::string& objectName)
	{
		setClickMode(CM_ADD_OBJECT);
		_objectToAddDefinition = ResourceManager::getInstance().loadRObject(objectName);

		loadNewObjectToAdd();
	}

	void addRoad(const std::string& roadProfileName, RoadType roadType)
	{
		setClickMode(CM_ROAD_EDIT);

		RRoadProfile* roadProfile = ResourceManager::getInstance().loadRoadProfile(roadProfileName);

		SceneObject* roadSceneObject = _sceneManager->addSceneObject("Road");

		if (roadType == RoadType::BEZIER_CURVES)
		{
			BezierCurve* bezierCurve = _graphicsManager->addBezierCurve();
			roadSceneObject->addComponent(bezierCurve);
		}

		RoadObject* roadRenderObject = _graphicsManager->addRoadObject(roadType, roadProfile, {}, {}, true, roadSceneObject);
		roadRenderObject->setInteractiveMode(true);
		roadRenderObject->setCastShadows(false);

		setSelectedSceneObject(roadSceneObject);
	}

	void addRoadLinesAndArc(const std::string& roadProfileName)
	{
		addRoad(roadProfileName, RoadType::LINES_AND_ARC);
	}

	void addRoadBezierCurve(const std::string& roadProfileName)
	{
		addRoad(roadProfileName, RoadType::BEZIER_CURVES);
	}

	void addRoadIntersection(const std::string& roadProfileName)
	{
		RRoadProfile* roadProfile = ResourceManager::getInstance().loadRoadProfile(roadProfileName);

		SceneObject* sceneObject = _sceneManager->addSceneObject("Road intersection");
		sceneObject->addComponent(_graphicsManager->addRoadIntersection(roadProfile, true));

		setSelectedSceneObject(sceneObject);
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
				ImGui::MenuItem("Open map...", NULL, _openMapDialogWindow->getOpenFlagPointer());
				ImGui::MenuItem("Save", NULL, &_saveMap);
				//ImGui::MenuItem("Save as...", "CTRL+SHIFT+S");
				ImGui::Separator();
				ImGui::MenuItem("Exit");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Map"))
			{
				ImGui::MenuItem("Map Description", NULL, &_showMapInfoWindow);
				if (ImGui::MenuItem("Add new Scene Object..", NULL, _addSceneObjectDialogWindow->getOpenFlagPointer()))
				{
					_addSceneObjectDialogWindow->setOnOkClickCallback(addObject);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Add new Road..", NULL, _selectRoadProfileDialogWindow->getOpenFlagPointer()))
				{
					_selectRoadProfileDialogWindow->setOnOkClickCallback(addRoadLinesAndArc);
				}
				if (ImGui::MenuItem("Add new Road (Bezier curves)..", NULL, _selectRoadProfileDialogWindow->getOpenFlagPointer()))
				{
					_selectRoadProfileDialogWindow->setOnOkClickCallback(addRoadBezierCurve);
				}
				if (ImGui::MenuItem("Add new road intersection", NULL, _selectRoadProfileDialogWindow->getOpenFlagPointer()))
				{
					_selectRoadProfileDialogWindow->setOnOkClickCallback(addRoadIntersection);
				}
				if (ImGui::MenuItem("Add new custom polygon", NULL))
				{
					SceneObject* polygonSceneObject = _sceneManager->addSceneObject("Polygon");
					polygonSceneObject->addComponent(new ShapePolygonComponent);

					setSelectedSceneObject(polygonSceneObject);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Add new decal (WIP)", NULL))
				{
					SceneObject* decalSceneObject = _sceneManager->addSceneObject("Decal");

					Material* material = new Material;
					material->shader = DECAL_MATERIAL;
					material->shininess = 96.0f;
					material->diffuseTexture = ResourceManager::getInstance().loadTexture("RoadProfiles/decal2.png");
					material->diffuseTexture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);

					Cube* decal = new Cube(1, material);
					decal->init();
					_graphicsManager->addRenderObject(decal, decalSceneObject);

					setSelectedSceneObject(decalSceneObject);
				}
				ImGui::Separator();

				if (ImGui::BeginMenu("Add mesh"))
				{
					if (ImGui::MenuItem("Plane", NULL))
					{
						SceneObject* sceneObject = _sceneManager->addSceneObject("Plane");

						Material* material = new Material;
						material->shader = SOLID_MATERIAL;
						material->shininess = 96.0f;
						material->diffuseTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();

						PlanePrefab* plane = new PlanePrefab(glm::vec2(1, 1), material);
						plane->init();
						_graphicsManager->addRenderObject(plane, sceneObject);
					}
					if (ImGui::MenuItem("Cube", NULL))
					{
						SceneObject* sceneObject = _sceneManager->addSceneObject("Cube");

						Material* material = new Material;
						material->shader = SOLID_MATERIAL;
						material->shininess = 96.0f;
						material->diffuseTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();

						Cube* cube = new Cube(1, material);
						cube->init();
						_graphicsManager->addRenderObject(cube, sceneObject);
					}
					if (ImGui::MenuItem("Sphere", NULL))
					{
						SceneObject* sceneObject = _sceneManager->addSceneObject("Sphere");

						Material* material = new Material;
						material->shader = SOLID_MATERIAL;
						material->shininess = 96.0f;
						material->diffuseTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();

						SpherePrefab* sphere = new SpherePrefab(1.0f, material);
						sphere->init();
						_graphicsManager->addRenderObject(sphere, sceneObject);
					}
					if (ImGui::MenuItem("Cylinder", NULL))
					{
						SceneObject* sceneObject = _sceneManager->addSceneObject("Cylinder");

						Material* material = new Material;
						material->shader = SOLID_MATERIAL;
						material->shininess = 96.0f;
						material->diffuseTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();

						CylinderPrefab* cylinder = new CylinderPrefab(0.5f, 2.0f, material);
						cylinder->init();
						_graphicsManager->addRenderObject(cylinder, sceneObject);
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Add Bezier curve", NULL))
				{
					SceneObject* sceneObject = _sceneManager->addSceneObject("Bezier curve");

					BezierCurve* bezierCurve = _graphicsManager->addBezierCurve();
					sceneObject->addComponent(bezierCurve);

					setSelectedSceneObject(sceneObject);
				}

				if (ImGui::MenuItem("Add AI Path", NULL))
				{
					SceneObject* sceneObject = _sceneManager->addSceneObject("AI Path");

					BezierCurve* bezierCurve = _graphicsManager->addBezierCurve();
					PathComponent* pathComponent = _sceneManager->getGameLogicSystem()->addPathComponent(PD_FORWARD);
					sceneObject->addComponent(bezierCurve);
					sceneObject->addComponent(pathComponent);

					setSelectedSceneObject(sceneObject);
				}

				if (ImGui::MenuItem("Add AI Agent", NULL))
				{
					if (_selectedSceneObject != NULL)
					{
						AIAgent* aiAgent = _sceneManager->getGameLogicSystem()->addAIAgent();
						_selectedSceneObject->addComponent(aiAgent);

						aiAgent->setCurrentPath(_sceneManager->getGameLogicSystem()->getPathComponents()[0]);
					}
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Add Bus Start Point", NULL))
				{
					SceneObject* sceneObject = _sceneManager->addSceneObject("Bus start point");

					BusStartPoint* busStartPoint = _sceneManager->getGameLogicSystem()->addBusStartPoint("Start point");
					sceneObject->addComponent(busStartPoint);

					setSelectedSceneObject(sceneObject);
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Bake static shadows", NULL))
				{
					Renderer::getInstance().bakeStaticShadows();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("Camera Settings", NULL, &_showCameraSettingsWindow);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
				if (ImGui::MenuItem("Reload all shaders", NULL))
				{
					ResourceManager::getInstance().reloadAllShaders();
				}
				if (ImGui::MenuItem("Reload all textures", NULL))
				{
					ResourceManager::getInstance().reloadAllTextures();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("AABB rendering", NULL, Renderer::getInstance().getAABBFlag()))
				{
					Renderer::getInstance().toogleRenderAABBFlag();
				}
				if (ImGui::MenuItem("OBB rendering", NULL, Renderer::getInstance().getOBBFlag()))
				{
					Renderer::getInstance().toogleRenderOBBFlag();
				}
				if (ImGui::MenuItem("Alpha to coverage", NULL, Renderer::getInstance().isAlphaToCoverageEnable()))
				{
					Renderer::getInstance().setAlphaToCoverage(!(Renderer::getInstance().isAlphaToCoverageEnable()));
				}
				if (ImGui::MenuItem("Bloom", NULL, Renderer::getInstance().isBloomEnable()))
				{
					Renderer::getInstance().setBloom(!(Renderer::getInstance().isBloomEnable()));
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Tone mapping"))
				{
					ToneMappingType toneMappingType = Renderer::getInstance().getToneMappingType();
					if (ImGui::MenuItem("Reinhard", NULL, toneMappingType == TMT_REINHARD))
					{
						Renderer::getInstance().setToneMappingType(TMT_REINHARD);
					}
					if (ImGui::MenuItem("Classic", NULL, toneMappingType == TMT_CLASSIC))
					{
						Renderer::getInstance().setToneMappingType(TMT_CLASSIC);
					}
					if (ImGui::MenuItem("ACES", NULL, toneMappingType == TMT_ACES))
					{
						Renderer::getInstance().setToneMappingType(TMT_ACES);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("None", NULL, toneMappingType == TMT_NONE))
					{
						Renderer::getInstance().setToneMappingType(TMT_NONE);
					}
					if (ImGui::MenuItem("Depth", NULL, toneMappingType == TMT_DEPTH_RENDERING))
					{
						Renderer::getInstance().setToneMappingType(TMT_DEPTH_RENDERING);
					}
					ImGui::EndMenu();
				}
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


	void drawMainDockSpace()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		ImGui::Begin("DockSpace Window", nullptr, window_flags);

		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		ImGui::End();
	}


	void renderGUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		drawMainDockSpace();

		ImGuizmo::BeginFrame();
		RoadManipulator::BeginFrame();
		AxisTool::BeginFrame();

		if (_clickMode == CM_ADD_OBJECT)
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

		drawMainMenu();

		_imGuiInterface->drawOnlyWindows();

		if (_showDemoWindow)
			ImGui::ShowDemoWindow();

		if (_saveMap)
		{
			//printf("Saving map\n");
			SceneSaver sceneSaver(_sceneManager);
			sceneSaver.saveMap(mapInfo.name, mapInfo);
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
			CameraFPS* cameraFPS = dynamic_cast<CameraFPS*>(_graphicsManager->getCurrentCamera());
			if(cameraFPS)
				CameraSettingsWindow(cameraFPS);
		}

		// show selected roads
		{
			ImGui::Text("Selected roads: %d", _selectedRoads.size());
			for (int i = 0; i < _selectedRoads.size(); ++i)
			{
				ImGui::Text("%d. %s", i, _selectedRoads[i]->getSceneObject()->getName());
			}

			if (ImGui::Button("Generate polygon"))
			{
				RoadPolygonsGenerator::generatePolygon(_selectedRoads, _sceneManager);
			}
		}

		if (_showObjectPropertyEditor)
		{
			SceneGraphWindow(_sceneManager->getSceneObjects());
			showObjectProperties();
		}

		if (_showMaterialEditorWindow)
		{
			if (!materialEditorWindow())
			{
				_showMaterialEditorWindow = false;
			}
		}

		if (_showGenerateObjectsAlongCurveWindow && _selectedSceneObject != nullptr)
		{
			BezierCurve* bezierCurve = dynamic_cast<BezierCurve*>(_selectedSceneObject->getComponent(CT_BEZIER_CURVE));
			if (!generateObjectsAlongCurveWindow(bezierCurve))
			{
				_showGenerateObjectsAlongCurveWindow = false;
			}
		}

		if (_selectedSceneObject)
		{
			//ShowTransformGizmo();

			//if (_showRoadTools)
			//{
				RoadObject* roadComponent = dynamic_cast<RoadObject*>(_selectedSceneObject->getComponent(CT_ROAD_OBJECT));
				ShapePolygonComponent* shapePolygonComponent = dynamic_cast<ShapePolygonComponent*>(_selectedSceneObject->getComponent(CT_SHAPE_POLYGON));
				BezierCurve* bezierCurveComponent = dynamic_cast<BezierCurve*>(_selectedSceneObject->getComponent(CT_BEZIER_CURVE));

				if (roadComponent && roadComponent->getRoadType() != RoadType::BEZIER_CURVES)
					showRoadTools();
				else if (shapePolygonComponent)
					showPolygonEditTool();
				else if (bezierCurveComponent)
					showBezierCurveTool();
				else
					ShowTransformGizmo();
			//}
		}


		glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), _camera->getDirection(), _camera->getUpVector());

		float width = _camera->getWindowWidth() / 7.0f;
		float height = _camera->getWindowHeight() / 7.0f;
		float x = 150.0f;
		float y = _camera->getWindowHeight() - height;
		AxisTool::SetRect(x, y, width, height);
		AxisTool::Show(viewMatrix, _camera->getProjectionMatrix(), _camera->getDirection());

		LoggerWindow::drawWindow();

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

			OGLDriver::getInstance().update();

			// fixed update
			while (accumulator > TIME_STEP)
			{
				accumulator -= TIME_STEP;

				_graphicsManager->update(TIME_STEP);

				_sceneManager->getGameLogicSystem()->update(deltaTime);
			}


			// rendering
			renderer.renderAll();
			renderGUI();

			window.swapBuffers();
			window.updateEvents();


			if (_isLoading && _loadingSceneFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
			{
				swapScenes(_newSceneManager);
				_isLoading = false;
				_newSceneManager = nullptr;
			}
		}
	}

	void shutDown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		delete _imGuiInterface;

		_physicsManager->stop();

		_soundManager->drop();
		_physicsManager->drop();
		delete _sceneManager;

		if (_backgroundWindow != nullptr)
		{
			delete _backgroundWindow;
		}
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
		ImGui::Checkbox("##usesnapCheckBox", &useSnap);

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

		glm::mat4 viewMatrix = _camera->getViewMatrix();
		if (_selectedSceneObject != nullptr && _selectedSceneObject->getParent() != nullptr)
		{
			viewMatrix = viewMatrix * _selectedSceneObject->getParent()->getGlobalTransformMatrix();
		}

		glm::mat4 modelMatrix = _selectedSceneObject->getLocalTransformMatrix();

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(_camera->getProjectionMatrix()),
			mCurrentGizmoOperation, mCurrentGizmoMode,
			glm::value_ptr(modelMatrix),
			NULL,
			useSnap ? &snap[0] : NULL,
			boundSizing ? bounds : NULL,
			boundSizingSnap ? boundsSnap : NULL
		);

		if (ImGuizmo::IsUsing())
		{
			_selectedSceneObject->setTransformFromMatrix(modelMatrix);
		}

		Component* roadIntersectionComponent = _selectedSceneObject->getComponent(CT_ROAD_INTERSECTION);
		if (roadIntersectionComponent != nullptr)
		{
			if (ImGuizmo::IsUsing())
			{
				dynamic_cast<RoadIntersectionComponent*>(roadIntersectionComponent)->needRebuildConnectedRoad();
			}
		}
	}

	void createAvailableConnectionPointsList(std::vector<glm::vec3>& connectionPointsPositions, std::vector<RoadConnectionPoint>& connectionPoints)
	{
		const std::vector<CrossroadComponent*>& crossroadComponents = _graphicsManager->getCrossroadComponents();
		for (CrossroadComponent* crossroadComponent : crossroadComponents)
		{
			float distance = glm::length(crossroadComponent->getSceneObject()->getPosition() - _camera->getPosition());
			if (distance < 500.0f) // todo: value
			{
				for (int i = 0; i < crossroadComponent->getConnectionsCount(); ++i)
				{
					connectionPointsPositions.push_back(crossroadComponent->getGlobalPositionOfConnectionPoint(i));
					connectionPoints.push_back(RoadConnectionPoint(crossroadComponent, i));
				}
			}
		}

		const std::vector<RoadIntersectionComponent*>& roadIntersectionComponents = _graphicsManager->getRoadIntersectionComponents();
		for (RoadIntersectionComponent* roadIntersectionComponent : roadIntersectionComponents)
		{
			float distance = glm::length(roadIntersectionComponent->getSceneObject()->getPosition() - _camera->getPosition());
			if (distance < 500.0f) // todo: value
			{
				connectionPointsPositions.push_back(roadIntersectionComponent->getSceneObject()->getPosition());
				connectionPoints.push_back(RoadConnectionPoint(roadIntersectionComponent));
			}
		}
	}

	struct PathConnectionPoint final
	{
		PathComponent* pathComponent;
		int index;
	};

	void createAvailableConnectionPointsListForPathComponent(PathComponent* selectedPathComponent, int selectedPointIndex, std::vector<glm::vec3>& connectionPointsPositions, std::vector<PathConnectionPoint>& connectionPoints)
	{
		const auto& pathComponents = _sceneManager->getGameLogicSystem()->getPathComponents();
		for (auto path : pathComponents)
		{
			if (path->getCurvePoints().size() > 0 && selectedPointIndex == 1)
			{
				connectionPointsPositions.push_back(path->getCurvePoints()[0]);
				connectionPoints.push_back({ path, 0 });
			}
			if (path->getCurvePoints().size() > 1 && selectedPointIndex == 0)
			{
				connectionPointsPositions.push_back(path->getCurvePoints()[path->getCurvePoints().size() - 1]);
				connectionPoints.push_back({ path, 1 });
			}
		}
	}

	void showRoadTools()
	{
		RoadObject* roadComponent = dynamic_cast<RoadObject*>(_selectedSceneObject->getComponent(CT_ROAD_OBJECT));

		std::vector<glm::vec3> connectionPointsPositions;
		std::vector<RoadConnectionPoint> connectionPoints;
		createAvailableConnectionPointsList(connectionPointsPositions, connectionPoints);

		ImGuiIO& io = ImGui::GetIO();
		RoadManipulator::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		RoadManipulator::SetAvailableConnectionPoints(&connectionPointsPositions);
		RoadManipulator::SetCurvePoints({});
		RoadManipulator::Manipulate(_camera->getViewMatrix(), _camera->getProjectionMatrix(),
			_selectedSceneObject->getLocalTransformMatrix(),
			_camera->getPosition(),
			roadComponent->getPoints(),
			roadComponent->getSegments(),
			static_cast<RoadManipulator::RoadType>(roadComponent->getRoadType()));

		roadActiveSegment = RoadManipulator::GetActiveSegment();
		roadActivePoint = RoadManipulator::GetActivePoint();
		if (RoadManipulator::IsModified())
		{
			roadComponent->setPointPostion(RoadManipulator::GetModifiedPointIndex(), RoadManipulator::GetModifiedPointNewPostion());
		}
		if (RoadManipulator::IsCreatedNewConnection())
		{
			int connectionPointIndex = RoadManipulator::GetModifiedPointIndex() == 0 ? 0 : 1;
			int newConnectionIndex = RoadManipulator::GetNewConnectionIndex();

			if (connectionPoints[newConnectionIndex].crossroadComponent != nullptr)
			{
				roadComponent->setConnectionPoint(connectionPointIndex, connectionPoints[newConnectionIndex].crossroadComponent, connectionPoints[newConnectionIndex].index);
			}
			else
			{
				roadComponent->setConnectionPointWithRoadIntersection(connectionPointIndex, connectionPoints[newConnectionIndex].roadIntersectionComponent);
			}
		}
	}

	void showPolygonEditTool()
	{
		ShapePolygonComponent* component = dynamic_cast<ShapePolygonComponent*>(_selectedSceneObject->getComponent(CT_SHAPE_POLYGON));

		std::vector<RoadSegment> segments;
		std::vector<glm::vec3> connectionPointsPositions;

		ImGuiIO& io = ImGui::GetIO();
		RoadManipulator::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		RoadManipulator::SetAvailableConnectionPoints(&connectionPointsPositions);
		RoadManipulator::SetCurvePoints(component->getPoints());
		RoadManipulator::Manipulate(_camera->getViewMatrix(), _camera->getProjectionMatrix(),
			/*_selectedSceneObject->getLocalTransformMatrix(),*/
			glm::mat4(1.0f),
			_camera->getPosition(),
			component->getPoints(),
			segments,
			RoadManipulator::RoadType::POLYGON);

		if (RoadManipulator::IsModified())
		{
			component->setPointPostion(RoadManipulator::GetModifiedPointIndex(), RoadManipulator::GetModifiedPointNewPostion());
		}
	}

	void showBezierCurveTool()
	{
		BezierCurve* component = dynamic_cast<BezierCurve*>(_selectedSceneObject->getComponent(CT_BEZIER_CURVE));
		RoadObject* roadObject = dynamic_cast<RoadObject*>(_selectedSceneObject->getComponent(CT_ROAD_OBJECT));
		PathComponent* pathComponent = dynamic_cast<PathComponent*>(_selectedSceneObject->getComponent(CT_PATH));

		std::vector<RoadSegment> segments;
		std::vector<glm::vec3> connectionPointsPositions;
		std::vector<RoadConnectionPoint> roatConnectionPoints;
		std::vector<PathConnectionPoint> pathConnectionPoints;
		if (roadObject != nullptr)
		{
			createAvailableConnectionPointsList(connectionPointsPositions, roatConnectionPoints);
		}
		if (pathComponent != nullptr)
		{
			int connectionPointIndex = RoadManipulator::GetModifiedPointIndex() == 0 ? 0 : 1;
			createAvailableConnectionPointsListForPathComponent(pathComponent, connectionPointIndex, connectionPointsPositions, pathConnectionPoints);
		}

		ImGuiIO& io = ImGui::GetIO();
		RoadManipulator::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		RoadManipulator::SetAvailableConnectionPoints(&connectionPointsPositions);
		RoadManipulator::SetCurvePoints(component->getCurvePoints());
		RoadManipulator::Manipulate(_camera->getViewMatrix(), _camera->getProjectionMatrix(),
			/*_selectedSceneObject->getLocalTransformMatrix(),*/
			glm::mat4(1.0f),
			_camera->getPosition(),
			component->getPoints(),
			segments,
			RoadManipulator::RoadType::BEZIER_CURVE);

		if (RoadManipulator::IsModified())
		{
			component->setPointPostion(RoadManipulator::GetModifiedPointIndex(), RoadManipulator::GetModifiedPointNewPostion());
		}

		if (RoadManipulator::IsCreatedNewConnection())
		{
			int connectionPointIndex = RoadManipulator::GetModifiedPointIndex() == 0 ? 0 : 1;
			int newConnectionIndex = RoadManipulator::GetNewConnectionIndex();

			if (roadObject != nullptr)
			{
				if (roatConnectionPoints[newConnectionIndex].crossroadComponent != nullptr)
				{
					roadObject->setConnectionPoint(connectionPointIndex, roatConnectionPoints[newConnectionIndex].crossroadComponent, roatConnectionPoints[newConnectionIndex].index);
				}
				else
				{
					roadObject->setConnectionPointWithRoadIntersection(connectionPointIndex, roatConnectionPoints[newConnectionIndex].roadIntersectionComponent);
				}
			}

			if (pathComponent != nullptr)
			{
				pathComponent->setConnection(connectionPointIndex, pathConnectionPoints[newConnectionIndex].pathComponent, pathConnectionPoints[newConnectionIndex].index);
			}
		}
	}

} // namespace