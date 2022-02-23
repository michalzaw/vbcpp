#include "EditorGUI.h"

#include "../Graphics/Renderer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "glm/gtc/type_ptr.hpp"

EditorGUI::EditorGUI(Window* window, SceneManager* sceneManager/*, std::shared_ptr<EditorContext> editorContext*/)
	: _window(window), _sceneManager(sceneManager),// , _editorContext(editorContext),
    _selectedSceneObject(nullptr),
    _styleDark(true),
    _showDemo(false)
{
    initializeImGui();

    //_sceneGraphWindow.reset(new SceneGraphWindow(_sceneManager, _selectedSceneObject));
    //_objectPropertiesWindow.reset(new ObjectPropertiesWindow(_sceneManager, _selectedSceneObject, &_events));
    //_newDialogWindow.reset(new NewDialogWindow(_sceneManager, _selectedSceneObject, &_events));
    //_openDialogWindow.reset(new OpenDialogWindow(_sceneManager, _selectedSceneObject, &_events, true));
    //_cameraSettingsWindow.reset(new CameraSettingsWindow(_sceneManager, _selectedSceneObject));
	//_objectDefinitionsWindow.reset(new ObjectDefinitionsWindow(_sceneManager, _selectedSceneObject, _editorContext));
}


EditorGUI::~EditorGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void EditorGUI::initializeImGui()
{
    ImGui::CreateContext();

    if (_styleDark)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(_window->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("fonts/arial.ttf", 13.0f);
    io.Fonts->AddFontDefault();
}


void EditorGUI::setSelectedSceneObject(SceneObject* sceneObject)
{
    _selectedSceneObject = sceneObject;

	//_sceneGraphWindow->centerGraphView();
}


SceneObject* EditorGUI::getSelectedSceneObject()
{
    return _selectedSceneObject;
}


bool EditorGUI::hasNextEvent()
{
    return !_events.empty();
}


EditorEvent EditorGUI::getNextEvent()
{
    EditorEvent event = _events.front();

    _events.pop_front();

    return event;
}


bool EditorGUI::GUIhasFocus()
{
    return (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard);
}


void EditorGUI::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

	ImGuizmo::BeginFrame();
	
	SceneObject* camObj = _sceneManager->getSceneObject("editor#cam1");
	CameraFPS* cam = dynamic_cast<CameraFPS*>(camObj->getComponent(CT_CAMERA));

	glm::mat4 cubepos = glm::mat4(1.);

	if (_selectedSceneObject)
		ShowTransformGizmo(cam, _selectedSceneObject);
	//ImGuizmo::DrawCube(glm::value_ptr(cam->getViewMatrix()), glm::value_ptr(cam->getProjectionMatrix()), glm::value_ptr(cubepos));

	
    drawMainMenu();

    if (_showDemo)
        ImGui::ShowDemoWindow();

	//showObjectProperties();

    //_sceneGraphWindow->draw();
    //_objectPropertiesWindow->draw();
    //_newDialogWindow->draw();
    //_openDialogWindow->draw();
    //_cameraSettingsWindow->draw();
	//_objectDefinitionsWindow->draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void EditorGUI::drawMainMenu()
{
    if( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu("File") )
        {
            //if ( ImGui::MenuItem("New", "CTRL+N") ) { _newDialogWindow->open(); }
            //ImGui::Separator();
            //if ( ImGui::MenuItem("Open", "CTRL+O") ) { _openDialogWindow->open(); }
            //if ( ImGui::MenuItem("Save", "CTRL+S") ) { _events.push_back(EditorEvent(EET_SAVE_CLICK, _editorContext->currentMapInfo.name));}
            if ( ImGui::MenuItem("Save as...", "CTRL+SHIFT+S") ) {}
            ImGui::Separator();
            if ( ImGui::MenuItem("Exit") ) {}
            ImGui::EndMenu();
        }
        if ( ImGui::BeginMenu("Edit") )
        {
            if ( ImGui::MenuItem("Undo", "CTRL+Z") ) {}
            if ( ImGui::MenuItem("Redo", "CTRL+Y", false, false) ) {}  // Disabled item
            ImGui::Separator();
            if ( ImGui::MenuItem("Cut", "CTRL+X") ) {}
            if ( ImGui::MenuItem("Copy", "CTRL+C") ) {}
            if ( ImGui::MenuItem("Paste", "CTRL+V") ) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            //ImGui::MenuItem("Object Properties", NULL, _objectPropertiesWindow->getOpenFlagPointer());
            //ImGui::MenuItem("Camera Settings", NULL, _cameraSettingsWindow->getOpenFlagPointer());
            ImGui::MenuItem("Demo", NULL, &_showDemo);
            //ImGui::MenuItem("Test", NULL, &_showTestWindow);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objects"))
        {
			
			if (ImGui::BeginMenu("Add Scene Object..."))
			{
				/*
				for (const std::string& object : _editorContext->availableObjects)
				{
					if (ImGui::MenuItem(object.c_str(), NULL))
					{
						_events.push(EditorEvent(EET_ADD_OBJECT, object));
					}
				}
				*/
				ImGui::EndMenu();
			}
			//ImGui::MenuItem("Available objects", NULL, _objectDefinitionsWindow->getOpenFlagPointer());
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
/*
void EditorGUI::showObjectProperties()
{
	glm::uvec2 mainWindowSize(Renderer::getInstance().getWindowDimensions());

	ImGui::SetNextWindowSize(ImVec2(200, mainWindowSize.y - 18), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(100, mainWindowSize.y - 18), ImVec2(500, mainWindowSize.y - 18));

	bool isOpened = true;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Object Properties", &isOpened, windowFlags))
	{
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImGui::SetWindowPos(ImVec2(mainWindowSize.x - windowSize.x, 18));

		if (_selectedSceneObject)
		{
			showObjectNameEdit();

			ImGui::Separator();

			showObjectTransformEdit();

			ImGui::Separator();

			RenderObject* renderComponent = dynamic_cast<RenderObject*>(_selectedSceneObject->getComponent(CT_RENDER_OBJECT));
			if (renderComponent)
			{
				if (ImGui::CollapsingHeader("Render Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Material* material = renderComponent->getMaterial(0);
					ImGui::Text("Material name: %s", material->name.c_str());
						
//					if (_selectedObject->getRenderComponent()->getMesh())
//					{
//						//ImGui::Text("Mesh");
//						unsigned int vertexCount = _selectedObject->getRenderComponent()->getMesh()->getVertexCount();
//						ImGui::Text("Vertex count: %d", vertexCount);
//					}
//
//					if (_selectedObject->getRenderComponent()->getTexture())
//					{
//						ImGui::Text("Texture image: %s",
//							_selectedObject->getRenderComponent()->getTexture()->getImage()->getName().c_str());
//
//						ImTextureID texture =
//							reinterpret_cast<void*>(_selectedObject->getRenderComponent()->getTexture()->getID());
//
//						ImVec2 textureSize = ImVec2(128.0f, 128.0f);
//						ImGui::Image(texture, textureSize, ImVec2(0, 0), ImVec2(1, 1),
//							ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128)
//						);
						
				}
			}

			PhysicalBody* physicsComponent = dynamic_cast<PhysicalBody*>(_selectedSceneObject->getComponent(CT_PHYSICAL_BODY));
			if (physicsComponent)
			{
				if (ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
				{

				}
			}
		}
	}
	ImGui::End();
}

void EditorGUI::showObjectNameEdit()
{
	// Object's name
	char buffer[50];

	strncpy(buffer, _selectedSceneObject->getName().c_str(), sizeof buffer);

	buffer[sizeof buffer - 1] = '\0';

	if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		_selectedSceneObject->setName(std::string(buffer));
	}
}

void EditorGUI::showObjectTransformEdit()
{
	// Objects transformation
	ImGui::Text("Transformation");
	SceneObject* camObj = _sceneManager->getSceneObject("editor#cam1");
	CameraFPS* cam = dynamic_cast<CameraFPS*>(camObj->getComponent(CT_CAMERA));

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_selectedSceneObject->getGlobalTransformMatrix()), matrixTranslation, matrixRotation, matrixScale);

	ImGui::DragFloat3("Position", matrixTranslation, 0.01f, 0.0f, 0.0f);
	ImGui::DragFloat3("Rotation", matrixRotation, 0.01f, 0.0f, 0.0f);
	ImGui::DragFloat3("Scale", matrixScale, 0.01f, 0.0f, 0.0f);

	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(_selectedSceneObject->getGlobalTransformMatrix()));

	//ImGuizmo::DrawCube(glm::value_ptr(cam->getViewMatrix()), glm::value_ptr(cam->getProjectionMatrix()), glm::value_ptr(_selectedSceneObject->getGlobalTransformMatrix()));
}
*/
void EditorGUI::ShowTransformGizmo(CameraStatic* camera, SceneObject* obj)
{
	
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (ImGui::IsKeyPressed(69)) // E key
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(82)) // R key
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(84)) // T key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	/*
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	glm::mat4 objMat = glm::mat4(1.);
	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(obj->getGlobalTransformMatrix()), matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation, 3);
	ImGui::InputFloat3("Rt", matrixRotation, 3);
	ImGui::InputFloat3("Sc", matrixScale, 3);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(obj->getGlobalTransformMatrix()));
	*/
	/*
	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	*/
	
	static bool useSnap(false);
	
	if (ImGui::IsKeyPressed(80))
		useSnap = !useSnap;
	ImGui::Checkbox("", &useSnap);
	
	ImGui::SameLine();
	
	//vec_t snap;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };

	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		//snap = config.mSnapTranslation;
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		//snap = config.mSnapRotation;
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		//snap = config.mSnapScale;
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}

	ImGui::Checkbox("Bound Sizing", &boundSizing);
	if (boundSizing)
	{
		ImGui::PushID(3);
		ImGui::Checkbox("", &boundSizingSnap);
		ImGui::SameLine();
		ImGui::InputFloat3("Snap", boundsSnap);
		ImGui::PopID();
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate( glm::value_ptr(camera->getViewMatrix()), glm::value_ptr(camera->getProjectionMatrix()), 
							mCurrentGizmoOperation, mCurrentGizmoMode, 
							glm::value_ptr(obj->getGlobalTransformMatrix()), 
							NULL, 
							useSnap ? &snap[0] : NULL, 
							boundSizing ? bounds : NULL, 
							boundSizingSnap ? boundsSnap : NULL
	);
}
