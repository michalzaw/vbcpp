#include "Editor.h"

#include "../Bus/BusLoader.h"


void editorKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}


void editorCharCallback(GLFWwindow* window, unsigned int c)
{

}


Editor::Editor()
{
    if (!createWindow())
    {
        return;
    }

    initializeEngineSubsystems();

    _editorGUI.reset(new EditorGUI(_window, _sceneManager));
}


Editor::~Editor()
{
    _physicsManager->stop();

    _soundManager->drop();
    _physicsManager->drop();
    delete _sceneManager;
}


bool Editor::createWindow()
{
    _window = std::make_shared<Window>();
    if (!_window->createWindow(1024, 768, 10, 40, false))
    {
        return false;
    }
    _window->setWindowTitle("vbcpp - editor");

    glfwSetKeyCallback(_window->getWindow(), editorKeyCallback);
    glfwSetCharCallback(_window->getWindow(), editorCharCallback);

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

    SceneObject* camera = _sceneManager->addSceneObject("cam1");
    CameraFPS* camFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000);
    camera->addComponent(camFPS);
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(8.0f);

    GraphicsManager::getInstance().setCurrentCamera(camFPS);
    _soundManager->setActiveCamera(camFPS);

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


void Editor::run()
{
    Renderer& renderer = Renderer::getInstance();
    while (_window->isOpened())
    {
        renderer.renderAll();

        _editorGUI->draw();

        _window->swapBuffers();
        _window->updateEvents();
    }
}
