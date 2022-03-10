#include "GenerateObjectsAlongRoadWindow.h"

#include "../../ImGui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenDialogWindow.h"
#include "../Utils/ObjectsGenerator.h"

#include "../../Graphics/RoadObject.h"


class SceneManager;


namespace vbEditor
{
	extern SceneManager* _sceneManager;
	extern OpenDialogWindow* _addSceneObjectDialogWindow;

	int _objectsNamesCurrentItem;

	ObjectsGenerator::ObjectsAlongRoadGeneratorData _generatorData;

	void showObjectsList()
	{
		int itemsCount = _generatorData.objectsNames.size();
		std::vector<const char*> objectsNamesItems;

		for (int i = 0; i < itemsCount; ++i)
		{
			objectsNamesItems.push_back(_generatorData.objectsNames[i].c_str());
		}

		ImGui::ListBox("Objects", &_objectsNamesCurrentItem, objectsNamesItems.data(), itemsCount);

		if (ImGui::Button("Add object"))
		{
			*(_addSceneObjectDialogWindow->getOpenFlagPointer()) = true;
			_addSceneObjectDialogWindow->setOnOkClickCallback([](const std::string& selectedObjectName)
				{
					_generatorData.objectsNames.push_back(selectedObjectName);

				});
		}

		ImGui::SameLine();

		if (ImGui::Button("Delete object"))
		{
			_generatorData.objectsNames.erase(_generatorData.objectsNames.begin() + _objectsNamesCurrentItem);
		}
	}

	void showLocationParameters()
	{
		ImGui::DragInt("Distance between objects", &_generatorData.distance, 0.1f, 1, 1000);
		ImGui::DragFloat2("Offset from the road axis", glm::value_ptr(_generatorData.offsetFromCenter), 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(_generatorData.rotation), 0.1f, 0.0f, 360.0f);

		ImGui::Checkbox("Right", &_generatorData.rightSide);
		ImGui::SameLine();
		ImGui::Checkbox("Left", &_generatorData.leftSide);
	}

	void showRandomizationParameters()
	{
		ImGui::DragFloat3("Range of random position offset", glm::value_ptr(_generatorData.positionOffset), 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat3("Range of random rotation offset", glm::value_ptr(_generatorData.rotationOffset), 0.1f, 0.0f, 360.0f);
	}

	bool generateObjectsAlongRoadWindow(RoadObject* roadComponent)
	{
		bool isOpen = true;

		if (ImGui::Begin("Generate objects along the road...", &isOpen))
		{
			showObjectsList();

			ImGui::Separator();

			showLocationParameters();

			ImGui::Separator();

			showRandomizationParameters();

			if (ImGui::Button("Generate"))
			{
				ObjectsGenerator::generateObjectsAlongRoad(roadComponent, &_generatorData, _sceneManager);
			}
		}
		ImGui::End();

		return isOpen;
	}
}
