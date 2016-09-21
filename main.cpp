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


bool MOUSE_RIGHT_BUTTON_PRESSED = false;

int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraFPS* camFPS;
double oldMouseX, oldMouseY;

Window* win;
PhysicsManager* physMgr;

std::string winTitle = "Virtual Bus Core++";
SceneObject* point, *point2, *point3, *dirLight;


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
	    Light* l = static_cast<Light*>(dirLight->getComponent(0));
	    if (l->GetAmbientIntensity() > 0.05)
        {
            l->SetAmbientIntensity(0.05);
            l->SetDiffuseIntensity(0.0);
        }
        else
        {
            l->SetAmbientIntensity(0.5);
            l->SetDiffuseIntensity(0.5);
        }

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

    OGLDriver::getInstance().Initialize();

    PhysicsManager::getInstance().createPhysicsWorld();

	SceneManager* scene = new SceneManager;

	GraphicsManager* graphMgr = scene->getGraphicsManager();

	Renderer* renderer = new Renderer;


    // Wczytywanie zasobów
    //Load3ds* l = new Load3ds;


    RModel* model = ResourceManager::getInstance().loadModel("crate.3ds", "./");
    RModel* terrain = ResourceManager::getInstance().loadModel("testarea/test_area_n.3ds", "testarea/");
    RModel* crate2 = ResourceManager::getInstance().loadModel("crate2.3ds", "./");

    /* terrain */
    SceneObject* terrainObject = scene->addSceneObject();

    PhysicalBodyBvtTriangleMesh* terrainMesh = PhysicsManager::getInstance().createPhysicalBodyBvtTriangleMesh(terrain, btVector3(0,0,0));
    terrainMesh->setRestitution(0.9f);

    RenderObject* terrainObj = graphMgr->AddRenderObject(terrain);

    terrainObject->addComponent(terrainMesh);
    terrainObject->addComponent(terrainObj);


    /* crate */
    SceneObject* crate = scene->addSceneObject();

    RenderObject* object2 = graphMgr->AddRenderObject(model);

    PhysicalBodyBox* boxBody2 = PhysicsManager::getInstance().createPhysicalBodyBox(btVector3(1,1,1), 1.0f, btVector3(0,7,0));
    boxBody2->setRestitution(0.1f);

    crate->addComponent(object2);
    crate->addComponent(boxBody2);

    /* legs */
    SceneObject* l1 = scene->addSceneObject();
    SceneObject* l2 = scene->addSceneObject();
    SceneObject* l3 = scene->addSceneObject();
    SceneObject* l4 = scene->addSceneObject();


    RenderObject* leg1 = graphMgr->AddRenderObject(crate2);
    RenderObject* leg2 = graphMgr->AddRenderObject(crate2);
    RenderObject* leg3 = graphMgr->AddRenderObject(crate2);
    RenderObject* leg4 = graphMgr->AddRenderObject(crate2);


    PhysicalBodyConvexHull* leg1Body = PhysicsManager::getInstance().createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,3));
    leg1Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg2Body = PhysicsManager::getInstance().createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,3));
    leg2Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg3Body = PhysicsManager::getInstance().createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,-3));
    leg3Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg4Body = PhysicsManager::getInstance().createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,-3));
    leg4Body->setRestitution(0.9f);


    l1->addComponent(leg1);
    l1->addComponent(leg1Body);
    l2->addComponent(leg2);
    l2->addComponent(leg2Body);
    l3->addComponent(leg3);
    l3->addComponent(leg3Body);
    l4->addComponent(leg4);
    l4->addComponent(leg4Body);


    ConstraintHinge2* hinge1 = PhysicsManager::getInstance().createConstraintHinge2(leg1Body, boxBody2, btVector3(1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge2 = PhysicsManager::getInstance().createConstraintHinge2(leg2Body, boxBody2, btVector3(-1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge3 = PhysicsManager::getInstance().createConstraintHinge2(leg3Body, boxBody2, btVector3(1,3,-1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge4 = PhysicsManager::getInstance().createConstraintHinge2(leg4Body, boxBody2, btVector3(-1,3,-1), btVector3(0,1,0), btVector3(1,0,0));

    // Kamera FPS
    SceneObject* Camera = scene->addSceneObject();
    camFPS = graphMgr->AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    Camera->addComponent(camFPS);
    Camera->getTransform()->SetPosition(glm::vec3(0,4,5));
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(8.0f);

    // Światło
    dirLight = scene->addSceneObject();
    dirLight->addComponent(graphMgr->AddDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f));
    dirLight->getTransform()->SetRotation(glm::vec3(0, 0, -0.2f * 3.1416));

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            SceneObject* pointLight = scene->addSceneObject();
            pointLight->addComponent(graphMgr->AddPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.2f, LightAttenuation(0.5f, 0.05f, 0.001f)));
            pointLight->getTransform()->SetPosition(glm::vec3(10 + j * 30, 5, i * 30));
            pointLight->setIsActive(false);
        }
    }


    RModel* busModel = ResourceManager::getInstance().loadModel("H9.3ds", "ZKM/");
    SceneObject* bus = scene->addSceneObject();
    bus->addComponent(graphMgr->AddRenderObject(busModel));
    bus->getTransform()->SetRotation(glm::vec3(-0.5 * 3.14, 0.0f, 0.0f));
    bus->getTransform()->SetPosition(glm::vec3(-10, 2, 20));

    point = scene->addSceneObject();
    point->addComponent(graphMgr->AddPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point->getTransform()->SetPosition(glm::vec3(-10, 4.5, 20));
    point->setIsActive(true);

    point2 = scene->addSceneObject();
    point2->addComponent(graphMgr->AddPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point2->getTransform()->SetPosition(glm::vec3(-10, 4.5, 17));
    point2->setIsActive(true);

    point3 = scene->addSceneObject();
    point3->addComponent(graphMgr->AddPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point3->getTransform()->SetPosition(glm::vec3(-10, 4.5, 12));
    point3->setIsActive(true);


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
            //physMgr->simulate(deltaTime);
            PhysicsManager::getInstance().simulate(deltaTime);


            frameTime -= deltaTime;

            t += deltaTime;
        }

        // Renderowanie sceny
        renderer->Render(graphMgr->GetRenderData());

        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    delete renderer;
    delete scene;

    delete win;

    return 0;
}
