#include <iostream>
#include <fstream>
#include <algorithm>

#include "Utils/Helpers.hpp"

#include "Window/Window.h"

#include "Graphics/Model.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Shader.h"
#include "Graphics/Renderer.h"
#include "Graphics/OGLDriver.h"
#include "Graphics/Load3ds.h"
#include "Graphics/Vector3.h"

#include "Physics/PhysicsManager.hpp"
#include "Physics/PhysicalBodyBox.hpp"
#include "Physics/ConstraintHinge.hpp"
#include "Physics/ConstraintHinge2.hpp"


int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraFPS* camFPS;

Window* win;
PhysicsManager* physMgr;


// Ta funkcja jest wymagana w GLFW > 3.0 aby odpowiednio przetworzyc dane o pozycji kursora myszy na Linuksie
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    camFPS->setRotation(xpos, ypos);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// Niebuforowane przetwarzanie klawiatury
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
    win->setWindowTitle("Virtual Bus Test");

    // Callbacki do obslugi zdarzen
    glfwSetCursorPosCallback(win->getWindow(), cursor_pos_callback);
    glfwSetKeyCallback(win->getWindow(), key_callback);
    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Tworzenie głównych modułów
    OGLDriver* driver = new OGLDriver;
    driver->Initialize();

    physMgr = new PhysicsManager;
    physMgr->createPhysicsWorld();

	SceneManager* scene = new SceneManager(driver);
	Renderer* renderer = new Renderer(driver);


    // Wczytywanie zasobów
    Load3ds* l = new Load3ds(driver);

    Model* model = l->loadModel("crate.3ds", "./");

    // Obiekty sceny
    RenderObject* object1 = scene->AddRenderObject(model);
    object1->GetTransform()->SetPosition(glm::vec3(3.0f,10,0));

    RenderObject* object2 = scene->AddRenderObject(model);
    object2->GetTransform()->SetPosition(glm::vec3(-1.0f,10,0));

    RenderObject* staticBox = scene->AddRenderObject(model);
    staticBox->GetTransform()->SetPosition(glm::vec3(0,0,0));
    staticBox->GetTransform()->SetScale(glm::vec3(10.0f,1,10.0f));

    // Obiekty fizyczne
    PhysicalBodyBox* boxBody1 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 1.0f, btVector3(3.0f,10,0));
    boxBody1->setRestitution(0.9f);

    PhysicalBodyBox* boxBody2 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 0.0f, btVector3(-1.0f,10,0));
    boxBody2->setRestitution(0.7f);

    ConstraintHinge2* hinge = physMgr->createConstraintHinge2(boxBody1, boxBody2, btVector3(1,10,0), btVector3(0,1,0), btVector3(1,0,0));

    PhysicalBodyBox* staticBoxBody = physMgr->createPhysicalBodyBox(btVector3(10,1,10), 0, btVector3(0,0,0));
    staticBoxBody->setRestitution(0.85f);


    // Podłączenie obiektów fizycznych do obiektów sceny
    object1->SetPhysicalBody(boxBody1);
    object2->SetPhysicalBody(boxBody2);
    staticBox->SetPhysicalBody(staticBoxBody);

    camFPS = scene->AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    camFPS->setPosition(0,4,15);
    camFPS->setRotationSpeed(0.0005f);
    camFPS->setMoveSpeed(1.0f);

    scene->AddLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.5f, -0.6f, 1.0f));


    float dt = 0.0f;
    float lastUpdateTime = glfwGetTime();
    float accu = 0.0f;
    const float TIME_STEP = 0.5;
    const float MAX_ACCU_TIME = 1.0f;

    double xpos, ypos;


    while (win->isOpened())
    {
        // Pobieramy aktualna pozycje kursora myszy
        glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
        glfwSetCursorPos(win->getWindow(), W_WIDTH/2, W_HEIGHT/2);
        camFPS->setCursorPos(xpos, ypos);

        dt = glfwGetTime() - lastUpdateTime;
        dt = std::max(float(0.0),dt);
        accu += dt;
        accu = clamp<float>(accu, 0, MAX_ACCU_TIME);


        while ( accu > TIME_STEP )
        {
        readInput(win->getWindow(), TIME_STEP);
            physMgr->simulate(TIME_STEP);
            accu -= TIME_STEP;
        }

        //physMgr->simulate(DeltaTime);


        scene->updatePhysics();

        renderer->Render(scene->GetRenderData());

        /* Swap buffers and poll events */
        win->swapBuffers();
        win->updateEvents();
    }

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    delete physMgr;
    // -----------------------------------

    delete model;
    delete l;

    delete renderer;
    delete scene;


    delete driver;

    delete win;

    return 0;
}
