#include <iostream>
#include <ctime>

#include "Window/Window.h"

#include "Graphics/GraphicsManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/OGLDriver.h"
#include "Graphics/LoadTexture.h"

#include "Physics/PhysicsManager.hpp"

#include "Scene/SceneManager.h"
#include "Scene/SoundManager.h"

#include "Utils/ResourceManager.h"
#include "Utils/Timer.h"
#include "Utils/Helpers.hpp"
#include "Utils/Math.h"

#include "Bus/BusConstraint.h"
#include "Bus/BusRaycast.h"

#include "GUI/GUIManager.h"

#include "Game/GameConfig.h"
#include "Game/BusStopSystem.h"


// Definicje globalne
CameraFPS* camFPS;

Window* win;
PhysicsManager* physMgr = NULL;
SceneManager* sceneMgr = NULL;
SoundManager* sndMgr = NULL;

SceneObject* dirLight = NULL;

Bus* bus = NULL;

std::string winTitle = "Virtual Bus Core++";


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        win->setCloseFlag();

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		bus->setIsEnableHeadlights(!bus->isEnableHeadlights());
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		bus->setIsEnableLights(!bus->isEnableLights());
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
	    Light* l = dynamic_cast<Light*>(dirLight->getComponent(CT_LIGHT));

	    if (l->getDiffiseIntenisty() > 0.05)
        {
            l->setAmbientIntensity(0.05);
            l->setDiffuseIntensity(0.0);
        }
        else
        {
            l->setAmbientIntensity(0.5);
            l->setDiffuseIntensity(0.5);
        }

	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        bus->doorOpenClose(1);
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        bus->doorOpenClose(2);
    }

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (camFPS->getSceneObject()->hasParent())
        {
            camFPS->getSceneObject()->setPosition(camFPS->getPosition());
            camFPS->getSceneObject()->removeParent();
        }
        else
        {
            bus->getSceneObject()->addChild(camFPS->getSceneObject());
            camFPS->getSceneObject()->setPosition(bus->getDriverPosition());
            camFPS->getSceneObject()->setRotation(0,0,0);
        }
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        bus->getGearbox()->shiftUp();

  	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
        bus->getGearbox()->shiftDown();

  	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        if (!bus->getEngine()->isRunning())
        {
            bus->startEngine();
        }
        else
        {
            bus->stopEngine();
        }
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if (!physMgr->isRunning())
            physMgr->play();
        else
            physMgr->stop();
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        bus->toggleHandbrake();
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        ResourceManager::getInstance().reloadAllShaders();
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        ResourceManager::getInstance().reloadAllTextures();
    }
}


// Callback dla pojedynczych zdarzeń - przyciski myszy
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}


// Bufforowane przetwarzanie zdarzeń
void readInput(GLFWwindow* window, double deltaTime)
{
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS)
	{
		camFPS->moveForward(deltaTime);
	}

	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS)
	{
		camFPS->moveBackward(deltaTime);
	}

	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS)
	{
		camFPS->strafeRight(deltaTime);
	}

	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS)
	{
		camFPS->strafeLeft(deltaTime);
	}

	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
    {
        bus->turnLeft(deltaTime);
    }

    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
    {
        bus->turnRight(deltaTime);
    }

    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS)
    {
        bus->accelerate();
    }

    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_RELEASE)
    {
        bus->idle();
    }

    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
    {
        bus->brakeOn();
    }

    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_RELEASE)
    {
        bus->brakeOff();
    }

    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_RELEASE && glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_RELEASE)
    {
        bus->centerSteringWheel(deltaTime);
    }
}


