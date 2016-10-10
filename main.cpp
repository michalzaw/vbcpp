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

// XML reader
#include "Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

// Definicje globalne

bool MOUSE_RIGHT_BUTTON_PRESSED = false;

int W_WIDTH = 1366;
int W_HEIGHT = 768;

CameraFPS* camFPS;
double oldMouseX, oldMouseY;

Window* win;
PhysicsManager* physMgr;
SceneManager* sceneMgr;

std::string winTitle = "Virtual Bus Core++";
SceneObject* point, *point2, *point3, *dirLight;

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

glm::vec3 XMLstringToVec3(const char* xmlstring)
{
    std::stringstream ss(xmlstring);
    //vec3 outVec = {0,0,0};

    std::string s1, s2, s3;
    getline(ss, s1, ',');
    float n1 = (float)atof(s1.c_str());
    std::cout << "N1 " << n1 << std::endl;

    getline(ss, s2, ',');
    float n2 = (float)atof(s2.c_str());
    std::cout << "N2 " << n2 << std::endl;

    getline(ss, s3, ',');
    float n3 = (float)atof(s3.c_str());
    std::cout << "N3 " << n3 << std::endl;

    glm::vec3 outVec(n1, n2, n3);
    //cout << "Sum: " << n1 + n2 + n3 << endl;
    return outVec;
}

