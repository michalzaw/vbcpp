#include "ObjectPropertiesWindow.h"

#include <numeric>

#include "../../ImGui/imGizmo.h"
#include "glm/gtc/type_ptr.hpp"

#include "../../ImGui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../../ImGui/imgui_internal.h"

#include "RoadTools.h"

#include "../../Graphics/ShapePolygonComponent.h"


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
	extern SceneManager* _sceneManager;
	extern SceneObject* _selectedSceneObject;
	extern CameraFPS* _camera;

	extern int roadActiveSegment;
	extern int roadActivePoint;
	extern bool isRoadModified;

	extern std::vector<std::string> _availableRoadProfiles;

	extern bool _showMaterialEditorWindow;
	extern RenderObject* currentRenderObject;
	extern RStaticModel* currentStaticModel;
	extern unsigned int currentModelLod;
	extern unsigned int currentMaterialIndex;

	extern bool _showGenerateObjectsAlongRoadWindow;
}


void showRenderComponentDetails(RenderObject* renderComponent)
{
	if (ImGui::CollapsingHeader("Render Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (int i = 0; i < renderComponent->getNumberOfLod(); ++i)
		{
			char id[50];
			sprintf(id, "Lod %d", i);

			ImGui::PushID(id);

			ImGui::Text(id);

			RStaticModel* model = renderComponent->getModel(i);

			char buffer[1024] = { '\0' };
			strncpy(buffer, model->getPath().c_str(), sizeof buffer);
			buffer[sizeof buffer - 1] = '\0';

			ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly);

			ImGui::SameLine();

			if (ImGui::Button("..."))
			{
				/*std::vector<std::string> result = pfd::open_file("Choose texture...").result();

				if (result.size() == 1)
				{
					LOG_INFO(result[0]);

					std::string path = result[0];
					std::string objectDirPath = currentRenderObject->getSceneObject()->getObjectDefinition()->getPath();

					std::string newPath = objectDirPath + FilesHelper::getFileNameFromPath(path);
					if (!FilesHelper::isInPathSubdir(path, objectDirPath))
					{
						FilesHelper::copyFile(path, newPath);
					}
					texture = ResourceManager::getInstance().loadTexture(newPath);

					isMaterialModified = true;
				}*/
			}

			ImGui::Text("Materials");

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;

			for (int j = 0; j < renderComponent->getModel(i)->getMaterialsCount(); ++j)
			{
				Material* material = renderComponent->getModel(i)->getMaterial(j);

				if (ImGui::Selectable(material->name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (ImGui::IsMouseDoubleClicked(0))
					{
						LOG_INFO(material->name);
						vbEditor::_showMaterialEditorWindow = true;
						vbEditor::currentRenderObject = renderComponent;
						vbEditor::currentStaticModel = renderComponent->getModel(i);
						vbEditor::currentModelLod = i;
						vbEditor::currentMaterialIndex = j;
					}
				}
			}

			ImGui::Separator();

			ImGui::PopID();
		}

		bool isCastShadows = renderComponent->isCastShadows();
		if (ImGui::Checkbox("Cast shadows", &isCastShadows))
		{
			renderComponent->setIsCastShadows(isCastShadows);
		}

		bool isDynamicObject = renderComponent->isDynamicObject();
		if (ImGui::Checkbox("Dynamic object", &isDynamicObject))
		{
			renderComponent->setIsDynamicObject(isDynamicObject);
		}
	}
}


void showRoadComponentDetails(RoadObject* roadComponent)
{
	if (ImGui::CollapsingHeader("Road component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int roadProfilesCurrentItem = 0;
		std::vector<const char*> roadProfilesComboItems;
		for (int i = 0; i < vbEditor::_availableRoadProfiles.size(); ++i)
		{
			roadProfilesComboItems.push_back(vbEditor::_availableRoadProfiles[i].c_str());

			if (vbEditor::_availableRoadProfiles[i] == roadComponent->getRoadProfile()->getName())
				roadProfilesCurrentItem = i;
		}
		if (ImGui::Combo("Road profile", &roadProfilesCurrentItem, roadProfilesComboItems.data(), roadProfilesComboItems.size()))
		{
			roadComponent->setRoadProfile(ResourceManager::getInstance().loadRoadProfile(vbEditor::_availableRoadProfiles[roadProfilesCurrentItem]));

			roadComponent->buildModel(false);
		}

		ImGui::Separator();

		if (vbEditor::roadActivePoint >= 0 && vbEditor::roadActivePoint < roadComponent->getPoints().size())
		{
			ImGui::Text("Point: %d", vbEditor::roadActivePoint);

			float* pointPosition = glm::value_ptr(roadComponent->getPoints()[vbEditor::roadActivePoint]);

			if (ImGui::DragFloat3("Point position", pointPosition, 0.01f, 0.0f, 0.0f))
			{
				vbEditor::isRoadModified = true;
			}

			if (vbEditor::roadActivePoint % 3 == 0)
			{
				if (ImGui::Button("Delete point"))
				{
					roadComponent->deletePoint(vbEditor::roadActivePoint);

					if (vbEditor::roadActivePoint == roadComponent->getPoints().size())
					{
						--vbEditor::roadActivePoint;
						--vbEditor::roadActiveSegment;
					}

					if (roadComponent->getPoints().size() == 0)
					{
						vbEditor::_sceneManager->removeSceneObject(roadComponent->getSceneObject());
						vbEditor::_selectedSceneObject = nullptr;

						return;
					}

					vbEditor::isRoadModified = true;
				}
			}
		}

		if (vbEditor::roadActiveSegment >= 0 && roadComponent->getSegments().size() > 0 && vbEditor::roadActiveSegment < roadComponent->getSegments().size())
		{
			ImGui::Separator();

			ImGui::Text("Segment: %d", vbEditor::roadActiveSegment);

			int* points = &(roadComponent->getSegments()[vbEditor::roadActiveSegment].pointsCount);
			if (ImGui::DragInt("Points count", points, 0.5f, 0, 1000))
			{
				vbEditor::isRoadModified = true;
				*points = std::max(*points, 0);
			}
		}

		ImGui::Separator();

		std::vector<CrossroadComponent*> availableCrossroads = vbEditor::_sceneManager->getGraphicsManager()->getCrossroadComponents();
		CrossroadComponent* connectedCrossroads[2] = { roadComponent->getConnectionPoint(0).crossroadComponent, roadComponent->getConnectionPoint(1).crossroadComponent };

		int crossroadCurrentItems[2] = { 0, 0 };
		int connectionPointCurrentItems[2] = { roadComponent->getConnectionPoint(0).index, roadComponent->getConnectionPoint(1).index };

		std::string crossroadsComboItems{};
		crossroadsComboItems += " ";
		crossroadsComboItems += '\0';

		for (int i = 0; i < availableCrossroads.size(); ++i)
		{
			crossroadsComboItems += availableCrossroads[i]->getSceneObject()->getName() + '\0';

			for (int j = 0; j < 2; ++j)
			{
				if (availableCrossroads[i] == connectedCrossroads[j])
				{
					crossroadCurrentItems[j] = i + 1;
				}
			}
		}

		for (int i = 0; i < 2; ++i)
		{
			ImGui::Text("Connection: %d", i);

			ImGui::PushID(i);
			if (ImGui::Combo("Crossroad", &crossroadCurrentItems[i], crossroadsComboItems.c_str()))
			{
				if (crossroadCurrentItems[i] == 0)
				{
					roadComponent->setConnectionPoint(i, nullptr);
				}
				else
				{
					roadComponent->setConnectionPoint(i, availableCrossroads[crossroadCurrentItems[i] - 1], 0);
				}

				vbEditor::isRoadModified = true;
			}

			if (connectedCrossroads[i] != nullptr)
			{
				std::string connectionPointComboItems{};
				for (int j = 0; j < connectedCrossroads[i]->getConnectionsCount(); ++j)
				{
					connectionPointComboItems += toString(j) + '\0';
				}

				if (ImGui::Combo("Index", &connectionPointCurrentItems[i], connectionPointComboItems.c_str()))
				{
					roadComponent->setConnectionPoint(i, availableCrossroads[crossroadCurrentItems[i] - 1], connectionPointCurrentItems[i]);

					vbEditor::isRoadModified = true;
				}
			}
			ImGui::PopID();
		}

		if (ImGui::Button("Generate objects"))
		{
			vbEditor::_showGenerateObjectsAlongRoadWindow = true;
		}
	}
}


void showShapePolygonComponentDetails(ShapePolygonComponent* component)
{
	if (ImGui::CollapsingHeader("Polygon Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Number of points: %d", component->getPoints().size());

		if (ImGui::Button("Generate Triangle Mesh"))
		{
			component->buildAndCreateRenderObject(true);
		}
		if (ImGui::Button("Generate Triangle Mesh (without mesh mender)"))
		{
			component->buildAndCreateRenderObject(false);
		}
	}
}


void showRoadIntersectionComponentDetails(RoadIntersectionComponent* component)
{
	if (ImGui::CollapsingHeader("Road intersection", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Number of connected roads: %d", component->getConnectedRoads().size());
		for (const auto& connectedRoad : component->getConnectedRoads())
		{
			ImGui::BulletText("%s (%d)", connectedRoad.road->getSceneObject()->getName().c_str(), connectedRoad.connectionPointInRoadIndex);
		}

		ImGui::Separator();

		static bool modifyAllConnectedRoads = true;
		ImGui::Checkbox("Modify all connected roads", &modifyAllConnectedRoads);

		int numberOfRoads = modifyAllConnectedRoads ? 1 : component->getConnectedRoads().size();
		for (int i = 0; i < numberOfRoads; ++i)
		{
			ImGui::PushID(i);

			ImGui::Text("#%d", i + 1);

			float length = component->getLength(i);
			if (ImGui::DragFloat("Length", &length, 1.0f, 0.0f, 100.0f))
			{
				component->setLength(i, length, modifyAllConnectedRoads);
			}
			float width = component->getWidth(i);
			if (ImGui::DragFloat("Width", &width, 1.0f, 0.0f, 20.0f))
			{
				component->setWidth(i, width, modifyAllConnectedRoads);
			}
			float arc = component->getArc(i);
			if (ImGui::DragFloat("Arc", &arc, 0.1f, 0.0f, 20.0f))
			{
				component->setArc(i, arc, modifyAllConnectedRoads);
			}

			ImGui::PopID();
		}

		ImGui::Separator();

		float quality = component->getQuality();
		if (ImGui::DragFloat("Quality", &quality, 2.0f, 3.0f, 21.0f))
		{
			component->setQuality(quality);
		}

		ImGui::Separator();


		if (ImGui::Button("Generate polygon"))
		{
			component->createPolygon();
		}
	}
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
				showRenderComponentDetails(renderComponent);
			}

			Terrain* terrain = dynamic_cast<Terrain*>(vbEditor::_selectedSceneObject->getComponent(CT_TERRAIN));
			if (terrain)
			{
				showRenderComponentDetails(terrain);
			}

			PhysicalBody* physicsComponent = dynamic_cast<PhysicalBody*>(vbEditor::_selectedSceneObject->getComponent(CT_PHYSICAL_BODY));
			if (physicsComponent)
			{
				if (ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
				{

				}
			}

			Grass* grassComponent = dynamic_cast<Grass*>(vbEditor::_selectedSceneObject->getComponent(CT_GRASS));
			if (grassComponent)
			{
				showRenderComponentDetails(grassComponent);
			}

			ShapePolygonComponent* shapePolygonComponent = dynamic_cast<ShapePolygonComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_SHAPE_POLYGON));
			if (shapePolygonComponent)
			{
				showShapePolygonComponentDetails(shapePolygonComponent);
			}

			RoadIntersectionComponent* roadIntersectionComponent = dynamic_cast<RoadIntersectionComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_ROAD_INTERSECTION));
			if (roadIntersectionComponent)
			{
				showRoadIntersectionComponentDetails(roadIntersectionComponent);
			}

			//RoadObject* roadComponent = dynamic_cast<RoadObject*>(vbEditor::_selectedSceneObject->getComponent(CT_ROAD_OBJECT));
			//if (roadComponent)
			//{
				//if (ImGui::CollapsingHeader("Road", ImGuiTreeNodeFlags_DefaultOpen))
				//{
				//	// left
				//	static float actualWinSize = Renderer::getInstance().getWindowDimensions().x;
				//	static int selected = 0;
				//	ImGui::BeginChild("left pane", ImVec2((ImGui::GetWindowSize().x * (0.4f)), 90), true);

				//	ImVec2 range = ImVec2(0.0f, 0.0f);

				//	RRoadProfile* profile = roadComponent->getRoadProfile();
				//	std::vector<RoadLane> lanes = profile->getRoadLanes();

				//	for (int i = 0; i < lanes.size(); i++)
				//	{
				//		char label[128];
				//		sprintf(label, "%d: %s##%d", i, lanes[i].material.name.c_str(), i);
				//		//sprintf(label, "MyObject %d", i);
				//		if (ImGui::Selectable(label, selected == i))
				//			selected = i;
				//	}
				//	ImGui::EndChild();
				//	ImGui::SameLine();

				//	//std::string pos = std::string("X: ");
				//	ImGui::Text("X: %f, Y: %f", lanes[selected].r1, lanes[selected].r2);
				//	//ImGui::LabelText(pos.c_str());

				//	//static float values[4] = { 0.0, 1.0, 0.0, -1.0 };
				//	
				//	static std::vector<ImVec2> values = { ImVec2(-5.0f, -5.0f), ImVec2(0.0f, 0.0f) }; //, ImVec2(0.0f, 0.0f), ImVec2(0.5f, 0.5f)};
				//
				//	static int values_offset = 0;
				//	static double refresh_time = 0.0;
				//	/*
				//		static float phase = 0.0f;
				//		values[values_offset] = cosf(phase);
				//		values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
				//		phase += 0.10f*values_offset;
				//		refresh_time += 1.0f / 60.0f;
				//	*/
				//	
				//	//ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(0, 100));

				//	ImGui::RoadProfileGraph("Profile", values, -5.0f, 5.0f, ImVec2(150, 150));

				//	std::vector<RoadSegment> segments = roadComponent->getSegments();
				//	RoadSegment segment = segments[0];
				//	//segment.begin = glm::vec3(0.0f, 0.0, 0.0f);

				//	glm::vec3 pos = segment.begin;
				//	ImGui::Text("Road Pos: %f, %f, %f", pos.x, pos.y, pos.z);

				//	//ImColor color(ImGui::GetStyle().Colors[ImGuiCol_PlotLines]);

				//	//glm::vec4 transPos = vbEditor::_camera->getProjectionMatrix() * vbEditor::_camera->getViewMatrix() * vbEditor::_selectedSceneObject->getGlobalTransformMatrix() * glm::vec4(pos, 1.0);

				//	//window->DrawList->AddCircleFilled(ImVec2(transPos.x, transPos.y), 14.0f, color);

				//	if (ImGui::Button("Update road"))
				//	{
				//		RoadSegment seg;
				//		seg.begin = glm::vec3(50, 20, 100);
				//		seg.end = segment.end;
				//		seg.interpolation = segment.interpolation;
				//		seg.pointsCount = segment.pointsCount;
				//		seg.r = segment.r;
				//		seg.type = segment.type;
				//		segments[0] = seg;

				//		roadComponent->setSegments(segments);

				//		roadComponent->buildModel();
				//	}
				//}
			RoadObject* roadComponent = dynamic_cast<RoadObject*>(vbEditor::_selectedSceneObject->getComponent(CT_ROAD_OBJECT));
			if (roadComponent && roadComponent->getRoadType() == RoadType::BEZIER_CURVES)
			{
				showRoadComponentDetails(roadComponent);
			}
			if (roadComponent && roadComponent->getRoadType() == RoadType::LINES_AND_ARC)
			{
				if (ImGui::CollapsingHeader("Road component", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (vbEditor::roadActivePoint >= 0 && vbEditor::roadActivePoint < roadComponent->getPoints().size())
					{
						ImGui::Text("Point: %d", vbEditor::roadActivePoint);

						float* pointPosition = glm::value_ptr(roadComponent->getPoints()[vbEditor::roadActivePoint]);

						if (ImGui::DragFloat3("Point position", pointPosition, 0.01f, 0.0f, 0.0f))
						{
							vbEditor::isRoadModified = true;
						}

						if (ImGui::Button("Delete point"))
						{
							vbEditor::isRoadModified = true;
							roadComponent->deletePoint(vbEditor::roadActivePoint);

							if (vbEditor::roadActivePoint == roadComponent->getPoints().size())
							{
								--vbEditor::roadActivePoint;
								--vbEditor::roadActiveSegment;
							}

							if (roadComponent->getPoints().size() == 0)
							{
								vbEditor::_sceneManager->removeSceneObject(roadComponent->getSceneObject());
								vbEditor::_selectedSceneObject = nullptr;
							}
						}
					}

					if (vbEditor::roadActiveSegment >= 0 && roadComponent->getSegments().size() > 0 && vbEditor::roadActiveSegment < roadComponent->getSegments().size())
					{
						ImGui::Separator();

						ImGui::Text("Segment: %d", vbEditor::roadActiveSegment);

						int typeComboCurrentItem = roadComponent->getSegments()[vbEditor::roadActiveSegment].type == RST_LINE ? 0 : 1;
						const char* typeComboItems[] = { "line", "arc" };
						if (ImGui::Combo("Type", &typeComboCurrentItem, typeComboItems, IM_ARRAYSIZE(typeComboItems)))
						{
							vbEditor::isRoadModified = true;
							roadComponent->getSegments()[vbEditor::roadActiveSegment].type = typeComboCurrentItem == 0 ? RST_LINE : RST_ARC;
						}

						int interpolationComboCurrentItem = roadComponent->getSegments()[vbEditor::roadActiveSegment].interpolation == RI_LIN ? 0 : 1;
						const char* interpolationComboItems[] = { "lin", "cos" };
						if (ImGui::Combo("Interpolation", &interpolationComboCurrentItem, interpolationComboItems, IM_ARRAYSIZE(interpolationComboItems)))
						{
							vbEditor::isRoadModified = true;
							roadComponent->getSegments()[vbEditor::roadActiveSegment].interpolation = interpolationComboCurrentItem == 0 ? RI_LIN : RI_COS;
						}

						int* points = &(roadComponent->getSegments()[vbEditor::roadActiveSegment].pointsCount);
						if (ImGui::DragInt("Points count", points, 5.0f, 0.0f, 0.0f))
						{
							vbEditor::isRoadModified = true;
						}

						float* radius = &(roadComponent->getSegments()[vbEditor::roadActiveSegment].r);
						if (ImGui::DragFloat("Radius", radius, 0.01f, 0.0f, 0.0f))
						{
							vbEditor::isRoadModified = true;
						}
					}

					ImGui::Separator();

					int roadProfilesCurrentItem = 0;
					std::vector<const char*> roadProfilesComboItems;
					for (int i = 0; i < vbEditor::_availableRoadProfiles.size(); ++i)
					{
						roadProfilesComboItems.push_back(vbEditor::_availableRoadProfiles[i].c_str());

						if (vbEditor::_availableRoadProfiles[i] == roadComponent->getRoadProfile()->getName())
							roadProfilesCurrentItem = i;
					}
					if (ImGui::Combo("Road profile", &roadProfilesCurrentItem, roadProfilesComboItems.data(), roadProfilesComboItems.size()))
					{
						roadComponent->setRoadProfile(ResourceManager::getInstance().loadRoadProfile(vbEditor::_availableRoadProfiles[roadProfilesCurrentItem]));

						roadComponent->buildModel(false);
					}

					ImGui::Separator();

					//ImGui::Text("Connections");


					std::vector<CrossroadComponent*> availableCrossroads = vbEditor::_sceneManager->getGraphicsManager()->getCrossroadComponents();
					CrossroadComponent* connectedCrossroads[2] = { roadComponent->getConnectionPoint(0).crossroadComponent, roadComponent->getConnectionPoint(1).crossroadComponent };

					int crossroadCurrentItems[2] = { 0, 0 };
					int connectionPointCurrentItems[2] = { roadComponent->getConnectionPoint(0).index, roadComponent->getConnectionPoint(1).index };

					std::string crossroadsComboItems{};
					crossroadsComboItems += " ";
					crossroadsComboItems += '\0';

					for (int i = 0; i < availableCrossroads.size(); ++i)
					{
						crossroadsComboItems += availableCrossroads[i]->getSceneObject()->getName() + '\0';

						for (int j = 0; j < 2; ++j)
						{
							if (availableCrossroads[i] == connectedCrossroads[j])
							{
								crossroadCurrentItems[j] = i + 1;
							}
						}
					}

					for (int i = 0; i < 2; ++i)
					{
						ImGui::Text("Connection: %d", i);

						ImGui::PushID(i);
						if (ImGui::Combo("Crossroad", &crossroadCurrentItems[i], crossroadsComboItems.c_str()))
						{
							if (crossroadCurrentItems[i] == 0)
							{
								roadComponent->setConnectionPoint(i, nullptr);
							}
							else
							{
								roadComponent->setConnectionPoint(i, availableCrossroads[crossroadCurrentItems[i] - 1], 0);
							}

							vbEditor::isRoadModified = true;
						}

						if (connectedCrossroads[i] != nullptr)
						{
							std::string connectionPointComboItems{};
							for (int j = 0; j < connectedCrossroads[i]->getConnectionsCount(); ++j)
							{
								connectionPointComboItems += toString(j) + '\0';
							}

							if (ImGui::Combo("Index", &connectionPointCurrentItems[i], connectionPointComboItems.c_str()))
							{
								roadComponent->setConnectionPoint(i, availableCrossroads[crossroadCurrentItems[i] - 1], connectionPointCurrentItems[i]);

								vbEditor::isRoadModified = true;
							}
						}
						ImGui::PopID();
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