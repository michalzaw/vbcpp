#include <iostream>
#include <fstream>

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
<<<<<<< HEAD
=======
    //glfwSetCursorPos(window, W_WIDTH/2, W_HEIGHT/2);
>>>>>>> 473f8febfd64c51e895f93fb0f1d8575ede36455
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

    OGLDriver* driver = new OGLDriver;
    driver->Initialize();

    physMgr = new PhysicsManager;
    physMgr->createPhysicsWorld();

    Load3ds* l = new Load3ds(driver);

	SceneManager* scene = new SceneManager(driver);
	Renderer* renderer = new Renderer(driver);

    Model* model = l->loadModel("crate.3ds", "./");

    RenderObject* object1 = scene->AddRenderObject(model);
    object1->GetTransform()->SetPosition(glm::vec3(-3.0f,10,0));

    RenderObject* object2 = scene->AddRenderObject(model);
    object2->GetTransform()->SetPosition(glm::vec3(1.0f,10,0));

    RenderObject* staticBox = scene->AddRenderObject(model);
    staticBox->GetTransform()->SetPosition(glm::vec3(0,0,0));
    staticBox->GetTransform()->SetScale(glm::vec3(10.0f,1,10.0f));

    PhysicalBodyBox* boxBody1 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 1.0f, btVector3(-3.0f,10,0));
    boxBody1->setRestitution(0.9f);

    PhysicalBodyBox* boxBody2 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 0.0f, btVector3(1.0f,10,0));
    boxBody2->setRestitution(0.7f);

    btTransform trans1, trans2;
    boxBody1->getTransform(trans1);
    boxBody2->getTransform(trans2);

    ConstraintHinge2* hinge = physMgr->createConstraintHinge2(boxBody1, boxBody2, btVector3(1,10,0), btVector3(0,1,0), btVector3(1,0,0));

    PhysicalBodyBox* staticBoxBody = physMgr->createPhysicalBodyBox(btVector3(10,1,10), 0, btVector3(0,0,0));
    staticBoxBody->setRestitution(0.85f);

    camFPS = scene->AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    camFPS->setPosition(0,4,-15);
    camFPS->setRotationSpeed(0.001f);
    camFPS->lookAt(glm::vec3(0,0,0));

    scene->AddLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.5f, -0.6f, 1.0f));

    float angle = 0.0f;
    double Time = glfwGetTime();
    double Total = 0.0;
    int FPS = 0;

    double xpos, ypos;


    while (win->isOpened())
    {
        // Pobieramy aktualna pozycje kursora myszy
        glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
        glfwSetCursorPos(win->getWindow(), W_WIDTH/2, W_HEIGHT/2);
        camFPS->setCursorPos(xpos, ypos);

        double NewTime = glfwGetTime();
        float DeltaTime = NewTime - Time;
        Time = NewTime;


        Total += DeltaTime;
        ++FPS;
        if (Total >= 1.0)
        {
            //std::cout << FPS << std::endl;
            FPS = 0;
            Total = 0.0;
        }

        readInput(win->getWindow(), DeltaTime);

        physMgr->simulate();
        btTransform trans;
        boxBody1->getTransform(trans);
        object1->GetTransform()->SetPosition(glm::vec3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));
        object1->GetTransform()->SetRotation(trans.getRotation().getX(),trans.getRotation().getY(),trans.getRotation().getZ(),trans.getRotation().getW());

        boxBody2->getTransform(trans);
        object2->GetTransform()->SetPosition(glm::vec3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));
        object2->GetTransform()->SetRotation(trans.getRotation().getX(),trans.getRotation().getY(),trans.getRotation().getZ(),trans.getRotation().getW());


        staticBoxBody->getTransform(trans);
        staticBox->GetTransform()->SetPosition(glm::vec3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));
        staticBox->GetTransform()->SetRotation(trans.getRotation().getX(),trans.getRotation().getY(),trans.getRotation().getZ(),trans.getRotation().getW());

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
