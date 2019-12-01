#include <iostream>
#include <ctime>

#include "Window/Window.h"

#include "Graphics/GraphicsManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/OGLDriver.h"
#include "Graphics/LoadTexture.h"

#include "Physics/PhysicsManager.hpp"

#include "Scene/SceneManager.h"
#include "Scene/SoundManager.h"
#include "Scene/SceneLoader.h"

#include "Utils/ResourceManager.h"
#include "Utils/Timer.h"
#include "Utils/Helpers.hpp"
#include "Utils/Math.h"
#include "Utils/Logger.h"
#include "Utils/RaycastingUtils.h"

#include "Bus/BusConstraint.h"
#include "Bus/BusRaycast.h"
#include "Bus/BusLoader.h"

#include "GUI/GUIManager.h"

#include "Game/GameConfig.h"
#include "Game/BusStopSystem.h"
#include "Game/Hud.h"

#include "EditorMain.h"

#define DRAW_IMGUI true
#ifdef DRAW_IMGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#endif // DRAW_IMGUI


#define RUN_EDITOR false


bool showImGui = false;


// Definicje globalne
CameraFPS* camFPS;

Window* win;
PhysicsManager* physMgr = NULL;
SceneManager* sceneMgr = NULL;
SoundManager* sndMgr = NULL;

Bus* bus = NULL;

std::string winTitle = "Virtual Bus Core++";


bool mirrorControl = false;
int mirrorControlIndex = -1;

bool cameraControll = true;

bool runEditor = RUN_EDITOR;


void onSceneObjectClick(SceneObject* sceneObject)
{
    std::cout << sceneObject->getName() << std::endl;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	#ifdef DRAW_IMGUI
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
	#endif // DRAW_IMGUI

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        cameraControll = !cameraControll;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        win->setCloseFlag();

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		bus->setIsEnableHeadlights(!bus->isEnableHeadlights());
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		bus->setIsEnableLights(!bus->isEnableLights());
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		SceneObject* dirLight = sceneMgr->getSceneObject("sun");
	    Light* l = dynamic_cast<Light*>(dirLight->getComponent(CT_LIGHT));

	    if (l->getDiffiseIntenisty() > 0.05)
        {
            l->setAmbientIntensity(0.0025);
            l->setDiffuseIntensity(0.0);
            Renderer::getInstance().setDayNightRatio(-1.0f);
        }
        else
        {
            l->setAmbientIntensity(0.025);
            l->setDiffuseIntensity(0.5);
            Renderer::getInstance().setDayNightRatio(1.0f);
        }

	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        bus->doorOpenClose(1);
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        bus->doorOpenClose(2);
    }

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (camFPS->getSceneObject()->hasParent())
        {
            camFPS->getSceneObject()->setPosition(camFPS->getPosition());
            camFPS->getSceneObject()->removeParent();
        }
        else
        {
            bus->getSceneObject()->addChild(camFPS->getSceneObject());
            camFPS->getSceneObject()->setPosition(bus->getDriverPosition());
            camFPS->getSceneObject()->setRotation(0,0,0);
        }
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        bus->getGearbox()->shiftUp();

  	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
        bus->getGearbox()->shiftDown();

  	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        if (!bus->getEngine()->isRunning())
        {
            bus->startEngine();
        }
        else
        {
            bus->stopEngine();
        }
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if (!physMgr->isRunning())
            physMgr->play();
        else
            physMgr->stop();
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        bus->toggleHandbrake();
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        ++mirrorControlIndex;
        if (mirrorControlIndex < bus->getMirrorsCount())
        {
            mirrorControl = true;
        }
        else
        {
            mirrorControl = false;
            mirrorControlIndex = -1;
        }
    }

	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
	{
		showImGui = !showImGui;
	}

    // debug
    if (key == GLFW_KEY_1 && action == GLFW_PRESS && glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS)
    {
        ResourceManager::getInstance().reloadAllShaders();
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS && glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS)
    {
        ResourceManager::getInstance().reloadAllTextures();
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS && glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS)
    {
        Renderer::getInstance().toogleRenderAABBFlag();
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS && glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS)
    {
        Renderer::getInstance().toogleRenderOBBFlag();
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS && glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS)
    {
        Renderer::getInstance().setAlphaToCoverage(!(Renderer::getInstance().isAlphaToCoverageEnable()));
    }
    if (key == GLFW_KEY_8 && action == GLFW_PRESS && glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS)
    {
        //Renderer::getInstance().t = (Renderer::getInstance().t + 1) % 2;
		Renderer::getInstance().setBloom(!(Renderer::getInstance().isBloomEnable()));
    }
	if (key == GLFW_KEY_9 && action == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->a = !(GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->a);
	}
}