// ### MAIN ###
int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    GameConfig::getInstance().loadGameConfig("game.xml");

    win = new Window;
    win->createWindow(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, 10, 40, GameConfig::getInstance().isFullscreen);
    win->setWindowTitle(winTitle);

    // Callbacki do obslugi zdarzen
    glfwSetKeyCallback(win->getWindow(), key_callback);
    glfwSetMouseButtonCallback(win->getWindow(), mouse_button_callback);

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicjalizujemy potrzebne rzeczy
    OGLDriver::getInstance().initialize();

    physMgr = new PhysicsManager;
    sndMgr = new SoundManager();
    sndMgr->setMute(true);
	sceneMgr = new SceneManager(physMgr, sndMgr);


	Renderer& renderer = Renderer::getInstance();
	renderer.setIsShadowMappingEnable(GameConfig::getInstance().isShadowmappingEnable);
	renderer.init(win->getWidth(), win->getHeight());


	GraphicsManager::getInstance().setWindDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	GraphicsManager::getInstance().setWindVelocity(0.6f);


    if (GameConfig::getInstance().busModel == "h9_raycast" || GameConfig::getInstance().busModel == "MAN")
        bus = new BusRaycast(sceneMgr, physMgr, sndMgr, GameConfig::getInstance().busModel);
    else
        bus = new BusConstraint(sceneMgr, physMgr, sndMgr, GameConfig::getInstance().busModel);

    sceneMgr->loadScene(GameConfig::getInstance().mapFile);

    bus->getSceneObject()->setPosition(sceneMgr->getBusStart().position);
    bus->getSceneObject()->setRotation(degToRad(sceneMgr->getBusStart().rotation.x),
                                       degToRad(sceneMgr->getBusStart().rotation.y),
                                       degToRad(sceneMgr->getBusStart().rotation.z));

    // Camera FPS
    SceneObject* Camera = sceneMgr->addSceneObject("cam1");
    camFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000);
    Camera->addComponent(camFPS);
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(8.0f);
    Camera->setRotation(0,degToRad(-90), 0);
    Camera->setPosition(10,7,-10);
    GraphicsManager::getInstance().setCurrentCamera(camFPS);

    // Light
    dirLight = sceneMgr->addSceneObject("light");
    Light* lightComponent = GraphicsManager::getInstance().addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f);
    dirLight->addComponent(lightComponent);
    dirLight->setRotation(glm::vec3(-0.2f * PI, 0.35f * PI, 0));
    dirLight->setPosition(glm::vec3(0,0,0));
    lightComponent->setShadowMapping(GameConfig::getInstance().isShadowmappingEnable);


    const char* skyboxTextures[] = {"Skybox/rt.bmp", "Skybox/lt.bmp", "Skybox/up.bmp", "Skybox/dn.bmp", "Skybox/ft.bmp", "Skybox/bk.bmp"};
    sceneMgr->addSky(loadTextureCubeMap(skyboxTextures, true));


    GUIManager* gui = new GUIManager;
    Image* img = gui->addImage(ResourceManager::getInstance().loadTexture("opengl_logo.png"));
    img->setPosition(0, GameConfig::getInstance().windowHeight - img->getTexture()->getSize().y / 2.0f);
    img->setScale(0.3f, 0.3f);

    RFont* font = ResourceManager::getInstance().loadFont("fonts/arial.ttf");
    Label* label = gui->addLabel(font, winTitle);
    label->setPosition(10, GameConfig::getInstance().windowHeight - 20);
    label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    label->scale(0.6f, 0.6f);

    Label* labelEngineIsRunning = gui->addLabel(font, "Engine off");
    labelEngineIsRunning->setPosition(10, 70);
    labelEngineIsRunning->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelEngineIsRunning->scale(0.6f, 0.6f);

    Label* labelGearRatio = gui->addLabel(font, "0");
    labelGearRatio->setPosition(10, 55);
    labelGearRatio->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelGearRatio->scale(0.6f, 0.6f);

    Label* labelRPM = gui->addLabel(font, "0");
    labelRPM->setPosition(10, 40);
    labelRPM->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelRPM->scale(0.6f, 0.6f);

    Label* labelThrottle = gui->addLabel(font, "0");
    labelThrottle->setPosition(10, 25);
    labelThrottle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelThrottle->scale(0.6f, 0.6f);

    Label* labelTorque = gui->addLabel(font, "0");
    labelTorque->setPosition(10, 10);
    labelTorque->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelTorque->scale(0.6f, 0.6f);

    Label* labelBusStop = gui->addLabel(font, "");
    labelBusStop->setPosition(450, 730);
    labelBusStop->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelBusStop->scale(0.6f, 0.6f);

    Label* labelBusStop2 = gui->addLabel(font, "");
    labelBusStop2->setPosition(450, 705);
    labelBusStop2->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelBusStop2->scale(0.6f, 0.6f);

    Label* labelPassengers = gui->addLabel(font, "Liczba pasazerow: " + toString(bus->getNumberOfPassengers()));
    labelPassengers->setPosition(10, 105);
    labelPassengers->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelPassengers->scale(0.6f, 0.6f);

    Label* labelPassengersGettingOff = gui->addLabel(font, "Liczba pasazerow wysiadajacych: " + toString(bus->getNumberOfPassengersGettingOff()));
    labelPassengersGettingOff->setPosition(10, 90);
    labelPassengersGettingOff->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelPassengersGettingOff->scale(0.6f, 0.6f);


    sndMgr->setActiveCamera(camFPS);

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

    double xpos, ypos;

    int nbFrames = 0;

    physMgr->play();
    sndMgr->setMute(false);

    // ========== MAIN LOOP START ==========
    while (win->isOpened())
    {
        nbFrames++;


        timePhysicsCurr = glfwGetTime();
        deltaTime = timePhysicsCurr - timePhysicsPrev;
        timePhysicsPrev = timePhysicsCurr;

        deltaTime = std::max(0.0, deltaTime);
        accumulator += deltaTime;
        accumulator = clamp(accumulator, 0.0, MAX_ACCUMULATED_TIME);

        //create string from frame/sec and display it in window title
        if ( timePhysicsCurr - lastFPSupdate >= 1.0f )
        {
            // Convert the fps value into a string using an output stringstream
			std::string sTiming = toString(nbFrames);

			// Append the FPS value to the window title details
			std::string newWindowTitle = winTitle + " | FPS: " + sTiming;
			win->setWindowTitle(newWindowTitle);
			//label->setText(newWindowTitle);

            nbFrames = 0;
            lastFPSupdate += 1.0f;
        }


        glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
        glfwSetCursorPos(win->getWindow(), win->getWidth()/2, win->getHeight()/2);

        camFPS->setRotation(xpos, ypos);


        readInput(win->getWindow(), deltaTime);


        while ( accumulator > TIME_STEP )
        {
            physMgr->simulate(TIME_STEP);
            bus->update(TIME_STEP);
            GraphicsManager::getInstance().update(TIME_STEP);
            BusStopSystem::getInstance().update(TIME_STEP, bus);

            accumulator -= TIME_STEP;
        }

        sndMgr->update();

        labelEngineIsRunning->setText(bus->getEngine()->isRunning() ? "Engine on" : "Engine off");
        labelGearRatio->setText("Gear ratio: " + toString(bus->getGearbox()->currentRatio()));
        labelRPM->setText("RPM: " + toString(bus->getEngine()->getCurrentRPM()));
        labelThrottle->setText("Throttle: " + toString(bus->getEngine()->getThrottle()));
        labelTorque->setText("Torque: " + toString(bus->getEngine()->getCurrentTorque()));

        BusStopSystem& busStopSystem = BusStopSystem::getInstance();
        if (busStopSystem.getCurrentBusStop() != NULL)
        {
            labelBusStop->setText(busStopSystem.getCurrentBusStop()->getName() + " (" + toString((int)busStopSystem.getDistanceToCurrentBusStop()) + "m)");
            labelBusStop2->setText("Liczba pasazerow: " + toString(busStopSystem.getCurrentBusStop()->getNumberOfPassengers()));

            labelPassengers->setText("Liczba pasazerow w autobusie: " + toString(bus->getNumberOfPassengers()));
            labelPassengersGettingOff->setText("Liczba pasazerow wysiadajacych: " + toString(bus->getNumberOfPassengersGettingOff()));
        }
        else
        {
            labelBusStop->setText("");
            labelBusStop2->setText("");
        }

        // Render the scene
        renderer.renderAll();

        // Render GUI
        renderer.renderGUI(gui->getGUIRenderList());

        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    physMgr->stop();

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    delete gui;

    bus->drop();

    sndMgr->drop();
    physMgr->drop();
    delete sceneMgr;

    delete win;

    return 0;
}
