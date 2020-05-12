#include "ObjectPropertiesWindow.h"

#include "../../ImGui/imGizmo.h"
#include "glm/gtc/type_ptr.hpp"

#include "../../ImGui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../../ImGui/imgui_internal.h"

#include "RoadTools.h"

ObjectPropertiesWindow::ObjectPropertiesWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen)
    : EditorWindow(sceneManager, selectedSceneObject, isOpen, events)
{

}


void ObjectPropertiesWindow::drawWindow()
{
	/*
    glm::uvec2 mainWindowSize(Renderer::getInstance().getWindowDimensions());

    ImGui::SetNextWindowSize(ImVec2(200, mainWindowSize.y - 18), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, mainWindowSize.y - 18), ImVec2(500, mainWindowSize.y - 18));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    if (ImGui::Begin("Object Properties", &_isOpen, windowFlags))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2(mainWindowSize.x - windowSize.x, 18));

        if (_selectedSceneObject)
        {
            // Object's name
            char buffer[50];

            strncpy(buffer, _selectedSceneObject->getName().c_str(), sizeof buffer);

            buffer[sizeof buffer - 1] = '\0';

            if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                _selectedSceneObject->setName(std::string(buffer));
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
			*/
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

			//ImGui::Separator();

			/*if (ImGui::Button("Delete"))
			{
				_events->push_back(EditorEvent(EET_SELECTED_OBJECT_DELETE_CLICK));
			}*/
        //}
    //}
    //ImGui::End();
}

namespace vbEditor
{
	extern SceneObject* _selectedSceneObject;
	extern CameraFPS* _camera;

	extern int roadActiveSegment;
}

