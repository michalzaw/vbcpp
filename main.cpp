#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <ctime>

#include "Utils/Helpers.hpp"

#include "Window/Window.h"

#include "Graphics/Model.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/RShader.h"
#include "Graphics/Renderer.h"
#include "Graphics/OGLDriver.h"
#include "Graphics/Load3ds.h"
#include "Graphics/Vector3.h"

#include "Physics/PhysicsManager.hpp"
#include "Physics/PhysicalBodyBox.hpp"
#include "Physics/ConstraintHinge.hpp"
#include "Physics/ConstraintHinge2.hpp"
#include "Physics/PhysicalBodyBvtTriangleMesh.hpp"

#include "Scene/SceneManager.h"
#include "Scene/SoundManager.h"

#include "Utils/ResourceManager.h"

#include "Scene/Bus.h"

// XML reader
#include "Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "Utils/Math.h"

#include "GUI/GUIManager.h"
#include "Graphics/Prefab.h"
#include "Graphics/Roads.h"

// Definicje globalne

bool MOUSE_RIGHT_BUTTON_PRESSED = false;

int W_WIDTH = 1024;
int W_HEIGHT = 768;

CameraFPS* camFPS;
double oldMouseX, oldMouseY;

Window* win;
PhysicsManager* physMgr;
SceneManager* sceneMgr;
SoundManager* sndMgr = 0;

SoundComponent* soundComp = 0;

SceneObject* point, *point2, *point3, *dirLight, *spot = 0;

Bus* bus = 0;

Label* engineIsRunning = 0;

std::string winTitle = "Virtual Bus Core++";


struct GameConfig
{
    GameConfig()
    : windowWidth(1024), windowHeight(768)
    {

    }

    int windowWidth;
    int windowHeight;
    std::string mapFile;
    std::string mapTexPath;
    std::string busModel;
} gameCfg;


// kod

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS)
	{
		//point3->setIsActive(!point3->isActive());
		bus->setIsEnableHeadlights(!bus->isEnableHeadlights());

	}

	if (glfwGetKey( window, GLFW_KEY_K ) == GLFW_PRESS)
	{
		bus->setIsEnableLights(!bus->isEnableLights());

	}

	if (glfwGetKey( window, GLFW_KEY_H ) == GLFW_PRESS)
	{
	    Light* l = dynamic_cast<Light*>(dirLight->getComponent(0));

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

	if (glfwGetKey( window, GLFW_KEY_U ) == GLFW_PRESS)
	{
		spot->rotate(0, 0, 0.01);
	}

	if (glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS)
	{
		spot->rotate(0, 0, -0.01);
	}

	if (glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS)
	{
		spot->rotate(0, 0.01, 0);
	}

	if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS)
	{
		spot->rotate(0, -0.01, 0);
	}


	if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS)
    {
        bus->doorOpenClose(1);
    }


    if (glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS)
    {
        bus->doorOpenClose(2);
    }


	if (glfwGetKey( window, GLFW_KEY_C ) == GLFW_PRESS)
	{
		if (camFPS->getSceneObject()->hasParent())
        {
            //camFPS->getTransform()->setPosition(camFPS->getPosition());
            camFPS->getSceneObject()->setPosition(camFPS->getPosition());
            camFPS->getSceneObject()->removeParent();
        }
        else
        {
            bus->getSceneObject()->addChild(camFPS->getSceneObject());
            camFPS->getSceneObject()->setPosition(bus->getDriverPosition());
        }

	}

	if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS)
        bus->getGearbox()->shiftUp();

  	if (glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS)
        bus->getGearbox()->shiftDown();

  	if (glfwGetKey( window, GLFW_KEY_0 ) == GLFW_PRESS)
    {
        if (!bus->getEngine()->isRunning())
        {
            //bus->getEngine()->turnOn();
            bus->startEngine();
            engineIsRunning->setText("Engine on");
            //soundComp->play();
        }
        else
        {
            //bus->getEngine()->turnOff();
            bus->stopEngine();
            engineIsRunning->setText("Engine off");
            //soundComp->stop();
        }
    }

    if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS)
    {
        if (!physMgr->isRunning())
            physMgr->play();
        else
            physMgr->stop();
    }

    if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS)
    {
        SceneObject* zapora = sceneMgr->getSceneObject("zapora");

        if (zapora)
        {
            zapora->setPosition(glm::vec3(0,3,0));
            zapora->setRotation(glm::vec3(PI / 2.0,0,0));
        }

    }

    if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS)
    {
        if (bus)
        {
            bus->getSceneObject()->setRotation(0, PI/2, 0);
            bus->getSceneObject()->setPosition(glm::vec3(0,3,0));
        }
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
}


