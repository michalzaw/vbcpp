#include <iostream>
#include <fstream>

#include <map>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Model.h"
//#include "source/Loader3ds.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Renderer.h"
#include "OGLDriver.h"
#include "Load3ds.h"
#include "Vector3.h"

CCamera* camera;
float Speed = 3.0f;
float MouseSpeed = 0.005;

GLFWwindow* Window;



void UpdateCamera(float DeltaTime)
{
	double MouseX, MouseY;
	glfwGetCursorPos(Window, &MouseX, &MouseY);
	glfwSetCursorPos(Window, 800 / 2.0f, 600 / 2.0f);

	camera->RotateX(MouseSpeed * (600 / 2.0f - MouseY));
	camera->RotateY(MouseSpeed * (800 / 2.0f - MouseX));


	CParamsCamera& Params = camera->GetParams();
	glm::vec3 Direction(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(Window, 'W') == GLFW_PRESS)
	{
		Direction += Params.GetForwardVector();
	}
	if (glfwGetKey(Window, 'S') == GLFW_PRESS)
	{
		Direction -= Params.GetForwardVector();
	}
	if (glfwGetKey(Window, 'D') == GLFW_PRESS)
	{
		Direction += Params.GetRightVector();
		}
	if (glfwGetKey(Window, 'A') == GLFW_PRESS)
	{
		Direction -= Params.GetRightVector();
	}

	camera->Move(Direction * Speed * DeltaTime);
}

int main()
{
    if ( !glfwInit() )
    {
        fprintf( stderr, "VIDEO: Failed to initialize GLFW!\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window = glfwCreateWindow(800, 600, "vbcpp", NULL, NULL);
    if (!Window)
    {
        printf("WTF?\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);

    OGLDriver* driver = new OGLDriver;
    driver->Initialize();

    Load3ds* l = new Load3ds(driver);
    //l->setMinCreaseCosAngle(1.0f);

	SceneManager scene(driver);
	Renderer renderer(driver);

    Model* model = l->loadModel("H9.3ds", "PKS2/");

    RenderObject* object = scene.AddRenderObject(model);
    object->GetTransform()->SetRotation(glm::vec3(-0.5f * 3.1415f, 0.0f, 0.0f));

    camera = scene.AddCamera(glm::vec3(0.0f, 0.0f, -6.0f), 0.1f, 1000.0f, 0.25f * 3.1415f,
				 800.0f / 600.0f);

    scene.AddLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.5f, -0.6f, 1.0f));

    float angle = 0.0f;
    double Time = glfwGetTime();
    double Total = 0.0;
    int FPS = 0;
    while (!glfwWindowShouldClose(Window))
    {
        double NewTime = glfwGetTime();
        float DeltaTime = NewTime - Time;
        Time = NewTime;

        Total += DeltaTime;
        ++FPS;
        if (Total >= 1.0)
        {
            std::cout << FPS << std::endl;
            FPS = 0;
            Total = 0.0;
        }
        //std::cout << 1.0f / DeltaTime << std::endl;

        UpdateCamera(DeltaTime);


        renderer.Render(scene.GetRenderData());


        /* Swap front and back buffers */
        glfwSwapBuffers(Window);

        /* Poll for aglm::vec3 color, float ambientIntensity, float diffuseIntensity, glm::vec3 direction)nd process events */
        glfwPollEvents();
    }

    glfwTerminate();

    //delete model;

    delete driver;

    return 0;
}
