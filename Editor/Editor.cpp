#include "Editor.h"

#include "../Bus/BusLoader.h"

#include "../Utils/RaycastingUtils.h"


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
    : _cameraActive(false)
{
    if (!createWindow())
    {
        return;
    }

    initializeEngineSubsystems();

    _editorGUI.reset(new EditorGUI(_window, _sceneManager));

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
    if (!_window->createWindow(1024, 768, 10, 40, false, false))
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
    renderer.init(_window->getWidth(), _window->getHeight());

    _physicsManager = new PhysicsManager;
    _soundManager = new SoundManager;
    _sceneManager = new SceneManager(_physicsManager, _soundManager);

    _cameraObject = _sceneManager->addSceneObject("cam1");
    _camera = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000);
    _cameraObject->addComponent(_camera);
    _camera->setRotationSpeed(0.01f);
    _camera->setMoveSpeed(8.0f);

    GraphicsManager::getInstance().setCurrentCamera(_camera);
    _soundManager->setActiveCamera(_camera);

    SceneObject* dirLight = _sceneManager->addSceneObject("light");
    Light* lightComponent = GraphicsManager::getInstance().addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);
    dirLight->addComponent(lightComponent);
    dirLight->setRotation(glm::vec3(-0.2f * PI, 0.35f * PI, 0));
    dirLight->setPosition(glm::vec3(0,0,0));
    lightComponent->setShadowMapping(false);

    // ---------------------------------------------------
    std::string skyboxTextures[] = {"Skybox/rt.bmp", "Skybox/lt.bmp", "Skybox/up.bmp", "Skybox/dn.bmp", "Skybox/ft.bmp", "Skybox/bk.bmp"};
    RTextureCubeMap* skyboxTexture = ResourceManager::getInstance().loadTextureCubeMap(skyboxTextures);
    _sceneManager->addSky(skyboxTexture);
    GraphicsManager::getInstance().addGlobalEnvironmentCaptureComponent(ResourceManager::getInstance().loadTextureCubeMap(skyboxTextures));

	BusLoader busLoader(_sceneManager, _physicsManager, _soundManager);
    Bus* bus = busLoader.loadBus("h9_raycast");
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


        // fixed update
        while ( accumulator > TIME_STEP )
        {
            accumulator -= TIME_STEP;
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

        // collision with render objects
        std::list<RenderObject*>& renderObjects = GraphicsManager::getInstance().getRenderObjects();
        for (std::list<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i)
        {
            RenderObject* renderObject = *i;
            AABB* aabb = renderObject->getModel()->getAABB();
            glm::mat4 modelMatrix = renderObject->getSceneObject()->getGlobalTransformMatrix();
            float distance;
            if (isRayIntersectOBB(rayStart, rayDir, *aabb, modelMatrix, distance))
            {
                if (distance > 0.0f)
                {
                    _editorGUI->setSelectedSceneObject(renderObject->getSceneObject());
                }
            }
        }
    }
}


void Editor::keyCallback(int key, int scancode, int action, int mods)
{

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
    glViewport(0, 0, width, height);
}
