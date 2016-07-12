#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "Utils/Helpers.hpp"

#include "Window/Window.h"

#include "Graphics/Model.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Shader.h"
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


bool MOUSE_RIGHT_BUTTON_PRESSED = false;

int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraFPS* camFPS;
double oldMouseX, oldMouseY;

Window* win;
PhysicsManager* physMgr;

std::string winTitle = "Virtual Bus Core++";


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

    // Tworzenie głównych modułów
    OGLDriver* driver = new OGLDriver;
    driver->Initialize();

    //physMgr = new PhysicsManager;
    //physMgr->createPhysicsWorld();

	//GraphicsManager* scene = new GraphicsManager(/*driver*/);
	SceneManager* scene = new SceneManager;

	physMgr = scene->getPhysicsManager();
	GraphicsManager* graphMgr = scene->getGraphicsManager();

	Renderer* renderer = new Renderer(driver);


    // Wczytywanie zasobów
    Load3ds* l = new Load3ds(driver);

    //Model* alfa = l->loadModel("alfa/alfa.3ds", "alfa/");
    //Model* model = l->loadModel("H9.3ds", "ZKM/");
    Model* model = l->loadModel("crate.3ds", "./");
    Model* crate2 = l->loadModel("crate2.3ds", "./");
    Model* terrain = l->loadModel("testarea/test_area_n.3ds", "testarea/");

    // Obiekty sceny


    /* terrain */
    SceneObject* terrainObject = scene->addSceneObject();

    PhysicalBodyBvtTriangleMesh* terrainMesh = physMgr->createPhysicalBodyBvtTriangleMesh(terrain, btVector3(0,0,0));
    terrainMesh->setRestitution(0.9f);

    RenderObject* terrainObj = graphMgr->AddRenderObject(terrain);

    terrainObject->addComponent(terrainMesh);
    terrainObject->addComponent(terrainObj);


    /* crate */
    SceneObject* crate = scene->addSceneObject();

    RenderObject* object2 = graphMgr->AddRenderObject(model);

    PhysicalBodyBox* boxBody2 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 1.0f, btVector3(0,7,0));
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


    PhysicalBodyConvexHull* leg1Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,3));
    leg1Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg2Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,3));
    leg2Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg3Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,-3));
    leg3Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg4Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,-3));
    leg4Body->setRestitution(0.9f);


    l1->addComponent(leg1);
    l1->addComponent(leg1Body);
    l2->addComponent(leg2);
    l2->addComponent(leg2Body);
    l3->addComponent(leg3);
    l3->addComponent(leg3Body);
    l4->addComponent(leg4);
    l4->addComponent(leg4Body);




    ConstraintHinge2* hinge1 = physMgr->createConstraintHinge2(leg1Body, boxBody2, btVector3(1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge2 = physMgr->createConstraintHinge2(leg2Body, boxBody2, btVector3(-1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge3 = physMgr->createConstraintHinge2(leg3Body, boxBody2, btVector3(1,3,-1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge4 = physMgr->createConstraintHinge2(leg4Body, boxBody2, btVector3(-1,3,-1), btVector3(0,1,0), btVector3(1,0,0));


    //PhysicalBodyBox* staticBoxBody = physMgr->createPhysicalBodyBox(btVector3(10,1,10), 0, btVector3(0,0,0));
    //staticBoxBody->setRestitution(0.85f);


    // Podłączenie obiektów fizycznych do obiektów sceny
//    leg1->SetPhysicalBody(leg1Body);
 //   leg2->SetPhysicalBody(leg2Body);
 //   leg3->SetPhysicalBody(leg3Body);
 //   leg4->SetPhysicalBody(leg4Body);


//    object2->SetPhysicalBody(boxBody2);
    //staticBox->SetPhysicalBody(staticBoxBody);
    //
    // Kamera FPS
    camFPS = graphMgr->AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    camFPS->setPosition(0,4,5);
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(8.0f);

    // Światło
    SceneObject* dirLight = scene->addSceneObject();
    dirLight->addComponent(graphMgr->AddLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.5f, -0.6f, 1.0f)));
    dirLight->getTransform()->SetRotation(glm::vec3(0, 0, -0.2f * 3.1416));


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

            frameTime -= deltaTime;

            t += deltaTime;
        }

        // Synchronizacja obiektów sceny z fizyką
//        scene->updatePhysics();

        // Renderowanie sceny
        renderer->Render(graphMgr->GetRenderData());

        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //delete physMgr;
    // -----------------------------------

    delete model;
    delete l;

    delete renderer;
    delete scene;


    delete driver;

    delete win;

    return 0;
}
