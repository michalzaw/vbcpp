#include "Editor.h"

#include "../Bus/BusLoader.h"

#include "../Game/Directories.h"

#include "../Scene/SceneLoader.h"
#include "../Scene/SceneSaver.h"

#include "../Utils/RaycastingUtils.h"
#include "../Utils/FilesHelper.h"


std::list<Editor*> editorInstances;


void editorMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    {
        if ((*i)->getWindow()->getWindow() == window)
        {
            (*i)->mouseButtonCallback(button, action, mods);
            break;
        }
    }
}


void editorKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    {
        if ((*i)->getWindow()->getWindow() == window)
        {
            (*i)->keyCallback(key, scancode, action, mods);
            break;
        }
    }
}


void editorCharCallback(GLFWwindow* window, unsigned int c)
{
    for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    {
        if ((*i)->getWindow()->getWindow() == window)
        {
            (*i)->charCallback(c);
            break;
        }
    }
}


void editorChangeWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    {
        if ((*i)->getWindow()->getWindow() == window)
        {
            (*i)->changeWindowSizeCallback(width, height);
            break;
        }
    }
}


void editorFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    for (std::list<Editor*>::iterator i = editorInstances.begin(); i != editorInstances.end(); ++i)
    {
        if ((*i)->getWindow()->getWindow() == window)
        {
            (*i)->changeFramebufferSizeCallback(width, height);
            break;
        }
    }
}


Editor::Editor()
    : _cameraActive(false),
	_addObjectMode(false), _objectToAdd(nullptr)
{
    if (!createWindow())
    {
        return;
    }

    initializeEngineSubsystems();

	_editorContext = std::make_shared<EditorContext>();
	_editorContext->availableObjects = FilesHelper::getDirectoriesList(GameDirectories::OBJECTS);

    _editorGUI.reset(new EditorGUI(_window, _sceneManager, _editorContext));

    editorInstances.push_back(this);
}


Editor::~Editor()
{
    _physicsManager->stop();

    _soundManager->drop();
    _physicsManager->drop();
    delete _sceneManager;

    editorInstances.remove(this);
}


bool Editor::createWindow()
{
    _window = std::make_shared<Window>();
    if (!_window->createWindow(1024, 768, 10, 40, false, true))
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

    _cameraObject = _sceneManager->addSceneObject("cam1");
    _camera = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000);
    _cameraObject->addComponent(_camera);
    _camera->setRotationSpeed(0.01f);
    _camera->setMoveSpeed(8.0f);

    GraphicsManager::getInstance().setCurrentCamera(_camera);
    _soundManager->setActiveCamera(_camera);

    _axisObject = _sceneManager->addSceneObject("axis");
    RStaticModel* axisModel = ResourceManager::getInstance().loadModel("axis.fbx", "");
    axisModel->getMaterial(0)->shader = EDITOR_AXIS_SHADER;
    RenderObject* axisRenderObject = new RenderObject(axisModel);
    GraphicsManager::getInstance().addRenderObject(axisRenderObject, _axisObject);

    // ---------------------------------------------------
	/*BusLoader busLoader(_sceneManager, _physicsManager, _soundManager);
    Bus* bus = busLoader.loadBus("h9_raycast");*/
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


std::shared_ptr<Window> Editor::getWindow()
{
    return _window;
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
					_editorContext->currentMapInfo.name = event.params;
					_editorContext->currentMapInfo.author = sceneLoader.getLoadedSceneDescription().author;
                    break;

				case EET_SAVE_CLICK:
					sceneSaver.saveMap(event.params, SceneDescription(_editorContext->currentMapInfo.author));
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

            if (_editorGUI->getSelectedSceneObject() != nullptr)
            {
                _axisObject->setPosition(_editorGUI->getSelectedSceneObject()->getPosition());
            }
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

				_editorGUI->setSelectedSceneObject(newObject);
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

			_editorGUI->setSelectedSceneObject(selectedObject);
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


void Editor::changeFramebufferSizeCallback(int width, int height)
{
    // TODO: change size all render targets in Renderer
    Renderer::getInstance().setWindowDimensions(width, height);

}
