#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

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

#include "Utils/ResourceManager.h"

#include "Scene/Bus.h"
//#include "Scene/RaycastBus.h"

// XML reader
#include "Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "Utils/Math.h"

#include "GUI/GUIManager.h"


// Definicje globalne

bool MOUSE_RIGHT_BUTTON_PRESSED = false;

int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraFPS* camFPS;
double oldMouseX, oldMouseY;

Window* win;
PhysicsManager* physMgr;
SceneManager* sceneMgr;

SceneObject* point, *point2, *point3, *dirLight, *spot = 0;

Bus* bus = 0;
//RaycastBus* bus = 0;

std::string winTitle = "Virtual Bus Core++";


// kod

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS)
	{
		point3->setIsActive(!point3->isActive());

	}if (glfwGetKey( window, GLFW_KEY_K ) == GLFW_PRESS)
	{
		point2->setIsActive(!point2->isActive());

	}if (glfwGetKey( window, GLFW_KEY_J ) == GLFW_PRESS)
	{
		point->setIsActive(!point->isActive());

	}if (glfwGetKey( window, GLFW_KEY_H ) == GLFW_PRESS)
	{
	    Light* l = dynamic_cast<Light*>(dirLight->getComponent(0));

	    if (l->getAmbientIntensity() > 0.05)
        {
            l->setAmbientIntensity(0.05);
            l->setDiffuseIntensity(0.0);
        }
        else
        {
            l->setAmbientIntensity(0.5);
            l->setDiffuseIntensity(0.5);
        }

	}if (glfwGetKey( window, GLFW_KEY_U ) == GLFW_PRESS)
	{
		glm::vec3 r = spot->getTransform()->getRotation();
		r.z += 0.01;
		spot->getTransform()->setRotation(r);

	}if (glfwGetKey( window, GLFW_KEY_I ) == GLFW_PRESS)
	{
		glm::vec3 r = spot->getTransform()->getRotation();
		r.z -= 0.01;
		spot->getTransform()->setRotation(r);

	}if (glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS)
	{
		glm::vec3 r = spot->getTransform()->getRotation();
		r.y += 0.01;
		spot->getTransform()->setRotation(r);

	}if (glfwGetKey( window, GLFW_KEY_P ) == GLFW_PRESS)
	{
		glm::vec3 r = spot->getTransform()->getRotation();
		r.y -= 0.01;
		spot->getTransform()->setRotation(r);

	}

	if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS)
    {
        if (bus->getDoor(0)->state == EDS_CLOSING)
            bus->openDoor(0);
        else
        if (bus->getDoor(0)->state == EDS_OPENING)
            bus->closeDoor(0);
    }

    if (glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS)
    {
        if (bus->getDoor(1)->state == EDS_CLOSING)
            bus->openDoor(1);
        else
        if (bus->getDoor(1)->state == EDS_OPENING)
            bus->closeDoor(1);
    }
}

