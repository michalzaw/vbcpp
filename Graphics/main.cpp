#include <iostream>
#include <fstream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Model.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Renderer.h"
#include "OGLDriver.h"
#include "Load3ds.h"
#include "Vector3.h"


int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraStatic* camera;
CameraFPS* camFPS;

float Speed = 3.0f;
float MouseSpeed = 0.05;

GLFWwindow* Window;



// Ta funkcja jest wymagana w GLFW > 3.0 aby odpowiednio przetworzyc dane o pozycji kursora myszy na Linuksie
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
std::cout << camFPS->getHorizontalAngle() << "\t" << camFPS->getVerticalAngle() << std::endl;
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
        glfwPollEvents();
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




int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "vbcpp", NULL, NULL);
    if (!Window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(Window, 100,100);

    glfwMakeContextCurrent(Window);

    // Callbacki do obslugi zdarzen
    glfwSetCursorPosCallback(Window, cursor_pos_callback);
    glfwSetKeyCallback(Window, key_callback);
    glfwSetCursorEnterCallback(Window, cursor_enter_callback);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    OGLDriver* driver = new OGLDriver;
    driver->Initialize();


    Load3ds* l = new Load3ds(driver);

	SceneManager* scene = new SceneManager(driver);
	Renderer* renderer = new Renderer(driver);


    Model* model = l->loadModel("crate.3ds", "./");

    std::cout << "Number of meshes: " << model->GetQuantumOfMeshes() << std::endl;

    RenderObject* object = scene->AddRenderObject(model);
    object->GetTransform()->SetPosition(glm::vec3(0,0,0));
    object->GetTransform()->SetRotation(glm::vec3(-0.5f * 3.1415f, 0, 0));


    camFPS = scene->AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    camFPS->setPosition(0,0,-15);
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
    glfwPollEvents();


    std::cout << "Przed petla: " << camFPS->getHorizontalAngle() << "\t" << camFPS->getVerticalAngle() << std::endl;

    while (!glfwWindowShouldClose(Window))
    {
        // Pobieramy aktualna pozycje kursora myszy
        glfwGetCursorPos(Window, &xpos, &ypos);
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

        readInput(Window, DeltaTime);

        renderer->Render(scene->GetRenderData());

        /* Swap front and back buffers */
        glfwSwapBuffers(Window);

        /* Poll for aglm::vec3 color, float ambientIntensity, float diffuseIntensity, glm::vec3 direction)nd process events */
        glfwPollEvents();
    }


    glfwSetInputMode(Window, GLFW_CURSOR_NORMAL, 1);
    glfwTerminate();

    delete l;

    delete renderer;
    delete scene;

    delete driver;

    return 0;
}
