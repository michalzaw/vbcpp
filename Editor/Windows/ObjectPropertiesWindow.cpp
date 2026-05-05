#include "ObjectPropertiesWindow.h"

#include <numeric>
#include <stdarg.h>

#include <ImGuizmo.h>
#include "glm/gtc/type_ptr.hpp"

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "OpenDialogWindow.h"
#include "../Tools/RoadManipulator.h"
#include "../FileDialogs.h"
#include "../Utils/AIPathGenerator.h"

#include "../../Game/AI/AIAgent.h"
#include "../../Game/AI/AIAgentVehicle.h"
#include "../../Game/AI/PathComponent.h"
#include "../../Game/AI/StopComponent.h"
#include "../../Game/AI/TrafficLightsComponent.h"
#include "../../Game/BusStartPoint.h"
#include "../../Game/GameLogicSystem.h"

#include "../../Graphics/BezierCurve.h"
#include "../../Graphics/FoliagePatch.h"
#include "../../Graphics/ShapePolygonComponent.h"
#include "../../Graphics/SkeletalAnimationComponent.h"
#include "../../Graphics/SkeletalAnimationComponent2.h"
#include "../../Graphics/SkeletalAnimationHelperComponent.h"

#include "../../Physics/PhysicalBodyGhost.h"

#include "../../Scripting/ScriptComponent.h"

#include "../../Utils/FilesHelper.h"


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

	extern bool _showMaterialEditorWindow;
	extern RenderObject* currentRenderObject;
	extern RStaticModel* currentStaticModel;
	extern unsigned int currentModelLod;
	extern unsigned int currentMaterialIndex;

	extern bool _showGenerateObjectsAlongCurveWindow;

	extern OpenDialogWindow* _addSceneObjectDialogWindow;
	extern OpenDialogWindow* _selectRoadProfileDialogWindow;

	extern SceneObject* _groupingSceneObject;
}


void shwoRoadProfileEdit(const std::string& roadProfileName, const std::function<void(const std::string&)>& onRoadProfileSelectedCallback)
{
	char buffer[1024] = { '\0' };
	strncpy(buffer, roadProfileName.c_str(), sizeof buffer);
	buffer[sizeof buffer - 1] = '\0';

	ImGui::Text("Road profile");

	ImGui::InputText("##RoadProfileEditText", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();

	if (ImGui::Button("..."))
	{
		*(vbEditor::_selectRoadProfileDialogWindow->getOpenFlagPointer()) = true;
		vbEditor::_selectRoadProfileDialogWindow->setOnOkClickCallback(onRoadProfileSelectedCallback);
	}
}


void shwoRoadProfileEdit2(const std::string& roadProfileName, const std::function<void(const std::string&)>& onRoadProfileSelectedCallback)
{
	ImGui::SetNextItemWidth(-30);

	char buffer[1024] = { '\0' };
	strncpy(buffer, roadProfileName.c_str(), sizeof buffer);
	buffer[sizeof buffer - 1] = '\0';

	ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();

	if (ImGui::Button("...", ImVec2(20, 0)))
	{
		*(vbEditor::_selectRoadProfileDialogWindow->getOpenFlagPointer()) = true;
		vbEditor::_selectRoadProfileDialogWindow->setOnOkClickCallback(onRoadProfileSelectedCallback);
	}
}


void showObjectSelectEdit(const std::string& objectName, const std::function<void(const std::string&)>& onObjectSelectedCallback)
{
	ImGui::SetNextItemWidth(-30);

	char buffer[1024] = { '\0' };
	strncpy(buffer, objectName.c_str(), sizeof buffer);
	buffer[sizeof buffer - 1] = '\0';

	ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();

	if (ImGui::Button("...", ImVec2(20, 0)))
	{
		*(vbEditor::_addSceneObjectDialogWindow->getOpenFlagPointer()) = true;
		vbEditor::_addSceneObjectDialogWindow->setOnOkClickCallback(onObjectSelectedCallback);
	}
}


void showRoadComponentDetails(RoadObject* roadComponent)
{
	if (ImGui::CollapsingHeader("Road component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		shwoRoadProfileEdit(roadComponent->getRoadProfile()->getName(), [roadComponent](const std::string& newRoadProfile)
			{
				roadComponent->setRoadProfile(ResourceManager::getInstance().loadRoadProfile(newRoadProfile));
			});

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
				}
			}
			ImGui::PopID();
		}

		ImGui::Separator();

		if (ImGui::Button("Generate AI paths", ImVec2(-1.0f, 0.0f)))
		{
			AIPathGenerator::generateAIPaths(roadComponent, vbEditor::_sceneManager);
		}
	}
}


template<typename TYPE>
void convertVectorToComboData(const std::vector<TYPE>& items, const TYPE& selectedItemValue, std::string& outItemsString, int& outSelectedItemIndex, bool addEmptyElement = true)
{
	outSelectedItemIndex = 0;

	if (addEmptyElement)
	{
		outItemsString += " ";
		outItemsString += '\0';

		outSelectedItemIndex = -1;
	}

	for (int i = 0; i < items.size(); ++i)
	{
		outItemsString += Strings::toString(items[i]) + '\0';

		if (items[i] == selectedItemValue)
		{
			outSelectedItemIndex = i;
		}
	}

	if (addEmptyElement)
	{
		outSelectedItemIndex += 1;
	}
}


template<typename TYPE>
void convertVectorToComboData(const TYPE* items, unsigned int itemsCount, const TYPE& selectedItemValue, std::string& outItemsString, int& outSelectedItemIndex, bool addEmptyElement = true)
{
	if (addEmptyElement)
	{
		outItemsString += " ";
		outItemsString += '\0';
	}

	for (int i = 0; i < itemsCount; ++i)
	{
		outItemsString += Strings::toString(items[i]) + '\0';

		if (items[i] == selectedItemValue)
		{
			outSelectedItemIndex = i;
		}
	}

	if (addEmptyElement)
	{
		outSelectedItemIndex += 1;
	}
}


typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef std::string str;
typedef std::string path;
typedef std::string str_combo;


#define IMGUI_INPUT_int(component, propertyName, additionalParams)																				\
int value = component->get##propertyName();																										\
bool result = ImGui::InputInt("##value", &value, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);


#define IMGUI_INPUT_float(component, propertyName, additionalParams)																			\
float value = component->get##propertyName();																									\
bool result = ImGui::DragFloat("##value", &value, 0.01, 0, 0);
//bool result = ImGui::InputFloat("##value", &value, 0, 0, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue);


#define IMGUI_INPUT_vec2(component, propertyName, additionalParams)																				\
glm::vec2 value = component->get##propertyName();																								\
bool result = ImGui::DragFloat2("##value", glm::value_ptr(value));