void showObjectProperties()
{
	glm::uvec2 mainWindowSize(Renderer::getInstance().getWindowDimensions());

	ImGui::SetNextWindowSize(ImVec2(200, mainWindowSize.y - 18), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(100, mainWindowSize.y - 18), ImVec2(500, mainWindowSize.y - 18));

	ImGuiWindow* window = ImGui::GetCurrentWindow();

	bool isOpened = true;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Object Properties", &isOpened, windowFlags))
	{
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImGui::SetWindowPos(ImVec2(mainWindowSize.x - windowSize.x, 18));

		if (vbEditor::_selectedSceneObject)
		{
			showObjectNameEdit();

			ImGui::Separator();

			showObjectTransformEdit();

			ImGui::Separator();

			RenderObject* renderComponent = dynamic_cast<RenderObject*>(vbEditor::_selectedSceneObject->getComponent(CT_RENDER_OBJECT));
			if (renderComponent)
			{
				if (ImGui::CollapsingHeader("Render Component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Material* material = renderComponent->getMaterial(0);
					ImGui::Text("Material name: %s", material->name.c_str());
				}
			}

			PhysicalBody* physicsComponent = dynamic_cast<PhysicalBody*>(vbEditor::_selectedSceneObject->getComponent(CT_PHYSICAL_BODY));
			if (physicsComponent)
			{
				if (ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
				{

				}
			}

			RoadObject* roadComponent = dynamic_cast<RoadObject*>(vbEditor::_selectedSceneObject->getComponent(CT_ROAD_OBJECT));
			if (roadComponent)
			{
				if (ImGui::CollapsingHeader("Road", ImGuiTreeNodeFlags_DefaultOpen))
				{
					// left
					static float actualWinSize = Renderer::getInstance().getWindowDimensions().x;
					static int selected = 0;
					ImGui::BeginChild("left pane", ImVec2((ImGui::GetWindowSize().x * (0.4f)), 90), true);

					ImVec2 range = ImVec2(0.0f, 0.0f);

					RRoadProfile* profile = roadComponent->getRoadProfile();
					std::vector<RoadLane> lanes = profile->getRoadLanes();

					for (int i = 0; i < lanes.size(); i++)
					{
						char label[128];
						sprintf(label, "%d: %s##%d", i, lanes[i].material.name.c_str(), i);
						//sprintf(label, "MyObject %d", i);
						if (ImGui::Selectable(label, selected == i))
							selected = i;
					}
					ImGui::EndChild();
					ImGui::SameLine();

					//std::string pos = std::string("X: ");
					ImGui::Text("X: %f, Y: %f", lanes[selected].r1, lanes[selected].r2);
					//ImGui::LabelText(pos.c_str());

					//static float values[4] = { 0.0, 1.0, 0.0, -1.0 };
					
					static std::vector<ImVec2> values = { ImVec2(-5.0f, -5.0f), ImVec2(0.0f, 0.0f) }; //, ImVec2(0.0f, 0.0f), ImVec2(0.5f, 0.5f)};
				
					static int values_offset = 0;
					static double refresh_time = 0.0;
					/*
						static float phase = 0.0f;
						values[values_offset] = cosf(phase);
						values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
						phase += 0.10f*values_offset;
						refresh_time += 1.0f / 60.0f;
					*/
					
					//ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(0, 100));

					ImGui::RoadProfileGraph("Profile", values, -5.0f, 5.0f, ImVec2(150, 150));

					std::vector<RoadSegment> segments = roadComponent->getSegments();
					RoadSegment segment = segments[0];
					//segment.begin = glm::vec3(0.0f, 0.0, 0.0f);

					glm::vec3 pos = segment.begin;
					ImGui::Text("Road Pos: %f, %f, %f", pos.x, pos.y, pos.z);

					//ImColor color(ImGui::GetStyle().Colors[ImGuiCol_PlotLines]);

					//glm::vec4 transPos = vbEditor::_camera->getProjectionMatrix() * vbEditor::_camera->getViewMatrix() * vbEditor::_selectedSceneObject->getGlobalTransformMatrix() * glm::vec4(pos, 1.0);

					//window->DrawList->AddCircleFilled(ImVec2(transPos.x, transPos.y), 14.0f, color);

					if (ImGui::Button("Update road"))
					{
						RoadSegment seg;
						seg.begin = glm::vec3(50, 20, 100);
						seg.end = segment.end;
						seg.interpolation = segment.interpolation;
						seg.pointsCount = segment.pointsCount;
						seg.r = segment.r;
						seg.type = segment.type;
						segments[0] = seg;

						roadComponent->setSegments(segments);

						roadComponent->buildModel();
					}
				}
				if (ImGui::CollapsingHeader("Road segment", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (vbEditor::roadActiveSegment >= 0 && vbEditor::roadActiveSegment < roadComponent->getSegments().size())
					{
						ImGui::Text("Segment: %d", vbEditor::roadActiveSegment);

						int typeComboCurrentItem = roadComponent->getSegments()[vbEditor::roadActiveSegment].type == RST_LINE ? 0 : 1;
						const char* typeComboItems[] = { "line", "arc" };
						ImGui::Combo("Type", &typeComboCurrentItem, typeComboItems, IM_ARRAYSIZE(typeComboItems));
						roadComponent->getSegments()[vbEditor::roadActiveSegment].type = typeComboCurrentItem == 0 ? RST_LINE : RST_ARC;

						int interpolationComboCurrentItem = roadComponent->getSegments()[vbEditor::roadActiveSegment].interpolation == RI_LIN ? 0 : 1;
						const char* interpolationComboItems[] = { "lin", "cos" };
						ImGui::Combo("Interpolation", &interpolationComboCurrentItem, interpolationComboItems, IM_ARRAYSIZE(interpolationComboItems));
						roadComponent->getSegments()[vbEditor::roadActiveSegment].interpolation = interpolationComboCurrentItem == 0 ? RI_LIN : RI_COS;

						int* points = &(roadComponent->getSegments()[vbEditor::roadActiveSegment].pointsCount);
						ImGui::DragInt("Points count", points, 5.0f, 0.0f, 0.0f);

						float* radius = &(roadComponent->getSegments()[vbEditor::roadActiveSegment].r);
						ImGui::DragFloat("Radius", radius, 0.01f, 0.0f, 0.0f);

						// radius validation
						float beginToEndHalfDistance = glm::length(roadComponent->getSegments()[vbEditor::roadActiveSegment].end - roadComponent->getSegments()[vbEditor::roadActiveSegment].begin) / 2.0f;
						if (abs(roadComponent->getSegments()[vbEditor::roadActiveSegment].r) < beginToEndHalfDistance)
						{
							roadComponent->getSegments()[vbEditor::roadActiveSegment].r = beginToEndHalfDistance * sign(roadComponent->getSegments()[vbEditor::roadActiveSegment].r);
						}
					}
				}
			}
		}
	}
	ImGui::End();
}


void showObjectNameEdit()
{
	char buffer[50];

	strncpy(buffer, vbEditor::_selectedSceneObject->getName().c_str(), sizeof buffer);

	buffer[sizeof buffer - 1] = '\0';

	if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		vbEditor::_selectedSceneObject->setName(std::string(buffer));
	}
}


void showObjectTransformEdit()
{
	ImGui::Text("Transformation");

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(vbEditor::_selectedSceneObject->getLocalTransformMatrix()), matrixTranslation, matrixRotation, matrixScale);

	ImGui::DragFloat3("Position", matrixTranslation, 0.01f, 0.0f, 0.0f);
	ImGui::DragFloat3("Rotation", matrixRotation, 0.01f, 0.0f, 0.0f);
	ImGui::DragFloat3("Scale", matrixScale, 0.01f, 0.0f, 0.0f);

	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(vbEditor::_selectedSceneObject->getLocalTransformMatrix()));

	vbEditor::_selectedSceneObject->updateFromLocalMatrix();
}