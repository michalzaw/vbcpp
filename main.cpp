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


bool MOUSE_RIGHT_BUTTON_PRESSED = false;

int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraFPS* camFPS;
double oldMouseX, oldMouseY;

Window* win;
PhysicsManager* physMgr;


// Ta funkcja jest wymagana w GLFW > 3.0 aby odpowiednio przetworzyc dane o pozycji kursora myszy na Linuksie
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (MOUSE_RIGHT_BUTTON_PRESSED)
    {
        camFPS->setRotation(xpos, ypos);
        glfwSetCursorPos(window, oldMouseX, oldMouseY);
    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Callback dla pojedynczych zdarzeń - przyciski myszy
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
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
    win->setWindowTitle("Virtual Bus Test");

    // Callbacki do obslugi zdarzen
    glfwSetCursorPosCallback(win->getWindow(), cursor_pos_callback);
    glfwSetKeyCallback(win->getWindow(), key_callback);
    glfwSetMouseButtonCallback(win->getWindow(), mouse_button_callback);

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
    Model* crate2 = l->loadModel("crate2.3ds", "./");

    // Obiekty sceny

    RenderObject* object2 = scene->AddRenderObject(model);
    object2->GetTransform()->SetPosition(glm::vec3(0,7,0));



    RenderObject* leg1 = scene->AddRenderObject(crate2);
    leg1->GetTransform()->SetPosition(glm::vec3(3.0f,3,3));

    RenderObject* leg2 = scene->AddRenderObject(crate2);
    leg2->GetTransform()->SetPosition(glm::vec3(-3.0f,3,3));

    RenderObject* leg3 = scene->AddRenderObject(crate2);
    leg3->GetTransform()->SetPosition(glm::vec3(3.0f,3,-3));

    RenderObject* leg4 = scene->AddRenderObject(crate2);
    leg4->GetTransform()->SetPosition(glm::vec3(-3.0f,3,-3));



    RenderObject* staticBox = scene->AddRenderObject(model);
    staticBox->GetTransform()->SetPosition(glm::vec3(0,0,0));
    staticBox->GetTransform()->SetScale(glm::vec3(10.0f,1,10.0f));

    PhysicalBodyConvexHull* leg1Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,3));
    leg1Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg2Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,3));
    leg2Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg3Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,-3));
    leg3Body->setRestitution(0.9f);

    PhysicalBodyConvexHull* leg4Body = physMgr->createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,-3));
    leg4Body->setRestitution(0.9f);

    PhysicalBodyBox* boxBody2 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 10.0f, btVector3(0,7,0));
    boxBody2->setRestitution(0.1f);

    ConstraintHinge2* hinge1 = physMgr->createConstraintHinge2(leg1Body, boxBody2, btVector3(1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge2 = physMgr->createConstraintHinge2(leg2Body, boxBody2, btVector3(-1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge3 = physMgr->createConstraintHinge2(leg3Body, boxBody2, btVector3(1,3,-1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge4 = physMgr->createConstraintHinge2(leg4Body, boxBody2, btVector3(-1,3,-1), btVector3(0,1,0), btVector3(1,0,0));




    PhysicalBodyBox* staticBoxBody = physMgr->createPhysicalBodyBox(btVector3(10,1,10), 0, btVector3(0,0,0));
    staticBoxBody->setRestitution(0.85f);


    // Podłączenie obiektów fizycznych do obiektów sceny
    leg1->SetPhysicalBody(leg1Body);
    leg2->SetPhysicalBody(leg2Body);
    leg3->SetPhysicalBody(leg3Body);
    leg4->SetPhysicalBody(leg4Body);


    object2->SetPhysicalBody(boxBody2);
    staticBox->SetPhysicalBody(staticBoxBody);

    // Kamera FPS
    camFPS = scene->AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    camFPS->setPosition(0,4,15);
    camFPS->setRotationSpeed(0.0005f);
    camFPS->setMoveSpeed(1.0f);

    // Światło
    scene->AddLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.5f, -0.6f, 1.0f));


    // Zmienne dla obliczania czasu
    float dt = 0.0f;
    float lastUpdateTime = glfwGetTime();
    float accu = 0.0f;
    const float TIME_STEP = 0.5;
    const float MAX_ACCU_TIME = 1.0f;

    double xpos, ypos;

    std::cout << "Vec3 size: " << sizeof(glm::vec3) << std::endl;
    std::cout << "Vec2 size: " << sizeof(glm::vec2) << std::endl;


    while (win->isOpened())
    {
        dt = glfwGetTime() - lastUpdateTime;
        dt = std::max(float(0.0),dt);
        accu += dt;
        accu = clamp<float>(accu, 0, MAX_ACCU_TIME);

        // Integracja modelu fizyki
        while ( accu > TIME_STEP )
        {
        readInput(win->getWindow(), TIME_STEP);
            physMgr->simulate(TIME_STEP);
            accu -= TIME_STEP;
        }

        // Synchronizacja obiektów sceny z fizyką
        scene->updatePhysics();

        // Renderowanie sceny
        renderer->Render(scene->GetRenderData());

        // Swap buffers and poll events
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