void loadScene()
{
    std::string filename("game.xml");

    XMLDocument doc;
    doc.LoadFile( filename.c_str() );

    XMLElement* objElement = doc.FirstChildElement("Game");

    for (XMLElement* child = objElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        const char* ename = child->Name();

        // OBJECT TRANSFORM
        if (strcmp(ename,"Resolution") == 0)
        {
            const char* cWidth = child->Attribute("x");
            const char* cHeight = child->Attribute("y");

            gameCfg.windowWidth = (int)atoi(cWidth);
            gameCfg.windowHeight = (int)atoi(cHeight);
        }
        else
        if (strcmp(ename,"Map") == 0)
        {
            gameCfg.mapFile = std::string(child->Attribute("name"));;
            //gameCfg.mapTexPath = std::string(child->Attribute("texturePath"));
        }
        else
        if (strcmp(ename,"Bus") == 0)
        {
            gameCfg.busModel = std::string(child->Attribute("model"));
        }
    }
}


// ### MAIN ###
int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    loadScene();

    win = new Window;

    win->createWindow(gameCfg.windowWidth, gameCfg.windowHeight, 10, 40);
    win->setWindowTitle(winTitle);

    // Callbacki do obslugi zdarzen
    glfwSetKeyCallback(win->getWindow(), key_callback);
    glfwSetMouseButtonCallback(win->getWindow(), mouse_button_callback);

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicjalizujemy potrzebne rzeczy
    OGLDriver::getInstance().initialize();
    //PhysicsManager::getInstance().createPhysicsWorld();

    physMgr = new PhysicsManager;
    sndMgr = new SoundManager();
	sceneMgr = new SceneManager(physMgr, sndMgr);



	Renderer* renderer = new Renderer(win->getWidth(), win->getHeight());


	GraphicsManager::getInstance().setWindDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	GraphicsManager::getInstance().setWindVelocity(0.6f);


    bus = new Bus(sceneMgr, physMgr, sndMgr, gameCfg.busModel);

    sceneMgr->loadScene(gameCfg.mapFile);

    // Camera FPS
    SceneObject* Camera = sceneMgr->addSceneObject("cam1");
    camFPS = GraphicsManager::getInstance().addCameraFPS(gameCfg.windowWidth, gameCfg.windowHeight, 45.0f, 0.1f, 1000);
    Camera->addComponent(camFPS);
    Camera->setPosition(glm::vec3(0,4,5));
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(8.0f);

    // Light
    dirLight = sceneMgr->addSceneObject("light");
    dirLight->addComponent( GraphicsManager::getInstance().addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f));
    dirLight->setRotation(glm::vec3(-0.2f * PI, 0.5f * PI, 0));
    dirLight->setPosition(glm::vec3(0,20,0));

    point = sceneMgr->addSceneObject("point1");
    point->addComponent(GraphicsManager::getInstance().addPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    //point->getTransform()->setPosition(glm::vec3(-10, 4.5, 20));
    point->setPosition(glm::vec3(0, 4.0, 5));
    point->setIsActive(false);

    point2 = sceneMgr->addSceneObject("point2");
    point2->addComponent(GraphicsManager::getInstance().addPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    //point2->getTransform()->setPosition(glm::vec3(-10, 4.5, 17));
    point2->setPosition(glm::vec3(0, 4.5, 0));
    point2->setIsActive(false);

    point3 = sceneMgr->addSceneObject("point3");
    point3->addComponent(GraphicsManager::getInstance().addPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    //point3->getTransform()->setPosition(glm::vec3(-10, 4.5, 12));
    point3->setPosition(glm::vec3(0, 4.0, -5));
    point3->setIsActive(false);

    spot = sceneMgr->addSceneObject("spot");
    spot->addComponent(GraphicsManager::getInstance().addSpotLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.4f, degToRad(20.0f), LightAttenuation(1.0f, 0.0014f, 0.000007f)));
    spot->setPosition(glm::vec3(0.0f, 5.0f, 5.0f));//-10.0f));
    spot->setRotation(glm::vec3(0.0f, 0.0f, degToRad(-45.0f)));
    spot->setIsActive(false);


    const char* skyboxTextures[] = {"Skybox/rt.bmp", "Skybox/lt.bmp", "Skybox/up.bmp", "Skybox/dn.bmp", "Skybox/ft.bmp", "Skybox/bk.bmp"};
    sceneMgr->addSky(loadTextureCubeMap(skyboxTextures, true));


    GUIManager* gui = new GUIManager;
    Image* img = gui->addImage(ResourceManager::getInstance().loadTexture("opengl_logo.png"));
    img->setPosition(0, gameCfg.windowHeight - img->getTexture()->getSize().y / 2.0f);
    //img->setTextureRect(UintRect(256, 0, 256, 256));
    img->setScale(0.3f, 0.3f);
    //img->setPosition(100, 100);


    RFont* font = ResourceManager::getInstance().loadFont("fonts/arial.ttf");
    Label* label = gui->addLabel(font, "Virtual Bus Core++");
    label->setPosition(10, gameCfg.windowHeight - 20);
    label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    label->scale(0.6f, 0.6f);


    engineIsRunning = gui->addLabel(font, "Engine off");
    engineIsRunning->setPosition(10, 70);
    engineIsRunning->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    engineIsRunning->scale(0.6f, 0.6f);

    Label* gearRatio = gui->addLabel(font, "0");
    gearRatio->setPosition(10, 55);
    gearRatio->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    gearRatio->scale(0.6f, 0.6f);

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


    sndMgr->setActiveCamera(camFPS);

    std::stringstream stream;

    // Time calculation variables
    double t;
    double dt = 1/60.0f;

    double timePhysicsPrev, timePhysicsCurr;

    timePhysicsPrev = timePhysicsCurr = glfwGetTime();

    //how long ago FPS counter was updated
    double lastFPSupdate = timePhysicsCurr;

    double xpos, ypos;

    int nbFrames = 0;

    physMgr->play();

    // ========== MAIN LOOP START ==========

    while (win->isOpened())
    {
        nbFrames++;

        timePhysicsCurr = glfwGetTime();
        double frameTime = timePhysicsCurr - timePhysicsPrev;

        //create string from frame/sec and display it in window title
        if ( timePhysicsCurr - lastFPSupdate >= 1.0f )
        {
            float timing = double(nbFrames);

            nbFrames = 0;
            lastFPSupdate += 1.0f;

            // Convert the fps value into a string using an output stringstream
			std::ostringstream stream;
			stream << timing;
			std::string sTiming = stream.str();

			// Append the FPS value to the window title details
			std::string newWindowTitle = winTitle + " | FPS: " + sTiming;
			win->setWindowTitle(newWindowTitle);
			label->setText(newWindowTitle);
        }

        timePhysicsPrev = timePhysicsCurr;

        glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
        glfwSetCursorPos(win->getWindow(), win->getWidth()/2, win->getHeight()/2);

        camFPS->setRotation(xpos, ypos);


        while ( frameTime > 0.0f )
        {
            float deltaTime = std::min(frameTime, dt);

            readInput(win->getWindow(), deltaTime);
            physMgr->simulate(deltaTime);
            bus->updatePhysics(deltaTime);
            GraphicsManager::getInstance().update(deltaTime);

            frameTime -= deltaTime;

            t += deltaTime;
        }

        sndMgr->update();

        stream.str(std::string());
        stream << bus->getGearbox()->currentRatio();
        std::string gearRatioString("Gear ratio: ");
        gearRatioString += stream.str();
        gearRatio->setText(gearRatioString);

        stream.str(std::string());
        stream << bus->getEngine()->getCurrentRPM();
        std::string stringRPM("RPM: ");
        stringRPM += stream.str();
        labelRPM->setText(stringRPM);

        stream.str(std::string());
        stream << bus->getEngine()->getThrottle();
        std::string stringThrottle("Throttle: ");
        stringThrottle += stream.str();
        labelThrottle->setText(stringThrottle);

        stream.str(std::string());
        stream << bus->getEngine()->getCurrentTorque();
        std::string stringTorque("Torque: ");
        stringTorque += stream.str();
        labelTorque->setText(stringTorque);

        // Render the scene
        renderer->render( GraphicsManager::getInstance().getRenderData() ); //graphMgr->GetRenderData());

        // Render GUI
        renderer->renderGUI(gui->getGUIRenderList());

        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    physMgr->stop();

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    bus->drop();

    sndMgr->drop();
    physMgr->drop();
    renderer->drop();
    delete sceneMgr;

    delete win;

    return 0;
}