void rayTestWithModelNode(RenderObject* renderObject, ModelNode* modelNode, glm::vec3 rayStart, glm::vec3 rayDir, glm::mat4 parentTransform = glm::mat4(1.0f))
{
    AABB* aabb = modelNode->getAABB();
    glm::mat4 modelMatrix = parentTransform * modelNode->getTransformMatrix();
    float distance;
    if (isRayIntersectOBB(rayStart, rayDir, *aabb, modelMatrix, distance))
    {
        glm::vec4 rayStartLocalspace = glm::inverse(modelMatrix) * glm::vec4(rayStart.x, rayStart.y, rayStart.z, 1.0f);
        glm::vec4 rayDirLocalspace = glm::inverse(modelMatrix) * glm::vec4(rayDir.x, rayDir.y, rayDir.z, 0.0f);

        if (distance > 0.0f)
        {
            ClickableObject* clickableObject = static_cast<ClickableObject*>(renderObject->getSceneObject()->getComponent(CT_CLICKABLE_OBJECT));
            if (clickableObject != NULL)
            {
                clickableObject->click(modelNode);
            }
        }
    }

    for (int i = 0; i < modelNode->getChildrenCount(); ++i)
    {
        rayTestWithModelNode(renderObject, modelNode->getChildren()[i], rayStart, rayDir, modelMatrix);
    }
}


// Callback dla pojedynczych zdarzeń - przyciski myszy
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
        cameraControll = !cameraControll;

    if (action == GLFW_RELEASE)
    {
        double xpos, ypos;
        glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
        ypos = win->getHeight() - ypos;

        glm::vec3 rayStart;
        glm::vec3 rayDir;
        calculateRay(xpos, ypos, camFPS, rayStart, rayDir);

        // collision with model nodes
        std::list<RenderObject*>& renderObjects = GraphicsManager::getInstance().getRenderObjects();
        for (std::list<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i)
        {
            RenderObject* renderObject = *i;
            rayTestWithModelNode(renderObject, renderObject->getModelRootNode(), rayStart, rayDir, renderObject->getSceneObject()->getGlobalTransformMatrix());
        }


        // collision with the whole model
        /*std::list<RenderObject*>& renderObjects = GraphicsManager::getInstance().getRenderObjects();
        for (std::list<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i)
        {
            RenderObject* renderObject = *i;
            AABB* aabb = renderObject->getModel()->getAABB();
            float distance;
            if (isRayIntersectOBB(glm::vec3(rayStartWorldspace), rayDir, *aabb, renderObject->getSceneObject()->getGlobalTransformMatrix(), distance))
            {
                if (distance > 0.0f)
                    std::cout << renderObject->getSceneObject()->getName() << " " << distance << std::endl;
            }

            rayTestWithModelNode(renderObject->getModelRootNode(), glm::vec3(rayStartWorldspace), rayDir, renderObject->getSceneObject()->getGlobalTransformMatrix());
        }*/


        // bullet physics engine
        /*btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(rayStartWorldspace.x, rayStartWorldspace.y, rayStartWorldspace.z),
                                                               btVector3(rayEnd.x, rayEnd.y, rayEnd.z));

        rayCallback.m_collisionFilterMask = COL_BUS | COL_DOOR | COL_ENV | COL_TERRAIN | COL_WHEEL;
        rayCallback.m_collisionFilterGroup = COL_ENV;

        physMgr->getDynamicsWorld()->rayTest(btVector3(rayStartWorldspace.x, rayStartWorldspace.y, rayStartWorldspace.z),
                                             btVector3(rayEnd.x, rayEnd.y, rayEnd.z),
                                             rayCallback);

        if (rayCallback.hasHit())
        {
            void* data = rayCallback.m_collisionObject->getUserPointer();
            if (data != NULL)
            {
                SceneObject* object = static_cast<SceneObject*>(data);
                onSceneObjectClick(object);
            }
        }*/
    }
}


