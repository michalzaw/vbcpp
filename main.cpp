#include <iostream>
#include <fstream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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


int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraFPS* camFPS;

Window* win;
PhysicsManager* physMgr;


const btScalar RADTODEG = 180.0f / PI;

// Converts a quaternion to an euler angle
void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler)
{
	btScalar W = TQuat.getW();
	btScalar X = TQuat.getX();
	btScalar Y = TQuat.getY();
	btScalar Z = TQuat.getZ();
	float WSquared = W * W;
	float XSquared = X * X;
	float YSquared = Y * Y;
	float ZSquared = Z * Z;

	TEuler.setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
	TEuler.setY(asinf(-2.0f * (X * Z - Y * W)));
	TEuler.setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
	TEuler *= RADTODEG;
}


// Ta funkcja jest wymagana w GLFW > 3.0 aby odpowiednio przetworzyc dane o pozycji kursora myszy na Linuksie
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    camFPS->setRotation(xpos, ypos);
    glfwSetCursorPos(window, W_WIDTH/2, W_HEIGHT/2);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// Funkcja definiujaca akcje jakie nalezy podjac
// po wejsciu kursora w obreb okna lub po jego opuszczeniu
void cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {


        // Resetowanie kamery po wejsciu kursora w obreb okna - aby uniknac niechcianych
        // rotacji kamery wywolanych przez ruchy mysza przed zainicjowaniem renderingu sceny
        //glfwPollEvents();
        camFPS->reset();
        //glfwSetCursorPos(window, W_WIDTH/2, W_HEIGHT/2);
    }
    else
    {
        // The cursor left the client area of the window
    }
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
    glfwSetCursorEnterCallback(win->getWindow(), cursor_enter_callback);
    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    OGLDriver* driver = new OGLDriver;
    driver->Initialize();

    physMgr = new PhysicsManager;
    physMgr->createPhysicsWorld();

    Load3ds* l = new Load3ds(driver);

	SceneManager* scene = new SceneManager(driver);
	Renderer* renderer = new Renderer(driver);

    Model* model = l->loadModel("crate.3ds", "./");

    RenderObject* object = scene->AddRenderObject(model);
    object->GetTransform()->SetPosition(glm::vec3(0,10,0));
    object->GetTransform()->SetRotation(glm::vec3(0, 0, 0));

    RenderObject* staticBox = scene->AddRenderObject(model);
    staticBox->GetTransform()->SetPosition(glm::vec3(0,0,0));
    staticBox->GetTransform()->SetScale(glm::vec3(4.0f,1,4.0f));

    PhysicalBodyBox* boxBody = new PhysicalBodyBox(btVector3(1,1,1), 8.0f, btVector3(0,10,0));
    physMgr->addPhysicalBody(boxBody);
    boxBody->setRestitution(0.9f);

    PhysicalBodyBox* staticBoxBody = new PhysicalBodyBox(btVector3(4,1,4), 0, btVector3(0,0,0));
    physMgr->addPhysicalBody(staticBoxBody);
    staticBoxBody->setRestitution(0.3f);

    camFPS = scene->AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    camFPS->setPosition(0,4,-15);
    camFPS->setRotationSpeed(0.001f);

    scene->AddLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.5f, -0.6f, 1.0f));

    float angle = 0.0f;
    double Time = glfwGetTime();
    double Total = 0.0;
    int FPS = 0;

    double xpos, ypos;

    // Zrzucamy wszystkie zdarzenia przed wejsciem do glownej petli
    // Inaczej zbierane sa ruchy myszy od momentu uruchomienia aplikacji do momentu uruchomienia renderingu
    // i po uruchomieniu renderowania przetwarzane sa na raz - wywolujac niepotrzebne rotacje kamery FPS
    //win->updateEvents();


    while (win->isOpened())
    {
        // Pobieramy aktualna pozycje kursora myszy
        glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
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
        boxBody->getTransform(trans);
        object->GetTransform()->SetPosition(glm::vec3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));

        btVector3 EulerRot;
        QuaternionToEuler(boxBody->getRigidBody()->getOrientation(), EulerRot);
        object->GetTransform()->SetRotation(glm::vec3(EulerRot[0], EulerRot[1], EulerRot[2]));

        staticBoxBody->getTransform(trans);
        staticBox->GetTransform()->SetPosition(glm::vec3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));



        renderer->Render(scene->GetRenderData());

        /* Swap buffers and poll events */
        win->swapBuffers();
        win->updateEvents();
    }


    glfwSetInputMode(win->getWindow(), GLFW_CURSOR_NORMAL, 1);

    // DESTROY PHYSICS
    physMgr->removePhysicalBody(staticBoxBody);
    physMgr->removePhysicalBody(boxBody);
    //physMgr->destroyPhysicsWorld();

    delete staticBoxBody;
    delete boxBody;
    delete physMgr;
    // -----------------------------------


    //delete staticBox;
    //delete object;

    // Musiałem usunąć niszczenie modelu z klasy RenderObject
    // - inaczej nie mozna wspoldzielic jednego modelu w wielu RenderObjectach
    delete model;
    delete l;

    delete renderer;
    delete scene;


    delete driver;

    delete win;

    return 0;
}