void loadXMLbusData(std::string filename)
{
    XMLDocument doc;
    doc.LoadFile( filename.c_str() );

    XMLElement* objElement = doc.FirstChildElement("object");

    std::cout << "XML DATA" << std::endl;
    //std::cout<< objElement->Attribute("name") << std::endl;

    std::string sObjName(objElement->Attribute("name"));

    std::cout<< sObjName << std::endl;

    SceneObject* scnObj = sceneMgr->addSceneObject(sObjName);

    for (XMLElement* child = objElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        const char* ename = child->Name();
        //cout << "Element name: " << ename << endl;

        // OBJECT TRANSFORM
        if (strcmp(ename,"transform") == 0)
        {
            std::cout << "XML: Transform" << std::endl;

            const char* cPosition = child->Attribute("position");
            glm::vec3 position(XMLstringToVec3(cPosition));
            scnObj->getTransform()->SetPosition(position);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));
            scnObj->getTransform()->SetRotation(rotation);

            const char* cScale = child->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            scnObj->getTransform()->SetRotation(scale);

        }
        else // RENDER COMPONENT
        if (strcmp(ename,"component") == 0)
        {
            const char* compType = child->Attribute("type");

            if (strcmp(compType, "render") == 0)
            {
                std::cout << "XML: Render component" << std::endl;
                //const char* renderModel = child->Attribute("type");

                std::string sModel(child->Attribute("model"));
                std::string sTextures(child->Attribute("textures"));

                RModel* model = ResourceManager::getInstance().loadModel(sModel, sTextures);
                RenderObject* renderObj = GraphicsManager::getInstance().AddRenderObject(new RenderObject(model));

                scnObj->addComponent(renderObj);
            }
            else // PHYSICS COMPONENT
            if (strcmp(compType, "physics") == 0)
            {
                std::cout << "XML: Physics component" << std::endl;
            }
        }
        else // PHYSICS CONSTRAINTS
        if (strcmp(ename,"constraint") == 0)
        {
            std::cout << "XML: Physics constraint" << std::endl;
        }

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
    OGLDriver::getInstance().Initialize();
    //PhysicsManager::getInstance().createPhysicsWorld();

    physMgr = new PhysicsManager;
	sceneMgr = new SceneManager;
	Renderer* renderer = new Renderer;

    /* terrain */
    RModel* terrain = ResourceManager::getInstance().loadModel("testarea/test_area_n.3ds", "testarea/");
    RenderObject* terrainObj = GraphicsManager::getInstance().AddRenderObject(new RenderObject(terrain));
    SceneObject* terrainObject = sceneMgr->addSceneObject("terrain");
    PhysicalBodyBvtTriangleMesh* terrainMesh = physMgr->createPhysicalBodyBvtTriangleMesh(terrain, btVector3(0,0,0));
    terrainMesh->setRestitution(0.9f);
    terrainObject->addComponent(terrainObj);
    terrainObject->addComponent(terrainMesh);

    loadXMLbusData("bus.xml");

    /* crate */
    SceneObject* crate = sceneMgr->addSceneObject("crate");
    RModel* model = ResourceManager::getInstance().loadModel("crate.3ds", "./");
    RenderObject* object2 = GraphicsManager::getInstance().AddRenderObject(new RenderObject(model));
    PhysicalBodyBox* boxBody2 = physMgr->createPhysicalBodyBox(btVector3(1,1,1), 1.0f, btVector3(0,7,0));
    boxBody2->setRestitution(0.1f);
    crate->addComponent(object2);
    crate->addComponent(boxBody2);

    /* legs objects */
    RModel* wheel1model = ResourceManager::getInstance().loadModel("wheel.3ds", "./");
    SceneObject* wheel1Obj = sceneMgr->addSceneObject("wheel1");
    RenderObject* wheel1Ren = GraphicsManager::getInstance().AddRenderObject(new RenderObject(wheel1model));
    PhysicalBodyCylinder* wheel1Body = physMgr->createPhysicalBodyCylinder(btVector3(0.5f,1,1), 1.0f, btVector3(3,3,3), X_AXIS);
    wheel1Body->setRestitution(0.9f);
    wheel1Obj->addComponent(wheel1Ren);
    wheel1Obj->addComponent(wheel1Body);


    RModel* wheel2model = ResourceManager::getInstance().loadModel("wheel.3ds", "./");
    SceneObject* wheel2Obj = sceneMgr->addSceneObject("wheel2");
    RenderObject* wheel2Ren = GraphicsManager::getInstance().AddRenderObject(new RenderObject(wheel2model));
    PhysicalBodyCylinder* wheel2Body = physMgr->createPhysicalBodyCylinder(btVector3(0.5f,1,1), 1.0f, btVector3(-3,3,3), X_AXIS);
    wheel2Body->setRestitution(0.9f);
    wheel2Obj->addComponent(wheel2Ren);
    wheel2Obj->addComponent(wheel2Body);


    RModel* wheel3model = ResourceManager::getInstance().loadModel("wheel.3ds", "./");
    SceneObject* wheel3Obj = sceneMgr->addSceneObject("wheel3");
    RenderObject* wheel3Ren = GraphicsManager::getInstance().AddRenderObject(new RenderObject(wheel3model));
    PhysicalBodyConvexHull* wheel3Body = physMgr->createPhysicalBodyConvexHull(wheel3model->GetVertices(), wheel3model->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,-3));
    wheel3Body->setRestitution(0.9f);
    wheel3Obj->addComponent(wheel3Ren);
    wheel3Obj->addComponent(wheel3Body);


    RModel* wheel4model = ResourceManager::getInstance().loadModel("wheel.3ds", "./");
    SceneObject* wheel4Obj = sceneMgr->addSceneObject("wheel4");
    RenderObject* wheel4Ren = GraphicsManager::getInstance().AddRenderObject(new RenderObject(wheel4model));
    PhysicalBodyConvexHull* wheel4Body = physMgr->createPhysicalBodyConvexHull(wheel4model->GetVertices(), wheel4model->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,-3));
    wheel4Body->setRestitution(0.9f);
    wheel4Obj->addComponent(wheel4Ren);
    wheel4Obj->addComponent(wheel4Body);


    //PhysicalBodyConvexHull* leg1Body = PhysicsManager::getInstance().createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,3));
    //PhysicalBodyConvexHull* leg2Body = PhysicsManager::getInstance().createPhysicalBodyConvexHull(crate2->GetVertices(), crate2->GetQuantumOfVertices(), 1.0f, btVector3(-3.0f,3,3));


    ConstraintHinge2* hinge1 = physMgr->createConstraintHinge2(wheel1Body, boxBody2, btVector3(1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge2 = physMgr->createConstraintHinge2(wheel2Body, boxBody2, btVector3(-1,3,1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge3 = physMgr->createConstraintHinge2(wheel3Body, boxBody2, btVector3(1,3,-1), btVector3(0,1,0), btVector3(1,0,0));
    ConstraintHinge2* hinge4 = physMgr->createConstraintHinge2(wheel4Body, boxBody2, btVector3(-1,3,-1), btVector3(0,1,0), btVector3(1,0,0));

    // Kamera FPS
    SceneObject* Camera = sceneMgr->addSceneObject("cam1");
    camFPS = GraphicsManager::getInstance().AddCameraFPS(W_WIDTH, W_HEIGHT, 45.0f, 0.1f, 500);
    Camera->addComponent(camFPS);
    Camera->getTransform()->SetPosition(glm::vec3(0,4,5));
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(8.0f);

    // Światło
    //dirLight = scene->addSceneObject();
    //dirLight->addComponent(graphMgr->AddDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f));
    SceneObject* dirLight = sceneMgr->addSceneObject("light");
    dirLight->addComponent( GraphicsManager::getInstance().AddDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f));
    dirLight->getTransform()->SetRotation(glm::vec3(0, 0, -0.2f * PI));

    /*
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
    */

    RModel* busModel = ResourceManager::getInstance().loadModel("H9.3ds", "ZKM/");
    SceneObject* bus = sceneMgr->addSceneObject("bus");
    //RenderObject* busro = GraphicsManager::getInstance().AddRenderObject(new RenderObject(busModel) );
    bus->addComponent(GraphicsManager::getInstance().AddRenderObject(new RenderObject(busModel) ));
    bus->getTransform()->SetRotation(glm::vec3(-0.5 * PI, 0.0f, 0.0f));
    bus->getTransform()->SetPosition(glm::vec3(-10, 2, 20));

    point = sceneMgr->addSceneObject("point1");
    point->addComponent(GraphicsManager::getInstance().AddPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point->getTransform()->SetPosition(glm::vec3(-10, 4.5, 20));
    point->setIsActive(true);

    point2 = sceneMgr->addSceneObject("point2");
    point2->addComponent(GraphicsManager::getInstance().AddPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
    point2->getTransform()->SetPosition(glm::vec3(-10, 4.5, 17));
    point2->setIsActive(true);

    point3 = sceneMgr->addSceneObject("point3");
    point3->addComponent(GraphicsManager::getInstance().AddPointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.f, 0.2f, LightAttenuation(1.0f, 0.1f, 0.01f)));
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
            physMgr->simulate(deltaTime);
            //PhysicsManager::getInstance().simulate(deltaTime);


            frameTime -= deltaTime;

            t += deltaTime;
        }

        // Renderowanie sceny
        renderer->Render( GraphicsManager::getInstance().GetRenderData() ); //graphMgr->GetRenderData());

        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    physMgr->drop();
    renderer->drop();
    //delete renderer;
    delete sceneMgr;

    delete win;

    return 0;
}
