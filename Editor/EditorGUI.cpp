#include "EditorGUI.h"


EditorGUI::EditorGUI(std::shared_ptr<Window> window, SceneManager* sceneManager)
    : _window(window), _sceneManager(sceneManager),
    _selectedSceneObject(nullptr),
    _styleDark(true),
    _showObjectProperties(true),
    _showCameraFPSSettings(false),
    _showAddSceneObjectDialog(false),
    _showFileIODialog(false),
    _showDemo(false),
    _showTestWindow(false)
{
    initializeImGui();
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


void EditorGUI::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    drawMainMenu();

    if (_showDemo)
    {
        ImGui::ShowTestWindow();
    }

    if (_showTestWindow)
    {
        drawTestWindow();
    }

    drawSceneGraph();

    if (_showObjectProperties)
    {
        drawObjectProperties();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void EditorGUI::drawMainMenu()
{
    if( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu("File") )
        {
            if ( ImGui::MenuItem("New", "CTRL+N") ) {}
            ImGui::Separator();
            if ( ImGui::MenuItem("Open", "CTRL+O") ) {}
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
            ImGui::MenuItem("Object Properties", NULL, &_showObjectProperties);
            ImGui::MenuItem("CameraFPS Settings", NULL, &_showCameraFPSSettings);
            ImGui::MenuItem("Demo", NULL, &_showDemo);
            ImGui::MenuItem("Test", NULL, &_showTestWindow);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objects"))
        {
            if (ImGui::MenuItem("Add Scene Object...", NULL) ) { _showAddSceneObjectDialog = true; };
            if (ImGui::MenuItem("File IO Dialog...", NULL) ) { _showFileIODialog = true; };
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void EditorGUI::drawTestWindow()
{
    ImGui::Begin("Test window", &_showTestWindow);

    ImGui::Text("Hello, world %d", 123);
    if (ImGui::Button("Save"))
    {
        // do stuff
    }
    char buf[1024];
    float f;
    ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    ImGui::End();
}


void EditorGUI::drawSceneGraph()
{
    if (ImGui::Begin("Scene Graph"))
    {
        std::list<SceneObject*>& sceneObjects = _sceneManager->getSceneObjects();
        for (std::list<SceneObject*>::iterator i = sceneObjects.begin(); i != sceneObjects.end(); ++i)
        {
            if ((*i)->getParent() == nullptr)
                inspectSceneObject(*i);
        }
    }
    ImGui::End();
}


void EditorGUI::inspectSceneObject(SceneObject* object)
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (object->getChildren().empty())
        node_flags = node_flags | ImGuiTreeNodeFlags_Leaf;
    if (object == _selectedSceneObject)
        node_flags = node_flags | ImGuiTreeNodeFlags_Selected;

    ImGui::PushID(object);
    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)object, node_flags, object->getName().c_str());

    if (ImGui::IsItemClicked())
        _selectedSceneObject = object;
    if (node_open)
    {
        std::list<SceneObject*>& children = object->getChildren();
        for (std::list<SceneObject*>::iterator i = children.begin(); i != children.end(); ++i)
        {
            SceneObject* child = *i;

            ImGui::PushID(child);

            inspectSceneObject(child);

            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}


void EditorGUI::drawObjectProperties()
{
    if (ImGui::Begin("Object Properties", &_showObjectProperties))
    {
        if (_selectedSceneObject)
        {
            // Object's name
            char buffer[50];

            strncpy(buffer, _selectedSceneObject->getName().c_str(), sizeof buffer);

            buffer[sizeof buffer - 1] = '\0';

            if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer),
                                    ImGuiInputTextFlags_EnterReturnsTrue ) )
            {
                //_selectedSceneObject->setName(std::string(buffer));
            }

            ImGui::Separator();

            // Objects transformation
            ImGui::Text("Transformation");

            // Position
            glm::vec3 position = _selectedSceneObject->getPosition();
            if (ImGui::DragFloat3("Position", (float*)&position, 0.01f, 0.0f, 0.0f))
            {
                _selectedSceneObject->setPosition(position);
            }

            // Rotation
            glm::vec3 rotation = _selectedSceneObject->getRotation();
            if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.1f, 0.0f, 0.0f))
            {
                _selectedSceneObject->setRotation(rotation);
            }

            // Scale
            glm::vec3 scale = _selectedSceneObject->getScale();
            if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f, 0.0f, 0.0f))
            {
                _selectedSceneObject->setScale(scale);
            }

            ImGui::Separator();
            ImGui::Text("COMPONENTS");

            Component* component;
            // CT_RENDER_OBJECT
            component = _selectedSceneObject->getComponent(CT_RENDER_OBJECT);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Render Component"))
                {

                }

            }

            // CT_CAMERA
            component = _selectedSceneObject->getComponent(CT_CAMERA);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Camera Component"))
                {

                }

            }

            // CT_LIGHT
            component = _selectedSceneObject->getComponent(CT_LIGHT);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Light Component"))
                {

                }

            }

            // CT_PHYSICAL_BODY
            component = _selectedSceneObject->getComponent(CT_PHYSICAL_BODY);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Physical Body Component"))
                {

                }

            }

            // CT_TREE_COMPONENT
            component = _selectedSceneObject->getComponent(CT_TREE_COMPONENT);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Tree Component"))
                {

                }

            }

            // CT_SOUND
            component = _selectedSceneObject->getComponent(CT_SOUND);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Sound Component"))
                {

                }

            }

            // CT_GRASS
            component = _selectedSceneObject->getComponent(CT_GRASS);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Grass Component"))
                {

                }

            }

            // CT_ENVIRONMENT_CAPTURE_COMPONENT
            component = _selectedSceneObject->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Environment capture Component"))
                {

                }

            }

            // CT_MIRROR
            component = _selectedSceneObject->getComponent(CT_MIRROR);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Mirror Component"))
                {

                }

            }

            // CT_CLICKABLE_OBJECT
            component = _selectedSceneObject->getComponent(CT_CLICKABLE_OBJECT);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Clickable object Component"))
                {

                }

            }

            // CT_BUS_STOP
            component = _selectedSceneObject->getComponent(CT_BUS_STOP);
            if (component != nullptr)
            {
                if (ImGui::CollapsingHeader("Bus Stop Component"))
                {

                }

            }

            /*if (_selectedObject->getRenderComponent() )
            {
                if (ImGui::CollapsingHeader("Render Component"))
                {
                    if (_selectedObject->getRenderComponent()->getMesh())
                    {
                        //ImGui::Text("Mesh");
                        unsigned int vertexCount = _selectedObject->getRenderComponent()->getMesh()->getVertexCount();
                        ImGui::Text("Vertex count: %d", vertexCount);
                    }

                    if (_selectedObject->getRenderComponent()->getTexture())
                    {
                        ImGui::Text("Texture image: %s",
                            _selectedObject->getRenderComponent()->getTexture()->getImage()->getName().c_str());

                        ImTextureID texture =
                            reinterpret_cast<void*>(_selectedObject->getRenderComponent()->getTexture()->getID());

                        ImVec2 textureSize = ImVec2(128.0f, 128.0f);
                        ImGui::Image(texture, textureSize, ImVec2(0,0), ImVec2(1,1),
                                        ImColor(255,255,255,255), ImColor(255,255,255,128)
                                    );
                    }
                }
            }*/
        }
    }
    ImGui::End();
}
