#include "EditorGUI.h"

#include "../Graphics/Renderer.h"


EditorGUI::EditorGUI(std::shared_ptr<Window> window, SceneManager* sceneManager, std::shared_ptr<EditorContext> editorContext)
    : _window(window), _sceneManager(sceneManager), _editorContext(editorContext),
    _selectedSceneObject(nullptr),
    _styleDark(true),
    _showDemo(false)
{
    initializeImGui();

    _sceneGraphWindow.reset(new SceneGraphWindow(_sceneManager, _selectedSceneObject));
    _objectPropertiesWindow.reset(new ObjectPropertiesWindow(_sceneManager, _selectedSceneObject));
    _newDialogWindow.reset(new NewDialogWindow(_sceneManager, _selectedSceneObject, &_events, true));
    _openDialogWindow.reset(new OpenDialogWindow(_sceneManager, _selectedSceneObject, &_events));
    _cameraSettingsWindow.reset(new CameraSettingsWindow(_sceneManager, _selectedSceneObject));
	_objectDefinitionsWindow.reset(new ObjectDefinitionsWindow(_sceneManager, _selectedSceneObject, _editorContext));
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

    drawMainMenu();

    if (_showDemo)
        ImGui::ShowTestWindow();

    _sceneGraphWindow->draw();
    _objectPropertiesWindow->draw();
    _newDialogWindow->draw();
    _openDialogWindow->draw();
    _cameraSettingsWindow->draw();
	_objectDefinitionsWindow->draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void EditorGUI::drawMainMenu()
{
    if( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu("File") )
        {
            if ( ImGui::MenuItem("New", "CTRL+N") ) { _newDialogWindow->open(); }
            ImGui::Separator();
            if ( ImGui::MenuItem("Open", "CTRL+O") ) { _openDialogWindow->open(); }
            if ( ImGui::MenuItem("Save", "CTRL+S") ) {}
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
            ImGui::MenuItem("Object Properties", NULL, _objectPropertiesWindow->getOpenFlagPointer());
            ImGui::MenuItem("Camera Settings", NULL, _cameraSettingsWindow->getOpenFlagPointer());
            ImGui::MenuItem("Demo", NULL, &_showDemo);
            //ImGui::MenuItem("Test", NULL, &_showTestWindow);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objects"))
        {
			if (ImGui::BeginMenu("Add Scene Object..."))
			{
				for (const std::string& object : _editorContext->availableObjects)
				{
					if (ImGui::MenuItem(object.c_str(), NULL))
					{
						_events.push_back(EditorEvent(EET_ADD_OBJECT, object));
					}
				}
				ImGui::EndMenu();
			}
			ImGui::MenuItem("Available objects", NULL, _objectDefinitionsWindow->getOpenFlagPointer());
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