// Callback dla pojedynczych zdarzeń - przyciski myszy
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    /*
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        MOUSE_RIGHT_BUTTON_PRESSED = true;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        camFPS->setCursorPos(xpos, ypos);
        oldMouseX = xpos; oldMouseY = ypos;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        MOUSE_RIGHT_BUTTON_PRESSED = false;
    }
    */
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
        bus->turnLeft();
    }

    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
    {
        bus->turnRight();
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



// ### MAIN ###
int main()
{
    win = new Window;

    win->createWindow(1366, 768, 100, 100);
    win->setWindowTitle(winTitle);

    // Callbacki do obslugi zdarzen
    glfwSetKeyCallback(win->getWindow(), key_callback);
    glfwSetMouseButtonCallback(win->getWindow(), mouse_button_callback);

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicjalizujemy potrzebne rzeczy
    OGLDriver::getInstance().initialize();
    //PhysicsManager::getInstance().createPhysicsWorld();

    physMgr = new PhysicsManager;
	sceneMgr = new SceneManager;
	Renderer* renderer = new Renderer(win->getWidth(), win->getHeight());


    /* terrain */
    RModel* terrain = ResourceManager::getInstance().loadModel("testarea/test_area_n.3ds", "testarea/");
    RenderObject* terrainObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(terrain));
    SceneObject* terrainObject = sceneMgr->addSceneObject("terrain");
    PhysicalBodyBvtTriangleMesh* terrainMesh = physMgr->createPhysicalBodyBvtTriangleMesh(terrain, btVector3(0,0,0));
    terrainMesh->setRestitution(0.9f);
    terrainMesh->getRigidBody()->setFriction(1.0f);
    terrainObject->addComponent(terrainObj);
    terrainObject->addComponent(terrainMesh);

    bus = new Bus(sceneMgr, physMgr, "h9");
    bus->getSceneObject()->getTransform()->setScale(glm::vec3(1,1,1));

    SceneObject* crate = sceneMgr->addSceneObject("crate");
    RModel* model = ResourceManager::getInstance().loadModel("craten.3ds", "./");
    RenderObject* object2 = GraphicsManager::getInstance().addRenderObject(new RenderObject(model));
    PhysicalBodyBox* boxBody2 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 5.0f, btVector3(0,7,0));
    boxBody2->setRestitution(0.1f);
    crate->addComponent(object2);
    crate->addComponent(boxBody2);
    crate->getTransform()->setPosition(glm::vec3(-10,3,-10));

    // Kamera FPS
    SceneObject* Camera = sceneMgr->addSceneObject("cam1");
    camFPS = GraphicsManager::getInstance().addCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    Camera->addComponent(camFPS);
    Camera->getTransform()->setPosition(glm::vec3(0,4,-15));
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(8.0f);

    // Światło
    dirLight = sceneMgr->addSceneObject("light");
    dirLight->addComponent( GraphicsManager::getInstance().addDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f));
    dirLight->getTransform()->setRotation(glm::vec3(0, 0, -0.2f * PI));
    dirLight->getTransform()->setPosition(glm::vec3(0,20,0));

    point = sceneMgr->addSceneObject("point1");
    point->addComponent(GraphicsManager::getInstance().addPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point->getTransform()->setPosition(glm::vec3(-10, 4.5, 20));
    point->setIsActive(true);

    point2 = sceneMgr->addSceneObject("point2");
    point2->addComponent(GraphicsManager::getInstance().addPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point2->getTransform()->setPosition(glm::vec3(-10, 4.5, 17));
    point2->setIsActive(true);

    point3 = sceneMgr->addSceneObject("point3");
    point3->addComponent(GraphicsManager::getInstance().addPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point3->getTransform()->setPosition(glm::vec3(-10, 4.5, 12));
    point3->setIsActive(true);

    spot = sceneMgr->addSceneObject("spot");
    spot->addComponent(GraphicsManager::getInstance().addSpotLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.4f, degToRad(20.0f), LightAttenuation(1.0f, 0.0014f, 0.000007f)));
    spot->getTransform()->setPosition(glm::vec3(0.0f, 5.0f, -10.0f));
    spot->getTransform()->setRotation(glm::vec3(0.0f, 0.0f, degToRad(-45.0f)));
    spot->setIsActive(true);


    GUIManager* gui = new GUIManager;
    Image* img = gui->addImage(ResourceManager::getInstance().loadTexture("opengl_logo.png"));
    //img->setPosition(100, 768 - 512);
    //img->setTextureRect(UintRect(256, 0, 256, 256));
    img->setScale(0.5f, 0.5f);


    // Zmienne dla obliczania czasu
    double t;
    double dt = 1/60.0f;

    double timePhysicsPrev, timePhysicsCurr;

    timePhysicsPrev = timePhysicsCurr = glfwGetTime();

    //dzieki temu sprawdzamy jak dawno temu byl update licznika klatek
    double lastFPSupdate = timePhysicsCurr;

    double xpos, ypos;

    int nbFrames = 0;

    // ========== MAIN LOOP START ==========

    while (win->isOpened())
    {
        nbFrames++;

        timePhysicsCurr = glfwGetTime();
        double frameTime = timePhysicsCurr - timePhysicsPrev;

        // wyswietlamy liczbe klatek/s
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
            //PhysicsManager::getInstance().simulate(deltaTime);

            frameTime -= deltaTime;

            t += deltaTime;
        }

        //bus->updatePhysics();
        // Renderowanie sceny
        renderer->render( GraphicsManager::getInstance().getRenderData() ); //graphMgr->GetRenderData());

        // Renderowanie GUI
        renderer->renderGUI(gui->getGUIList());



        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    bus->drop();

    physMgr->drop();
    renderer->drop();
    //delete renderer;
    delete sceneMgr;

    delete win;

    return 0;
}