// Bufforowane przetwarzanie zdarzeń
void readInput(GLFWwindow* window, double deltaTime)
{
	#ifdef DRAW_IMGUI
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
	#endif // DRAW_IMGUI

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

	if (mirrorControl)
    {
        if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
        {
            bus->getMirror(mirrorControlIndex)->getSceneObject()->rotate(0.0f, -0.01f, 0.0f);
        }

        if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
        {
            bus->getMirror(mirrorControlIndex)->getSceneObject()->rotate(0.0f, 0.01f, 0.0f);
        }

        if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS)
        {
            bus->getMirror(mirrorControlIndex)->getSceneObject()->rotate(0.01f, 0.0f, 0.0f);
        }

        if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
        {
            bus->getMirror(mirrorControlIndex)->getSceneObject()->rotate(-0.01f, 0.0f, 0.0f);
        }
    }
    else
    {
        if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
        {
            bus->turnLeft(deltaTime);
        }

        if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
        {
            bus->turnRight(deltaTime);
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

        if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_RELEASE && glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_RELEASE)
        {
            bus->centerSteringWheel(deltaTime);
        }
    }

    static float x = 0;
    static float angle = 0;
    if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS)
    {
        if (x < 1.0f)
        {
            x += deltaTime;
            float x1 = glm::mix(-PI / 2.0f, PI / 2.0f, x);
            float sinx1 = sinf(x1);
            sinx1 = sinx1 * 0.5f + 0.5f;
            sinx1 *= 25.0f;

            float delta = sinx1 - angle;
            angle += delta;
            camFPS->getSceneObject()->rotate(0, degToRad(-delta), 0);
        }
    }

    if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_RELEASE)
    {
        if (x > 0.0f)
        {
            x -= deltaTime;
            float x1 = glm::mix(-PI / 2.0f, PI / 2.0f, x);
            float sinx1 = sinf(x1);
            sinx1 = sinx1 * 0.5f + 0.5f;
            sinx1 *= 25.0f;

            float delta = sinx1 - angle;
            angle += delta;
            camFPS->getSceneObject()->rotate(0, degToRad(-delta), 0);
        }
    }

    if (glfwGetKey( window, GLFW_KEY_6 ) == GLFW_PRESS)
    {
        Renderer::getInstance().setExposure(Renderer::getInstance().getExposure() * 1.1f);
        //dirLight->rotate(0, -0.001, 0);
        //Light* l = static_cast<Light*>(dirLight->getComponent(CT_LIGHT));
        //Logger::info("Light dir: " + toString(l->getDirection().x) + ", " + toString(l->getDirection().y) + ", " + toString(l->getDirection().z));
    }
    if (glfwGetKey( window, GLFW_KEY_7 ) == GLFW_PRESS)
    {
        Renderer::getInstance().setExposure(Renderer::getInstance().getExposure() / 1.1f);
        //dirLight->rotate(0, 0.001, 0);
        //Light* l = static_cast<Light*>(dirLight->getComponent(CT_LIGHT));
        //Logger::info("Light dir: " + toString(l->getDirection().x) + ", " + toString(l->getDirection().y) + ", " + toString(l->getDirection().z));
    }
    if (glfwGetKey( window, GLFW_KEY_8 ) == GLFW_PRESS)
    {
        Logger::info("Exposure: " + toString(Renderer::getInstance().getExposure()));
        //Logger::info("Light dir: " + toString(dirLight->getRotation().x) + ", " + toString(dirLight->getRotation().y) + ", " + toString(dirLight->getRotation().z));
    }

}