#define IMGUI_INPUT_vec3(component, propertyName, additionalParams)																				\
glm::vec3 value = component->get##propertyName();																								\
bool result = ImGui::DragFloat3("##value", glm::value_ptr(value));


#define IMGUI_INPUT_btVector3(component, propertyName, additionalParams)																		\
btVector3 value = component->get##propertyName();																								\
float valueArray[] = { value.getX(), value.getY(), value.getZ() };																				\
bool result = ImGui::DragFloat3("##value", valueArray);																				\
value.setValue(valueArray[0], valueArray[1], valueArray[2]);


#define IMGUI_INPUT_bool(component, propertyName, additionalParams)																				\
bool value = component->is##propertyName();																										\
bool result = ImGui::Checkbox("##value", &value);


#define IMGUI_INPUT_str(component, propertyName, additionalParams)																				\
char value[50];																																	\
strncpy(value, component->get##propertyName().c_str(), sizeof value);																			\
value[sizeof value - 1] = '\0';																													\
																																				\
bool result = ImGui::InputText("##value", value, IM_ARRAYSIZE(value), ImGuiInputTextFlags_EnterReturnsTrue);


#define IMGUI_INPUT_path(component, propertyName, additionalParams)																				\
ImGui::SetNextItemWidth(-30);																													\
																																				\
char buffer[1024];																																\
strncpy(buffer, component->get##propertyName().c_str(), sizeof buffer);																			\
buffer[sizeof buffer - 1] = '\0';																												\
																																				\
ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_ReadOnly);																\
																																				\
ImGui::SameLine();																																\
																																				\
std::string value;																																\
bool result = false;																															\
																																				\
if (ImGui::Button("...", ImVec2(20, 0)))																										\
{																																				\
	std::vector<std::string> dialogResult = FileDialogs::openFile("Choose file...");															\
																																				\
	if (dialogResult.size() == 1)																												\
	{																																			\
		LOG_INFO(dialogResult[0]);																												\
		value = dialogResult[0];;																												\
		result = true;																															\
	}																																			\
}


#define IMGUI_INPUT_str_combo(component, propertyName, additionalParams)																		\
const std::vector<std::string>& itemsNames = additionalParams;																					\
std::string comboItems;																															\
int selectedItemIndex = 0;																														\
convertVectorToComboData(itemsNames, component->get##propertyName(), comboItems, selectedItemIndex);											\
																																				\
bool result = false;																															\
std::string value = "";																															\
if (ImGui::Combo("##value", &selectedItemIndex, comboItems.c_str()))																			\
{																																				\
	result = true;																																\
	if (selectedItemIndex > 0)																													\
		value = itemsNames[selectedItemIndex - 1];																								\
}


#define COMPONENT_PROPERTY_EDIT_BEGIN(propertyName, displayName)																				\
{																																				\
	ImGui::PushID(#propertyName);																												\
																																				\
	ImGui::AlignTextToFramePadding();																											\
	ImGui::TreeNodeEx(displayName, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, displayName);		\
	ImGui::NextColumn();																														\
	ImGui::SetNextItemWidth(-1);																												\
}

#define COMPONENT_PROPERTY_EDIT_END																												\
{																																				\
	ImGui::NextColumn();																														\
																																				\
	ImGui::PopID();																																\
}


#define COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(component, propertyName, type, displayName, callback, additionalParams)							\
{																																				\
	COMPONENT_PROPERTY_EDIT_BEGIN(propertyName, displayName)																					\
																																				\
	IMGUI_INPUT_##type(component, propertyName, additionalParams)																				\
																																				\
	if (result)																																	\
	{																																			\
		component->set##propertyName(type(value));																								\
		callback(type(value));																													\
	}																																			\
																																				\
	COMPONENT_PROPERTY_EDIT_END																													\
}


#define COMPONENT_RESOURCE_EDIT(component, propertyName, displayName, pathToResourceMapper)														\
{																																				\
	COMPONENT_PROPERTY_EDIT_BEGIN(propertyName, displayName)																					\
																																				\
	IMGUI_INPUT_path(component->get##propertyName(), Path, additionalParams)																	\
																																				\
	if (result)																																	\
	{																																			\
		component->set##propertyName(pathToResourceMapper(value));																				\
	}																																			\
																																				\
	COMPONENT_PROPERTY_EDIT_END																													\
}


#define COMPONENT_PROPERTY_EDIT(component, propertyName, type, displayName) COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(component, propertyName, type, displayName, [](type){}, "")
#define COMPONENT_PROPERTY_EDIT_2(component, propertyName, type, displayName, additionalParams) COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(component, propertyName, type, displayName, [](type){}, additionalParams)


bool newNode(const char* name, const char* descriptionFmt, ...)
{
	ImGui::PushID(name);
	ImGui::AlignTextToFramePadding();
	bool nodeOpen = ImGui::TreeNode("treeNode", name);

	ImGui::NextColumn();

	ImGui::AlignTextToFramePadding();

	va_list args;
	va_start(args, descriptionFmt);
	ImGui::TextV(descriptionFmt, args);
	va_end(args);

	ImGui::NextColumn();

	return nodeOpen;
}


bool newNode(const char* name, ImGuiTreeNodeFlags flags, const char* descriptionFmt, ...)
{
	ImGui::PushID(name);
	ImGui::AlignTextToFramePadding();
	bool nodeOpen = ImGui::TreeNodeEx("treeNode", flags, name);

	ImGui::NextColumn();

	ImGui::AlignTextToFramePadding();

	va_list args;
	va_start(args, descriptionFmt);
	ImGui::TextV(descriptionFmt, args);
	va_end(args);

	ImGui::NextColumn();

	return nodeOpen;
}


void showRenderComponentDetails(RenderObject* renderComponent)
{
	if (ImGui::CollapsingHeader("Render Component (Old)"/*, ImGuiTreeNodeFlags_DefaultOpen*/))
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
			renderComponent->setCastShadows(isCastShadows);
		}

		bool isDynamicObject = renderComponent->isDynamicObject();
		if (ImGui::Checkbox("Dynamic object", &isDynamicObject))
		{
			renderComponent->setDynamicObject(isDynamicObject);
		}
	}


	if (ImGui::CollapsingHeader("Render Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("RenderComponentDetails");


		ImGui::PushID("Models");
		for (int i = 0; i < renderComponent->getNumberOfLod(); ++i)
		{
			ImGui::PushID(i);
			if (newNode("LOD", "%d", i))
			{
				COMPONENT_RESOURCE_EDIT(renderComponent, Model, "Model", [renderComponent](const std::string& path) { return ResourceManager::getInstance().loadModel(path, renderComponent->getSceneObject()->getObjectDefinition()->getPath()); })

				if (newNode("Materials", ""))
				{
					static RTexture* whiteTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();

					for (int j = 0; j < renderComponent->getModel(i)->getMaterialsCount(); ++j)
					{
						Material* material = renderComponent->getModel(i)->getMaterial(j);

						ImGui::PushID(material->name.c_str());
						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("#treeNode", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "");
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);

						RTexture* texture = material->diffuseTexture != nullptr
							? material->diffuseTexture
							: whiteTexture;

						if (ImGui::ImageButton((ImTextureID)texture->getID(), ImVec2(48, 48)))
						{
							LOG_DEBUG(material->name);
							vbEditor::_showMaterialEditorWindow = true;
							vbEditor::currentRenderObject = renderComponent;
							vbEditor::currentStaticModel = renderComponent->getModel(i);
							vbEditor::currentModelLod = i;
							vbEditor::currentMaterialIndex = j;
						}

						ImGui::SameLine();
						ImGui::Text(material->name.c_str());

						ImGui::NextColumn();

						ImGui::PopID();
					}

					ImGui::TreePop();
				}
				ImGui::PopID();

				ImGui::TreePop();
			}
			ImGui::PopID();

			ImGui::PopID(); // i
		}
		ImGui::PopID(); // Models

		COMPONENT_PROPERTY_EDIT(renderComponent, CastShadows, bool, "Cast shadows")
		COMPONENT_PROPERTY_EDIT(renderComponent, DynamicObject, bool, "Dynamic object")

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showRoadIntersectionComponentDetails(RoadIntersectionComponent* component)
{
	if (ImGui::CollapsingHeader("Road intersection", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("RoadIntersectionComponentDetails");

		if (newNode("Number of connected roads", ImGuiTreeNodeFlags_DefaultOpen, "%d", component->getConnectedRoads().size()))
		{
			for (const auto& connectedRoad : component->getConnectedRoads())
			{
				ImGui::BulletText("%s (%d)", connectedRoad.road->getSceneObject()->getName().c_str(), connectedRoad.connectionPointInRoadIndex);
			}

			ImGui::TreePop(); // newNode
		}
		ImGui::PopID(); // newNode

		ImGui::Separator();

		ImGui::PushID("ConnectedRoads");

		if (newNode("Connected roads", ImGuiTreeNodeFlags_DefaultOpen, ""))
		{
			static bool modifyAllConnectedRoads = true;
			COMPONENT_PROPERTY_EDIT_BEGIN(modifyAllConnectedRoads, "Modify all connected roads")
			{
				ImGui::Checkbox("", &modifyAllConnectedRoads);
			}
			COMPONENT_PROPERTY_EDIT_END

			ImGui::Separator();

			int numberOfRoads = modifyAllConnectedRoads && component->getConnectedRoads().size() > 0 ? 1 : component->getConnectedRoads().size();
			for (int i = 0; i < numberOfRoads; ++i)
			{
				ImGui::PushID(i);

				if (newNode("Road", ImGuiTreeNodeFlags_DefaultOpen, "%d", i + 1))
				{
					COMPONENT_PROPERTY_EDIT_BEGIN(Length, "Length")
					{
						float length = component->getLength(i);
						if (ImGui::DragFloat("", &length, 1.0f, 0.0f, 100.0f))
						{
							component->setLength(i, length, modifyAllConnectedRoads);
						}
					}
					COMPONENT_PROPERTY_EDIT_END

					COMPONENT_PROPERTY_EDIT_BEGIN(Width, "Width")
					{
						float width = component->getWidth(i);
						if (ImGui::DragFloat("", &width, 1.0f, 0.0f, 20.0f))
						{
							component->setWidth(i, width, modifyAllConnectedRoads);
						}
					}
					COMPONENT_PROPERTY_EDIT_END

					COMPONENT_PROPERTY_EDIT_BEGIN(Arc, "Arc")
					{
						float arc = component->getArc(i);
						if (ImGui::DragFloat("", &arc, 0.1f, 0.0f, 20.0f))
						{
							component->setArc(i, arc, modifyAllConnectedRoads);
						}
					}
					COMPONENT_PROPERTY_EDIT_END

					ImGui::TreePop(); // newNode Road
				}
				ImGui::PopID(); // newNode Road

				ImGui::PopID(); // i
			}

			ImGui::Separator();

			COMPONENT_PROPERTY_EDIT_BEGIN(Quality, "Quality")
			{
				float quality = component->getQuality();
				if (ImGui::DragFloat("", &quality, 2.0f, 3.0f, 21.0f))
				{
					component->setQuality(quality);
				}
			}
			COMPONENT_PROPERTY_EDIT_END

			ImGui::TreePop(); // newNode Connected roads
		}
		ImGui::PopID(); // newNode Connected roads

		ImGui::PopID(); // ConnectedRoads

		ImGui::Separator();

		COMPONENT_PROPERTY_EDIT_BEGIN(RoadProfile, "Road profile")
		{
			shwoRoadProfileEdit2(component->getEdgeRoadProfile()->getName(), [component](const std::string& newRoadProfile)
				{
					component->setEdgeRoadProfile(ResourceManager::getInstance().loadRoadProfile(newRoadProfile));
				});
		}
		COMPONENT_PROPERTY_EDIT_END

		COMPONENT_PROPERTY_EDIT(component, EdgeRoadProfileNumberOfLanesToRemove, int, "Lanes to remove")

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

		if (ImGui::Button("Refresh intersection geometry", ImVec2(-1.0f, 0.0f)))
		{
			component->createPolygon();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("RoadIntersectionComponentDetails");

		if (newNode("AI path generator", ImGuiTreeNodeFlags_DefaultOpen, ""))
		{
			static bool connectGeneratedPaths = true;
			static float maxDistanceToCreateConnection = 5.0f;
			static int pathQuality = 20;
			static float innerPathCurveFactor = 2.0f;
			static float outerPathCurveFactor = 1.0f;
			static float speedOnStraightPaths = 40.0f;
			static float speedOnCurvedPaths = 20.0f;

			COMPONENT_PROPERTY_EDIT_BEGIN(connectGeneratedPaths, "Connect with other paths on scene")
			{
				ImGui::Checkbox("", &connectGeneratedPaths);
			}
			COMPONENT_PROPERTY_EDIT_END

			COMPONENT_PROPERTY_EDIT_BEGIN(maxDistanceToCreateConnection, "Max distance to create connection")
			{
				ImGui::DragFloat("", &maxDistanceToCreateConnection, 1.0f, 1, 30);
			}
			COMPONENT_PROPERTY_EDIT_END

			COMPONENT_PROPERTY_EDIT_BEGIN(pathQuality, "Path quality")
			{
				ImGui::DragInt("", &pathQuality, 1.0f, 4, 100);
			}
			COMPONENT_PROPERTY_EDIT_END

			COMPONENT_PROPERTY_EDIT_BEGIN(innerPathCurveFactor, "Inner path curve factor")
			{
				ImGui::DragFloat("", &innerPathCurveFactor, 1.0f, 1, 10);
			}
			COMPONENT_PROPERTY_EDIT_END

			COMPONENT_PROPERTY_EDIT_BEGIN(outerPathCurveFactor, "Outer path curve factor")
			{
				ImGui::DragFloat("", &outerPathCurveFactor, 1.0f, 1, 10);
			}
			COMPONENT_PROPERTY_EDIT_END

			COMPONENT_PROPERTY_EDIT_BEGIN(speedOnStraightPaths, "Speed on straight paths")
			{
				ImGui::DragFloat("", &speedOnStraightPaths, 1.0f, 1, 10);
			}
			COMPONENT_PROPERTY_EDIT_END

			COMPONENT_PROPERTY_EDIT_BEGIN(speedOnCurvedPaths, "Sped on curved paths")
			{
				ImGui::DragFloat("", &speedOnCurvedPaths, 1.0f, 1, 10);
			}
			COMPONENT_PROPERTY_EDIT_END

			ImGui::Columns(1);

			if (ImGui::Button("Generate AI paths", ImVec2(-1.0f, 0.0f)))
			{
				AIPathGenerator::generateAIPaths(component, vbEditor::_sceneManager, pathQuality, innerPathCurveFactor, outerPathCurveFactor, connectGeneratedPaths, maxDistanceToCreateConnection,
												 speedOnStraightPaths, speedOnCurvedPaths);
			}

			ImGui::Columns(2);

			ImGui::TreePop(); // newNode
		}
		ImGui::PopID(); // newNode

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}

	if (ImGui::CollapsingHeader("Road intersection (Old)"))
	{
		ImGui::Text("Number of connected roads: %d", component->getConnectedRoads().size());
		for (const auto& connectedRoad : component->getConnectedRoads())
		{
			ImGui::BulletText("%s (%d)", connectedRoad.road->getSceneObject()->getName().c_str(), connectedRoad.connectionPointInRoadIndex);
		}

		ImGui::Separator();

		static bool modifyAllConnectedRoads = true;
		ImGui::Checkbox("Modify all connected roads", &modifyAllConnectedRoads);

		int numberOfRoads = modifyAllConnectedRoads && component->getConnectedRoads().size() > 0 ? 1 : component->getConnectedRoads().size();
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

		shwoRoadProfileEdit(component->getEdgeRoadProfile()->getName(), [component](const std::string& newRoadProfile)
			{
				component->setEdgeRoadProfile(ResourceManager::getInstance().loadRoadProfile(newRoadProfile));
			});

		int numberOfLanesToRemove = component->getEdgeRoadProfileNumberOfLanesToRemove();
		if (ImGui::DragInt("Lanes to remove", &numberOfLanesToRemove, 1.0f, 1, 20))
		{
			component->setEdgeRoadProfileNumberOfLanesToRemove(numberOfLanesToRemove);
		}

		ImGui::Separator();


		if (ImGui::Button("Generate polygon"))
		{
			component->createPolygon();
		}

		ImGui::Separator();

		static bool connectGeneratedPaths = true;
		ImGui::Checkbox("Connect with other paths on scene", &connectGeneratedPaths);

		if (ImGui::Button("Generate AI paths", ImVec2(-1.0f, 0.0f)))
		{
			AIPathGenerator::generateAIPaths(component, vbEditor::_sceneManager, connectGeneratedPaths);
		}
	}
}


void showBusStopComponentDetails(BusStopComponent* component)
{
	if (ImGui::CollapsingHeader("Bus Stop Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//ImGui::Text("Number of connected roads: %d", component->getConnectedRoads().size());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("BusStopComponentDetails");

		COMPONENT_PROPERTY_EDIT(component, Id, int, "Id")
		COMPONENT_PROPERTY_EDIT(component, Name, str, "Name")
		COMPONENT_PROPERTY_EDIT(component, AnnouncementFileName, path, "Announcement")
		COMPONENT_PROPERTY_EDIT(component, Distance, float, "Distance")
		COMPONENT_PROPERTY_EDIT(component, RequestStop, bool, "Request stop")

		if (newNode("Number of passengers", "[%d, %d]", component->getNumberOfPassengersMin(), component->getNumberOfPassengersMax()))
		{
			COMPONENT_PROPERTY_EDIT(component, NumberOfPassengersMin, int, "Min")
			COMPONENT_PROPERTY_EDIT(component, NumberOfPassengersMax, int, "Max")

			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showSkeletalAnimationComponentDetails(SkeletalAnimationComponent* component)
{
	if (ImGui::CollapsingHeader("Skeletal Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("SkeletalAnimationComponentDetails");

		if (newNode("Animation states", ""))
		{
			const std::string& currentStateName = component->getCurrentAnimationState() != nullptr ? component->getCurrentAnimationState()->name : "";
			const std::string& nextStateName = component->getNextAnimationState() != nullptr ? component->getNextAnimationState()->name : "";

			int i = 0;
			for (const auto& animationState : component->getAnimationStates())
			{
				ImGui::PushID(i++);

				if (newNode("State", "%s %s", animationState.first.c_str(), currentStateName == animationState.first ? "[current]" : (nextStateName == animationState.first ? "[next]" : "")))
				{
					AnimationState* state = const_cast<AnimationState*>(&animationState.second);

					COMPONENT_RESOURCE_EDIT(state, Animation, "Animation file", [](const std::string& path) { return ResourceManager::getInstance().loadAnimation(path); })
					COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(state, CurrentTime, float, "Current time", [component](float newValue) { component->recalculateAllBonesTransform(); }, "")

					RAnimation* animation = state->getAnimation();
					COMPONENT_PROPERTY_EDIT(animation, StartFrame, int, "Start frame")
					COMPONENT_PROPERTY_EDIT(animation, EndFrame, int, "End frame")
					COMPONENT_PROPERTY_EDIT(animation, TicksPerSecond, int, "Ticks per second")

					ImGui::TreePop();
				}
				ImGui::PopID();

				ImGui::PopID(); // i
			}

			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGui::Separator();

		COMPONENT_PROPERTY_EDIT(component, AnimationSpeed, float, "Animation speed")
		COMPONENT_PROPERTY_EDIT(component, Play, bool, "Play")
		COMPONENT_PROPERTY_EDIT_2(component, RootBone, str_combo, "Root", component->getCurrentAnimationState()->getAnimation()->getBonesNames())
		COMPONENT_PROPERTY_EDIT(component, LockRootBoneTranslation, bool, "Lock translation")
		ImGui::Separator();
		COMPONENT_PROPERTY_EDIT(component, EndToStartFrameBlending, bool, "End to start frame blending")
		COMPONENT_PROPERTY_EDIT(component, EndToStartFrameBlendingTime, float, "End to start frame blending time")
		ImGui::Separator();
		COMPONENT_PROPERTY_EDIT(component, StateBlendingDuration, float, "State blending duration")
		ImGui::Separator();
		COMPONENT_PROPERTY_EDIT(component, Scale, float, "Scale")
		ImGui::Separator();

		COMPONENT_PROPERTY_EDIT_BEGIN(NextState, "Set next state")
		{
			int statesCurrentItem = 0;

			std::string statesComboItems{};
			statesComboItems += " ";
			statesComboItems += '\0';

			std::vector<std::string> statesComboItemsVec;

			const std::string& nextAnimationStateName = component->getNextAnimationState() != nullptr ? component->getNextAnimationState()->name : "";

			int i = 0;
			for (const auto& animationState : component->getAnimationStates())
			{
				++i;

				statesComboItems += animationState.first + '\0';
				statesComboItemsVec.push_back(animationState.first);

				if (animationState.first == nextAnimationStateName)
				{
					statesCurrentItem = i;
				}
			}

			if (ImGui::Combo("", &statesCurrentItem, statesComboItems.c_str()))
			{
				if (statesCurrentItem > 0)
				{
					component->setNextAnimationState(statesComboItemsVec[statesCurrentItem - 1]);
				}
			}
		}
		COMPONENT_PROPERTY_EDIT_END

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showSkeletalAnimationComponent2Details(SkeletalAnimationComponent2* component)
{
	if (ImGui::CollapsingHeader("Skeletal Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("SkeletalAnimation2ComponentDetails");

		COMPONENT_PROPERTY_EDIT(component, CurrentTime, float, "Current time")
		COMPONENT_PROPERTY_EDIT(component, AnimationDuration, float, "Duration")
		COMPONENT_PROPERTY_EDIT(component, AnimationTicksPerSecond, int, "Ticks per second")
		COMPONENT_PROPERTY_EDIT(component, Play, bool, "Play")

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showSkeletalAnimationHelperComponentDetails(SkeletalAnimationHelperComponent* component)
{
	if (ImGui::CollapsingHeader("Skeletal Animation Helper", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("SkeletalAnimationHelperComponentDetails");

		COMPONENT_PROPERTY_EDIT(component, ShowModelBones, bool, "Show model bones")
		COMPONENT_PROPERTY_EDIT(component, ShowAnimationBones, bool, "Show animation bones")

		if (newNode("Animation bones transform", ""))
		{
			COMPONENT_PROPERTY_EDIT(component, AnimationBonesTransformFromAnimation, bool, "Transform from animation")
			COMPONENT_PROPERTY_EDIT(component, AnimationBonesTransformDefault, bool, "Default transform")
			COMPONENT_PROPERTY_EDIT(component, AnimationBonesTransformCustom, bool, "Custom transform")

			ImGui::TreePop();
		}
		ImGui::PopID();

		COMPONENT_PROPERTY_EDIT(component, ShowFinalBones, bool, "Show final bones")
		COMPONENT_PROPERTY_EDIT(component, ShowFinalSkeletonBones, bool, "Show final skeleton bones")

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void createPhysicalBodyForPrefab(Prefab* component)
{
	if (component->getPrefabType() == PrefabType::PLANE)
	{
		PlanePrefab* plane = static_cast<PlanePrefab*>(component);

		glm::vec2 size = plane->getSize();
		PhysicalBodyBox* physicalBody = vbEditor::_sceneManager->getPhysicsManager()->createPhysicalBodyBox(btVector3(size.x, 0.1f, size.y), 1.0f, COL_ENV, COL_ENV);
		component->getSceneObject()->addComponent(physicalBody);
	}

	if (component->getPrefabType() == PrefabType::CUBE)
	{
		Cube* cube = static_cast<Cube*>(component);

		float size = cube->getSize();
		PhysicalBodyBox* physicalBody = vbEditor::_sceneManager->getPhysicsManager()->createPhysicalBodyBox(btVector3(size, size, size), 1.0f, COL_ENV, COL_ENV);
		component->getSceneObject()->addComponent(physicalBody);
	}

	if (component->getPrefabType() == PrefabType::SPHERE)
	{
		SpherePrefab* sphere = static_cast<SpherePrefab*>(component);

		PhysicalBodySphere* physicalBody = vbEditor::_sceneManager->getPhysicsManager()->createPhysicalBodySphere(sphere->getRadius(), 1.0f, COL_ENV, COL_ENV);
		component->getSceneObject()->addComponent(physicalBody);
	}

	if (component->getPrefabType() == PrefabType::CYLINDER)
	{
		CylinderPrefab* cylinder = static_cast<CylinderPrefab*>(component);

		int axis = cylinder->getAxis();
		float radius2 = cylinder->getRadius() * 2.0f;
		float height = cylinder->getHeight();
		btVector3 dimensions;
		if (axis == 0)
		{
			dimensions = btVector3(height, radius2, radius2);
		}
		else if (axis == 1)
		{
			dimensions = btVector3(radius2, height, radius2);
		}
		else if (axis == 2)
		{
			dimensions = btVector3(radius2, radius2, height);
		}
		PhysicalBodyCylinder* physicalBody = vbEditor::_sceneManager->getPhysicsManager()->createPhysicalBodyCylinder(dimensions, 1.0f, (ShapeAlign) axis, COL_ENV, COL_ENV);
		component->getSceneObject()->addComponent(physicalBody);
	}
}


void showPrefabComponentDetails(Prefab* component)
{
	if (ImGui::CollapsingHeader(convertPrefabTypeToString(component->getPrefabType()).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("PrefabComponentDetails");

		if (component->getPrefabType() == PrefabType::PLANE)
		{
			PlanePrefab* plane = static_cast<PlanePrefab*>(component);
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(plane, Size, vec2, "Size", [plane](glm::vec2 newValue) { plane->init(); }, "")
		}

		if (component->getPrefabType() == PrefabType::CUBE)
		{
			Cube* cube = static_cast<Cube*>(component);
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(cube, Size, float, "Size", [cube](float newValue) { cube->init(); }, "")
		}

		if (component->getPrefabType() == PrefabType::SPHERE)
		{
			SpherePrefab* sphere = static_cast<SpherePrefab*>(component);
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(sphere, Radius, float, "Radius", [sphere](float newValue) { sphere->init(); }, "")
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(sphere, Quality, float, "Quality", [sphere](float newValue) { sphere->init(); }, "")
		}

		if (component->getPrefabType() == PrefabType::CYLINDER)
		{
			CylinderPrefab* cylinder = static_cast<CylinderPrefab*>(component);
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(cylinder, Radius, float, "Radius", [cylinder](float newValue) { cylinder->init(); }, "")
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(cylinder, Height, float, "Height", [cylinder](float newValue) { cylinder->init(); }, "")
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(cylinder, Axis, int, "Axis", [cylinder](float newValue) { cylinder->init(); }, "")
			COMPONENT_PROPERTY_EDIT_WITH_CALLBACK(cylinder, Quality, int, "Quality", [cylinder](float newValue) { cylinder->init(); }, "")
		}

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

		if (component->getSceneObject()->getComponent(CT_PHYSICAL_BODY) == nullptr)
		{
			if (ImGui::Button("Add physical body"))
			{
				createPhysicalBodyForPrefab(component);
			}
		}
	}
}


void showAiAgentComponentDetails(AIAgent* component)
{
	if (ImGui::CollapsingHeader("AI agent", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("AIAgentComponentDetails");

		const auto& pathComponents = vbEditor::_sceneManager->getGameLogicSystem()->getPathComponents();
		std::vector<std::string> availablePaths;
		availablePaths.reserve(pathComponents.size());

		for (auto pathComponent : pathComponents)
		{
			availablePaths.push_back(pathComponent->getSceneObject()->getName());
		}

		COMPONENT_PROPERTY_EDIT(component, Speed, float, "Speed")
		COMPONENT_PROPERTY_EDIT(component, MotionFromAnimation, bool, "Motion from animation")

		COMPONENT_PROPERTY_EDIT_BEGIN(CurrentPath, "Path")
		{
			const std::vector<std::string>& itemsNames = availablePaths;
			const std::string& currentValue = component->getCurrentPath() != nullptr ? component->getCurrentPath()->getSceneObject()->getName() : "";
			std::string comboItems;
			int selectedItemIndex = 0;
			convertVectorToComboData(itemsNames, currentValue, comboItems, selectedItemIndex);
		
			bool result = false;
			std::string value = "";
			if (ImGui::Combo("##value", &selectedItemIndex, comboItems.c_str()))
			{
				result = true;
				if (selectedItemIndex > 0)
					value = itemsNames[selectedItemIndex - 1];
			}

			if (result)
			{
				if (selectedItemIndex > 0)
				{
					component->setCurrentPath(pathComponents[selectedItemIndex - 1]);
				}
				else
				{
					component->setCurrentPath(nullptr);
				}
			}
		}
		COMPONENT_PROPERTY_EDIT_END

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showAiAgentVehicleComponentDetails(AIAgentVehicle* component)
{
	if (ImGui::CollapsingHeader("AI agent Vehicle", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("AIAgentVehicleComponentDetails");

		const auto& pathComponents = vbEditor::_sceneManager->getGameLogicSystem()->getPathComponents();
		std::vector<std::string> availablePaths;
		availablePaths.reserve(pathComponents.size());

		for (auto pathComponent : pathComponents)
		{
			availablePaths.push_back(pathComponent->getSceneObject()->getName());
		}

		COMPONENT_PROPERTY_EDIT_BEGIN(CurrentPath, "Path")
		{
			const std::vector<std::string>& itemsNames = availablePaths;
			const std::string& currentValue = component->getCurrentPath() != nullptr ? component->getCurrentPath()->getSceneObject()->getName() : "";
			std::string comboItems;
			int selectedItemIndex = 0;
			convertVectorToComboData(itemsNames, currentValue, comboItems, selectedItemIndex);

			bool result = false;
			std::string value = "";
			if (ImGui::Combo("##value", &selectedItemIndex, comboItems.c_str()))
			{
				result = true;
				if (selectedItemIndex > 0)
					value = itemsNames[selectedItemIndex - 1];
			}

			if (result)
			{
				if (selectedItemIndex > 0)
				{
					component->setCurrentPath(pathComponents[selectedItemIndex - 1]);
				}
				else
				{
					component->setCurrentPath(nullptr);
				}
			}
		}
		COMPONENT_PROPERTY_EDIT_END

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showStopComponentDetails(StopComponent* component)
{
	if (ImGui::CollapsingHeader("Stop Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("BezierCurveComponentDetails");

		COMPONENT_PROPERTY_EDIT(component, DistanceToStop, float, "Distance to stop")

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showTrafficLightsComponentDetails(TrafficLightsComponent* component)
{
	if (ImGui::CollapsingHeader("Traffic Lights", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("TrafficLightsComponentDetails");

		COMPONENT_PROPERTY_EDIT(component, StopPointPosition, vec3, "Stop point")
		COMPONENT_PROPERTY_EDIT_BEGIN(CurrentPath, "Init state")
		{
			const std::string& currentValue = trafficLightsStateStrings[component->getInitState()];
			std::string comboItems;
			int selectedItemIndex = 0;
			convertVectorToComboData(&trafficLightsStateStrings[0], (unsigned int) TLS_STATE_COUNT, currentValue, comboItems, selectedItemIndex, false);

			if (ImGui::Combo("##value", &selectedItemIndex, comboItems.c_str()))
			{
				component->setInitState((TrafficLightsState) selectedItemIndex);
			}
		}
		COMPONENT_PROPERTY_EDIT_END

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showBezierCurveComponentDetails(BezierCurve* component)
{
	if (ImGui::CollapsingHeader("Bezier curve", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("BezierCurveComponentDetails");

		COMPONENT_PROPERTY_EDIT(component, MarginBegin, float, "Margin begin")
		COMPONENT_PROPERTY_EDIT(component, MarginEnd, float, "Margin end")
		COMPONENT_PROPERTY_EDIT(component, OffsetFromBaseCurve, vec2, "Offset from base curve")

		int activePoint = RoadManipulator::GetActivePoint();
		if (activePoint >= 0 && activePoint < component->getPoints().size())
		{
			ImGui::Separator();

			ImGui::Text("Point: %d", activePoint);
			ImGui::NextColumn();

			ImGui::NextColumn();

			COMPONENT_PROPERTY_EDIT_BEGIN(PointPosition, "Point position")
			glm::vec3 pointPosition = component->getPoints()[activePoint];
			if (ImGui::DragFloat3("##value", glm::value_ptr(pointPosition), 0.01f, 0.0f, 0.0f))
			{
				component->setPointPostion(activePoint, pointPosition);
			}
			COMPONENT_PROPERTY_EDIT_END

			if (activePoint % 3 == 0)
			{
				if (ImGui::Button("Delete point"))
				{
					component->deletePoint(activePoint);
				}
			}
		}

		int activeSegment = RoadManipulator::GetActiveSegment();
		if (activeSegment >= 0 && component->getSegmentsCount() > 0 && activeSegment < component->getSegmentsCount())
		{
			ImGui::Separator();

			ImGui::Text("Segment: %d", activeSegment);
			ImGui::NextColumn();

			ImGui::NextColumn();

			COMPONENT_PROPERTY_EDIT_BEGIN(SegmentPointsCount, "Segment points count")
			int pointsCount = component->getSegmentPointsCount(activeSegment);
			if (ImGui::DragInt("##value", &pointsCount, 0.5f, 2, 1000))
			{
				component->setSegmentPointsCount(activeSegment, pointsCount);
			}
			COMPONENT_PROPERTY_EDIT_END
		}

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

		if (ImGui::Button("Generate objects", ImVec2(-1.0f, 0.0f)))
		{
			vbEditor::_showGenerateObjectsAlongCurveWindow = true;
		}

		/*if (ImGui::Button("Split curve", ImVec2(-1.0f, 0.0f)))
		{
			component->splitCurve(0, 0.5f);
		}*/
	}
}


void showPathComponentDetails(PathComponent* component)
{
	if (ImGui::CollapsingHeader("AI Path", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("PathComponentDetails");

		COMPONENT_PROPERTY_EDIT(component, MaxSpeed, float, "Max speed")

		if (newNode("Next paths", "[%d]", component->getNextPaths().size()))
		{
			for (const auto& connectedPath : component->getNextPaths())
			{
				ImGui::PushID(&connectedPath);
				ImGui::AlignTextToFramePadding();
				ImGui::TreeNodeEx("#treeNode", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(-1);

				ImGui::Text(connectedPath.path->getSceneObject()->getName().c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}

			ImGui::TreePop();
		}
		ImGui::PopID();


		if (newNode("Previous paths", "[%d]", component->getPreviousPaths().size()))
		{
			for (const auto& connectedPath : component->getPreviousPaths())
			{
				ImGui::PushID(&connectedPath);
				ImGui::AlignTextToFramePadding();
				ImGui::TreeNodeEx("#treeNode", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "");
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(-1);

				ImGui::Text(connectedPath.path->getSceneObject()->getName().c_str());

				ImGui::NextColumn();

				ImGui::PopID();
			}

			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showBusStartComponentDetails(BusStartPoint* component)
{
	if (ImGui::CollapsingHeader("Bus start point", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("BusStartPointCpmponentDetails");

		COMPONENT_PROPERTY_EDIT(component, Name, str, "Point name")

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showPhysicalBodyDetails(PhysicalBody* component)
{
	if (component->getPhysicalBodyType() == PhysicalBodyType::GHOST)
	{
		PhysicalBodyGhost* ghostComponent = static_cast<PhysicalBodyGhost*>(component);
		if (ImGui::CollapsingHeader("Trigger box", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			ImGui::Columns(2);
			ImGui::Separator();
			ImGui::PushID("TriggerBoxDetails");
			
			COMPONENT_PROPERTY_EDIT(ghostComponent, Size, btVector3, "Size")

			ImGui::PopID();
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::PopStyleVar();
		}
	}
	else
	{
		if (ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
		{

		}
	}
}


void showScriptComponentDetails(ScriptComponent* component)
{
	if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("ScriptCpmponentDetails");

		COMPONENT_PROPERTY_EDIT_BEGIN(ScriptFile, "Script file")
		ImGui::Text(component->getScriptFile()->getPath().c_str());
		COMPONENT_PROPERTY_EDIT_END

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
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


void showFoliagePatchDetails(FoliagePatch* component)
{
	if (ImGui::CollapsingHeader("Foliage Patch", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::PushID("RenderComponentDetails");

		ImGui::PushID("Layers");
		for (int i = 0; i < component->getLayersCount(); ++i)
		{
			ImGui::PushID(i);
			if (newNode("Layer", ImGuiTreeNodeFlags_DefaultOpen, "%d", i))
			{
				FoliagePatchLayer& layer = component->getLayer(i);

				COMPONENT_PROPERTY_EDIT_BEGIN(RoadProfile, "Object")
				{
					showObjectSelectEdit(layer.objectName, [&layer](const std::string& newObjectName)
						{
							layer.objectName = newObjectName;
						});
				}
				COMPONENT_PROPERTY_EDIT_END

				COMPONENT_PROPERTY_EDIT_BEGIN(minDistance, "Min distance")
				{
					ImGui::DragFloat("", &layer.minDistance, 1.0f, 1, 30);
				}
				COMPONENT_PROPERTY_EDIT_END

				COMPONENT_PROPERTY_EDIT_BEGIN(seed, "Seed")
				{
					ImGui::SetNextItemWidth(-60);
					int seed = layer.seed;
					ImGui::InputInt("", &seed, 0, 0);
					layer.seed = seed;

					ImGui::SameLine();

					if (ImGui::Button("Refresh", ImVec2(50, 0)))
					{
						layer.seed = (unsigned int)std::time(0);
						component->generateFoliage();
					}
				}
				COMPONENT_PROPERTY_EDIT_END

				if (ImGui::Button("Delete layer", ImVec2(-1.0f, 0.0f)))
				{
					component->removeLayer(i);
				}
				ImGui::NextColumn();
				ImGui::NextColumn();

				ImGui::TreePop();
			}
			ImGui::PopID();

			ImGui::PopID(); // i
		}
		ImGui::PopID(); // Layers

		ImGui::Columns(1);

		ImGui::Separator();

		if (ImGui::Button("Add layer", ImVec2(-1.0f, 0.0f)))
		{
			component->addLayer("");
		}

		ImGui::Separator();

		if (ImGui::Button("Generate objects", ImVec2(-1.0f, 0.0f)))
		{
			component->generateFoliage();
		}

		ImGui::Columns(2);

		ImGui::PopID();
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
	}
}


void showObjectProperties()
{
	bool isOpened = true;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
	if (ImGui::Begin("Object Properties", &isOpened, windowFlags))
	{
		if (vbEditor::_selectedSceneObject && vbEditor::_selectedSceneObject != vbEditor::_groupingSceneObject)
		{
			showObjectPropertiesDetails();

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
				showPhysicalBodyDetails(physicsComponent);
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

			BusStopComponent* busStopComponent = dynamic_cast<BusStopComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_BUS_STOP));
			if (busStopComponent)
			{
				showBusStopComponentDetails(busStopComponent);
			}

			SkeletalAnimationComponent* skeletalAnimationComponent = dynamic_cast<SkeletalAnimationComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_SKELETAL_ANIMATION));
			if (skeletalAnimationComponent)
			{
				showSkeletalAnimationComponentDetails(skeletalAnimationComponent);
			}

			SkeletalAnimationComponent2* skeletalAnimationComponent2 = dynamic_cast<SkeletalAnimationComponent2*>(vbEditor::_selectedSceneObject->getComponent(CT_SKELETAL_ANIMATION_2));
			if (skeletalAnimationComponent2)
			{
				showSkeletalAnimationComponent2Details(skeletalAnimationComponent2);
			}

			SkeletalAnimationHelperComponent* skeletalAnimationHelperComponent = dynamic_cast<SkeletalAnimationHelperComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_SKELETAL_ANIMATION_HELPER));
			if (skeletalAnimationHelperComponent)
			{
				showSkeletalAnimationHelperComponentDetails(skeletalAnimationHelperComponent);
			}

			Prefab* prefabComponent = dynamic_cast<Prefab*>(vbEditor::_selectedSceneObject->getComponent(CT_PREFAB));
			if (prefabComponent)
			{
				showPrefabComponentDetails(prefabComponent);
				showRenderComponentDetails(prefabComponent);
			}

			AIAgent* aiAgent = dynamic_cast<AIAgent*>(vbEditor::_selectedSceneObject->getComponent(CT_AI_AGENT));
			if (aiAgent)
			{
				showAiAgentComponentDetails(aiAgent);
			}

			AIAgentVehicle* aiAgentVehicle = dynamic_cast<AIAgentVehicle*>(vbEditor::_selectedSceneObject->getComponent(CT_AI_AGENT_VEHICLE));
			if (aiAgentVehicle)
			{
				showAiAgentVehicleComponentDetails(aiAgentVehicle);
			}

			StopComponent* stopComponent = dynamic_cast<StopComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_STOP_COMPONENT));
			if (stopComponent)
			{
				showStopComponentDetails(stopComponent);
			}

			TrafficLightsComponent* trafficLightsComponent = dynamic_cast<TrafficLightsComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_TRAFFIC_LIGHTS));
			if (trafficLightsComponent)
			{
				showTrafficLightsComponentDetails(trafficLightsComponent);
			}

			BezierCurve* bezierCurve = dynamic_cast<BezierCurve*>(vbEditor::_selectedSceneObject->getComponent(CT_BEZIER_CURVE));
			if (bezierCurve)
			{
				showBezierCurveComponentDetails(bezierCurve);
			}

			PathComponent* pathComponent = dynamic_cast<PathComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_PATH));
			if (pathComponent)
			{
				showPathComponentDetails(pathComponent);
			}

			BusStartPoint* busStartPoint = dynamic_cast<BusStartPoint*>(vbEditor::_selectedSceneObject->getComponent(CT_BUS_START_POINT));
			if (busStartPoint)
			{
				showBusStartComponentDetails(busStartPoint);
			}

			ScriptComponent* scriptComponent = dynamic_cast<ScriptComponent*>(vbEditor::_selectedSceneObject->getComponent(CT_SCRIPT));
			if (scriptComponent)
			{
				showScriptComponentDetails(scriptComponent);
			}

			FoliagePatch* foliagePatch = dynamic_cast<FoliagePatch*>(vbEditor::_selectedSceneObject->getComponent(CT_FOLIAGE_PATCH));
			if (foliagePatch)
			{
				showFoliagePatchDetails(foliagePatch);
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
				showRenderComponentDetails(roadComponent);
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
							roadComponent->needRebuild();
						}

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
							roadComponent->needRebuild();
							roadComponent->getSegments()[vbEditor::roadActiveSegment].type = typeComboCurrentItem == 0 ? RST_LINE : RST_ARC;
						}

						int interpolationComboCurrentItem = roadComponent->getSegments()[vbEditor::roadActiveSegment].interpolation == RI_LIN ? 0 : 1;
						const char* interpolationComboItems[] = { "lin", "cos" };
						if (ImGui::Combo("Interpolation", &interpolationComboCurrentItem, interpolationComboItems, IM_ARRAYSIZE(interpolationComboItems)))
						{
							roadComponent->needRebuild();
							roadComponent->getSegments()[vbEditor::roadActiveSegment].interpolation = interpolationComboCurrentItem == 0 ? RI_LIN : RI_COS;
						}

						int* points = &(roadComponent->getSegments()[vbEditor::roadActiveSegment].pointsCount);
						if (ImGui::DragInt("Points count", points, 5.0f, 0.0f, 0.0f))
						{
							roadComponent->needRebuild();
						}

						float* radius = &(roadComponent->getSegments()[vbEditor::roadActiveSegment].r);
						if (ImGui::DragFloat("Radius", radius, 0.01f, 0.0f, 0.0f))
						{
							roadComponent->needRebuild();
						}
					}

					ImGui::Separator();


					shwoRoadProfileEdit(roadComponent->getRoadProfile()->getName(), [roadComponent](const std::string& newRoadProfile)
						{
							roadComponent->setRoadProfile(ResourceManager::getInstance().loadRoadProfile(newRoadProfile));
						});

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
							}
						}
						ImGui::PopID();
					}


				}
			}
			
			ImGui::Separator();

			if (ImGui::Button("Delete"))
			{
				vbEditor::_sceneManager->removeSceneObject(vbEditor::_selectedSceneObject, true);
				vbEditor::_selectedSceneObject = nullptr;
			}
		}
	}
	ImGui::End();
}


void showObjectPropertiesDetails()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::Columns(2);
	ImGui::Separator();

	COMPONENT_PROPERTY_EDIT(vbEditor::_selectedSceneObject, Name, str, "Name")

	ImGui::Separator();

	COMPONENT_PROPERTY_EDIT(vbEditor::_selectedSceneObject, Position, vec3, "Position")

	COMPONENT_PROPERTY_EDIT_BEGIN(Rotation, "Rotation")
	{
		glm::vec3 value = vbEditor::_selectedSceneObject->getRotation();
		value = glm::vec3(radToDeg(value.x), radToDeg(value.y), radToDeg(value.z));
		bool result = ImGui::DragFloat3("##value", glm::value_ptr(value));

		if (result)
		{
			vbEditor::_selectedSceneObject->setRotation(degToRad(value.x), degToRad(value.y), degToRad(value.z));
		}
	}
	COMPONENT_PROPERTY_EDIT_END	

	//COMPONENT_PROPERTY_EDIT(vbEditor::_selectedSceneObject, Rotation, vec3, "Rotation")
	COMPONENT_PROPERTY_EDIT(vbEditor::_selectedSceneObject, Scale, vec3, "Scale")

	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::PopStyleVar();
}