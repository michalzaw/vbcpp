#include <iostream>
#include <ctime>
#include <iomanip>

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
#include "Game.h"

#define DRAW_IMGUI true
#ifdef DRAW_IMGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#endif // DRAW_IMGUI
#include <thread>


bool showImGui = false;
bool isResourcesLoaded = false;


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

    

    

	

	

	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
	{
		showImGui = !showImGui;
	}
}




// Callback dla pojedynczych zdarzeń - przyciski myszy
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    
}


// Bufforowane przetwarzanie zdarzeń


void initializeImGui()
{
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(win->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
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


void MyThread(GLFWwindow* win)
{
	glfwMakeContextCurrent(win);
	glewInit();

	SceneLoader sceneLoader(sceneMgr);
	sceneLoader.loadMap(GameConfig::getInstance().mapFile);

	isResourcesLoaded = true;
}


int main()
{
	Game game;
	game.initialize();
	game.run();
	game.terminate();
}


// ### MAIN ###
int main2(int argc, char** argv)
{

    


    

    

	#ifdef DRAW_IMGUI
	initializeImGui();
	#endif // DRAW_IMGUI


    // Inicjalizujemy potrzebne rzeczy
    


	


	

	//std::thread thread(MyThread, win->getWindow2());

	

    //sceneMgr->loadScene(GameConfig::getInstance().mapFile);

    

    // Camera FPS
    




    GUIManager* gui = new GUIManager;

    Image* img = gui->addImage(ResourceManager::getInstance().loadTexture("logo.jpg"));
    img->setPosition(GameConfig::getInstance().windowWidth / 2.0f - img->getTexture()->getSize().x / 2.0f, GameConfig::getInstance().windowHeight / 2.0f - img->getTexture()->getSize().y / 2.0f);

    RFont* font = ResourceManager::getInstance().loadFont("fonts/arial.ttf");

	Hud hud(gui, bus);



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
	int fps = 0;
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

			fps = nbFrames;

            nbFrames = 0;
            lastFPSupdate += 1.0f;
        }


        if (cameraControll)
        {
            glfwGetCursorPos(win->getWindow(), &xpos, &ypos);
            glfwSetCursorPos(win->getWindow(), win->getWidth()/2, win->getHeight()/2);

            camFPS->setRotation(xpos, ypos);
        }


        //readInput(win->getWindow(), deltaTime);


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

        

        // Render the scene
		//if (isResourcesLoaded)
		{
			//renderer.renderAll();
			img->setIsActive(false);
		}

        // Render GUI
        //renderer.renderGUI(gui->getGUIRenderList());

		// ImGUI
		#ifdef DRAW_IMGUI
		drawImGui();
		#endif // DRAW_IMGUI

        // Swap buffers and poll events
        win->swapBuffers();
        win->updateEvents();
    }
    // ========== MAIN LOOP END ==========

    

	//thread.join();

    return 0;
}
