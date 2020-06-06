#include "ObjectDefinitionsWindow.h"

#include "../../Game/Directories.h"

#include "../../Utils/FilesHelper.h"


ObjectDefinitionsWindow::ObjectDefinitionsWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::shared_ptr<EditorContext> editorContext)
	: EditorWindow(sceneManager, selectedSceneObject, false),
	_editorContext(editorContext), _listBoxObjectsSelectedItem(0)
{

}


void ObjectDefinitionsWindow::drawWindow()
{
	if (ImGui::Begin("Objects", &_isOpen))
	{
		std::vector<const char*> availableObjectsNamesCstr;
		for (int i = 0; i < _editorContext->availableObjects.size(); ++i)
		{
			availableObjectsNamesCstr.push_back(_editorContext->availableObjects[i].c_str());
		}


		RObject* object = ResourceManager::getInstance().loadRObject(_editorContext->availableObjects[_listBoxObjectsSelectedItem]);

		ImGui::PushItemWidth(80);

		ImGui::PushID(0);
		ImGui::ListBox("", &_listBoxObjectsSelectedItem, &availableObjectsNamesCstr[0], availableObjectsNamesCstr.size());
		ImGui::PopID();

		ImGui::SameLine();

		ImGui::PushID(1);
		ImGui::BeginGroup();
		if (ImGui::BeginTabBar("", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Components"))
			{
				std::vector<std::unordered_map<std::string, std::string>>&  components = object->getComponents();

				for (int i = 0; i < components.size(); ++i)
				{
					if (components[i]["type"] == "render" && ImGui::CollapsingHeader("Render Component"))
					{
						static char buffer[50];

						strncpy(buffer, components[i]["model"].c_str(), sizeof buffer);

						buffer[sizeof buffer - 1] = '\0';

						if (ImGui::InputText("Model name", buffer, IM_ARRAYSIZE(buffer),
							ImGuiInputTextFlags_EnterReturnsTrue))
						{
							components[i]["model"] = std::string(buffer);
						}
					}
					if (components[i]["type"] == "physics" && ImGui::CollapsingHeader("Physical Body Component"))
					{
						ImGui::Text("In next version!");

					}
					if (components[i]["type"] == "tree" && ImGui::CollapsingHeader("Tree Component"))
					{
						ImGui::Text("In next version!");

					}
					if (components[i]["type"] == "sound" && ImGui::CollapsingHeader("Sound Component"))
					{
						ImGui::Text("In next version!");

					}
					if (components[i]["type"] == "bus-stop" && ImGui::CollapsingHeader("Bus stop Component"))
					{
						ImGui::Text("In next version!");

					}
					if (components[i]["type"] == "environmentCapture" && ImGui::CollapsingHeader("Environment capture Component"))
					{
						ImGui::Text("In next version!");

					}
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Preview"))
			{
				ImGui::Text("In next version!");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndGroup();
		ImGui::PopID();


		ImGui::PopItemWidth();

		

	}
	ImGui::End();
}