void initializeImGui()
{
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(win->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("fonts/arial.ttf", 13.0f);
	io.Fonts->AddFontDefault();
}


void destroyImGuiContext()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void drawLineAndDirectionWindow()
{
	if (ImGui::Begin("Line and direction"))
	{
		char lineBuffer[1024];
		char direction1Buffer[1024];
		char direction2Buffer[1024];

		strncpy(lineBuffer, bus->getDisplayText().head.c_str(), sizeof lineBuffer);
		strncpy(direction1Buffer, bus->getDisplayText().line1.c_str(), sizeof direction1Buffer);
		strncpy(direction2Buffer, bus->getDisplayText().line2.c_str(), sizeof direction2Buffer);

		lineBuffer[sizeof lineBuffer - 1] = '\0';
		direction1Buffer[sizeof direction1Buffer - 1] = '\0';
		direction2Buffer[sizeof direction2Buffer - 1] = '\0';

		if (ImGui::InputText("Line", lineBuffer, IM_ARRAYSIZE(lineBuffer)))
		{
			bus->getDisplayText().head = lineBuffer;
		}
		if (ImGui::InputText("Direction line 1", direction1Buffer, IM_ARRAYSIZE(direction1Buffer)))
		{
			bus->getDisplayText().line1 = direction1Buffer;
		}
		if (ImGui::InputText("Direction line 2", direction2Buffer, IM_ARRAYSIZE(direction2Buffer)))
		{
			bus->getDisplayText().line2 = direction2Buffer;
		}

		int e = bus->getDisplayText().type;
		ImGui::RadioButton("one line", &e, 1);
		ImGui::RadioButton("two lines", &e, 2);
		ImGui::RadioButton("two lines, first big", &e, 3);
		ImGui::RadioButton("two lines, second big", &e, 4);
		bus->getDisplayText().type = (DisplayTextType)e;

		if (ImGui::Button("Ok", ImVec2(120, 0)))
		{
			bus->updateDisplays();
		}
	}
	ImGui::End();
}


void drawGrassColorWindow()
{
	if (ImGui::Begin("Grass color"))
	{
		SceneObject* grassObject = sceneMgr->getSceneObject("grass");
		Grass* grass = static_cast<Grass*>(grassObject->getComponent(CT_GRASS));
		glm::vec4 color = grass->getGrassColor();

		ImGui::Text("Color widget:");
		ImGui::ColorEdit3("MyColor##1", (float*)& color, ImGuiColorEditFlags_NoOptions);

		grass->setGrassColor(color);
	}
	ImGui::End();
}


void drawColorWindow()
{
	if (ImGui::Begin("Tree colors"))
	{
		ImGui::ColorEdit3("Color1##1", (float*)& Renderer::getInstance().color1, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit3("Color2##2", (float*)& Renderer::getInstance().color2, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit3("Color3##3", (float*)& Renderer::getInstance().color3, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit3("Color4##4", (float*)& Renderer::getInstance().color4, ImGuiColorEditFlags_NoOptions);
	}
	ImGui::End();
}


void drawImGui()
{
	if (showImGui)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		drawLineAndDirectionWindow();
		drawGrassColorWindow();
		drawColorWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}


void catchArgs(int argc, char** argv)
{
	if (argc > 1)
	{
		Logger::info("Running with args:");
		for (int i = 1; i < argc; ++i)
		{
			Logger::info(argv[i]);

			if (strcmp(argv[i], "--editor") == 0)
			{
				runEditor = true;
			}
		}
	}
}


// ### MAIN ###
int main(int argc, char** argv)
{
	catchArgs(argc, argv);

	if (runEditor)
	{
		return editorMain();
	}

    srand(static_cast<unsigned int>(time(NULL)));

    GameConfig::getInstance().loadGameConfig("game.xml");
	#ifdef DEVELOPMENT_RESOURCES
	GameConfig::getInstance().loadDevelopmentConfig("devSettings.xml");
	ResourceManager::getInstance().setAlternativeResourcePath(GameConfig::getInstance().alternativeResourcesPath);
	#endif // DEVELOPMENT_RESOURCES


    win = new Window;
    win->createWindow(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, 10, 40, GameConfig::getInstance().isFullscreen);
    win->setWindowTitle(winTitle);

    // Callbacki do obslugi zdarzen
    glfwSetKeyCallback(win->getWindow(), key_callback);
    glfwSetMouseButtonCallback(win->getWindow(), mouse_button_callback);

	#ifdef DRAW_IMGUI
	initializeImGui();
	#endif // DRAW_IMGUI

    //glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicjalizujemy potrzebne rzeczy
    OGLDriver::getInstance().initialize();

    physMgr = new PhysicsManager;
    sndMgr = new SoundManager();
    sndMgr->setMute(true);
	sceneMgr = new SceneManager(physMgr, sndMgr);


	Renderer& renderer = Renderer::getInstance();
	renderer.setMsaaAntialiasing(GameConfig::getInstance().msaaAntialiasing);
	renderer.setMsaaAntialiasingLevel(GameConfig::getInstance().msaaAntialiasingLevel);
	renderer.setBloom(GameConfig::getInstance().isBloomEnabled);
	renderer.setIsShadowMappingEnable(GameConfig::getInstance().isShadowmappingEnable);
	renderer.init(win->getWidth(), win->getHeight());
    renderer.setDayNightRatio(1.0f);
    renderer.setAlphaToCoverage(true);
    renderer.setExposure(1.87022f);
	renderer.setToneMappingType(TMT_CLASSIC);
    renderer.t = 0;


	GraphicsManager::getInstance().setWindDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	GraphicsManager::getInstance().setWindVelocity(0.6f);


	BusLoader busLoader(sceneMgr, physMgr, sndMgr);
	bus = busLoader.loadBus(GameConfig::getInstance().busModel);

	SceneLoader sceneLoader(sceneMgr);
	sceneLoader.loadMap(GameConfig::getInstance().mapFile);

    //sceneMgr->loadScene(GameConfig::getInstance().mapFile);

    bus->getSceneObject()->setPosition(sceneMgr->getBusStart().position);
    bus->getSceneObject()->setRotation(degToRad(sceneMgr->getBusStart().rotation.x),
                                       degToRad(sceneMgr->getBusStart().rotation.y),
                                       degToRad(sceneMgr->getBusStart().rotation.z));

    // Camera FPS
    SceneObject* Camera = sceneMgr->addSceneObject("cam1");
    camFPS = GraphicsManager::getInstance().addCameraFPS(GameConfig::getInstance().windowWidth, GameConfig::getInstance().windowHeight, degToRad(58.0f), 0.1f, 1000);
    Camera->addComponent(camFPS);
    camFPS->setRotationSpeed(0.001f);
    camFPS->setMoveSpeed(50.0f);
    Camera->setRotation(0,degToRad(-90), 0);
    Camera->setPosition(10,7,-10);
    Camera->setPosition(0, 0, 0);
    GraphicsManager::getInstance().setCurrentCamera(camFPS);


	renderer.bakeStaticShadows();


    GUIManager* gui = new GUIManager;
    //Image* img = gui->addImage(ResourceManager::getInstance().loadTexture("opengl_logo.png"));
    //img->setPosition(0, GameConfig::getInstance().windowHeight - img->getTexture()->getSize().y / 2.0f);
    //img->setScale(0.3f, 0.3f);

    RFont* font = ResourceManager::getInstance().loadFont("fonts/arial.ttf");
    /*Label* label = gui->addLabel(font, winTitle);
    label->setPosition(10, GameConfig::getInstance().windowHeight - 20);
    label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    label->scale(0.6f, 0.6f);

    Label* labelSpeed = gui->addLabel(font, "Speed: ");
    labelSpeed->setPosition(10, 85);
    labelSpeed->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelSpeed->scale(0.6f, 0.6f);

    Label* labelEngineIsRunning = gui->addLabel(font, "Engine off");
    labelEngineIsRunning->setPosition(10, 70);
    labelEngineIsRunning->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelEngineIsRunning->scale(0.6f, 0.6f);

    Label* labelGearRatio = gui->addLabel(font, "0");
    labelGearRatio->setPosition(10, 55);
    labelGearRatio->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelGearRatio->scale(0.6f, 0.6f);

    Label* labelRPM = gui->addLabel(font, "0");
    labelRPM->setPosition(10, 40);
    labelRPM->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelRPM->scale(0.6f, 0.6f);

    Label* labelThrottle = gui->addLabel(font, "0");
    labelThrottle->setPosition(10, 25);
    labelThrottle->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelThrottle->scale(0.6f, 0.6f);

    Label* labelTorque = gui->addLabel(font, "0");
    labelTorque->setPosition(10, 10);
    labelTorque->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelTorque->scale(0.6f, 0.6f);*/

    Label* labelBusStop = gui->addLabel(font, "");
    labelBusStop->setPosition(450, 730);
    labelBusStop->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelBusStop->scale(0.6f, 0.6f);

    Label* labelBusStop2 = gui->addLabel(font, "");
    labelBusStop2->setPosition(450, 705);
    labelBusStop2->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelBusStop2->scale(0.6f, 0.6f);

    /*Label* labelPassengers = gui->addLabel(font, "Liczba pasazerow: " + toString(bus->getNumberOfPassengers()));
    labelPassengers->setPosition(10, 120);
    labelPassengers->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelPassengers->scale(0.6f, 0.6f);

    Label* labelPassengersGettingOff = gui->addLabel(font, "Liczba pasazerow wysiadajacych: " + toString(bus->getNumberOfPassengersGettingOff()));
    labelPassengersGettingOff->setPosition(10, 105);
    labelPassengersGettingOff->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    labelPassengersGettingOff->scale(0.6f, 0.6f);*/


	Hud hud(gui, bus);


    sndMgr->setActiveCamera(camFPS);

    // Time calculation variables
    double deltaTime = 0.0;
    double accumulator = 0.0;

    const double TIME_STEP = 1/60.0f;
    const double MAX_ACCUMULATED_TIME = 1.0;

    double timePhysicsCurr;
    double timePhysicsPrev;
    timePhysicsPrev = timePhysicsCurr = glfwGetTime();

    //how long ago FPS counter was updated
    double lastFPSupdate = timePhysicsCurr;

    double xpos, ypos;

    int nbFrames = 0;

    physMgr->play();
    sndMgr->setMute(false);

    // ========== MAIN LOOP START ==========
    while (win->isOpened())
    {
        nbFrames++;


        timePhysicsCurr = glfwGetTime();
        deltaTime = timePhysicsCurr - timePhysicsPrev;
        timePhysicsPrev = timePhysicsCurr;

        deltaTime = std::max(0.0, deltaTime);
        accumulator += deltaTime;
        accumulator = clamp(accumulator, 0.0, MAX_ACCUMULATED_TIME);

        //create string from frame/sec and display it in window title
        if ( timePhysicsCurr - lastFPSupdate >= 1.0f )
        {
            // Convert the fps value into a string using an output stringstream
			std::string sTiming = toString(nbFrames);

			// Append the FPS value to the window title details
			std::string newWindowTitle = winTitle + " | FPS: " + sTiming;
			win->setWindowTitle(newWindowTitle);
			//label->setText(newWindowTitle);

            nbFrames = 0;
            lastFPSupdate += 1.0f;
        }


        if (cameraControll)
        {
            glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
            glfwSetCursorPos(win->getWindow(), win->getWidth()/2, win->getHeight()/2);

            camFPS->setRotation(xpos, ypos);
        }


        readInput(win->getWindow(), deltaTime);


        while ( accumulator > TIME_STEP )
        {
            physMgr->simulate(TIME_STEP);
            bus->update(TIME_STEP);
            GraphicsManager::getInstance().update(TIME_STEP);
            BusStopSystem::getInstance().update(TIME_STEP, bus);

            accumulator -= TIME_STEP;
        }

        sndMgr->update();

		hud.update();

        /*labelSpeed->setText("Speed: " + toString((int)bus->getBusSpeed()) + "km/h");
        labelEngineIsRunning->setText(bus->getEngine()->isRunning() ? "Engine on" : "Engine off");
        labelGearRatio->setText("Gear ratio: " + toString(bus->getGearbox()->currentRatio()));
        labelRPM->setText("RPM: " + toString(bus->getEngine()->getCurrentRPM()));
        labelThrottle->setText("Throttle: " + toString(bus->getEngine()->getThrottle()));
        labelTorque->setText("Torque: " + toString(bus->getEngine()->getCurrentTorque()));*/

        BusStopSystem& busStopSystem = BusStopSystem::getInstance();
        if (busStopSystem.getCurrentBusStop() != NULL)
        {
            labelBusStop->setText(busStopSystem.getCurrentBusStop()->getName() + " (" + toString((int)busStopSystem.getDistanceToCurrentBusStop()) + "m)");
            labelBusStop2->setText("Liczba pasazerow: " + toString(busStopSystem.getCurrentBusStop()->getNumberOfPassengers()));

            //labelPassengers->setText("Liczba pasazerow w autobusie: " + toString(bus->getNumberOfPassengers()));
            //labelPassengersGettingOff->setText("Liczba pasazerow wysiadajacych: " + toString(bus->getNumberOfPassengersGettingOff()));
        }
        else
        {
            labelBusStop->setText("");
            labelBusStop2->setText("");
        }

        // Render the scene
        renderer.renderAll();

        // Render GUI
        renderer.renderGUI(gui->getGUIRenderList());

		// ImGUI
		#ifdef DRAW_IMGUI
		drawImGui();
		#endif // DRAW_IMGUI

        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    physMgr->stop();

    glfwSetInputMode(win->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    delete gui;

    bus->drop();

    sndMgr->drop();
    physMgr->drop();
    delete sceneMgr;

	#ifdef DRAW_IMGUI
	destroyImGuiContext();
	#endif // DRAW_IMGUI

    delete win;

    return 0;
}
