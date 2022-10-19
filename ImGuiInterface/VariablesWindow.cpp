#include "VariablesWindow.h"

#include "../Game/GameEnvironment.h"


VariablesWindow::VariablesWindow(SceneManager* sceneManager, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen),
	_newVariableFloatName(""), _newVariableFloatValue(0.0f),
	_newVariablesStringName(""), _newVariablesStringValue("")
{
	GameEnvironment::Variables::stringVaribles["testowazmienna1"] = "test123";
	GameEnvironment::Variables::stringVaribles["fooBar"] = "Hello World";
}


void showInputText(const std::string& name, std::string& value)
{
	char buffer[128];

	strncpy(buffer, value.c_str(), sizeof buffer);

	buffer[sizeof buffer - 1] = '\0';

	if (ImGui::InputText(name.c_str(), buffer, IM_ARRAYSIZE(buffer)))
	{
		value = std::string(buffer);
	}
}


void VariablesWindow::drawWindow()
{
	if (ImGui::Begin("Game variables", &_isOpen))
	{
		if (ImGui::CollapsingHeader("Float variables", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto& variable : GameEnvironment::Variables::floatVaribles)
			{
				ImGui::DragFloat(variable.first.c_str(), &variable.second);
			}

			ImGui::Separator();

			ImGui::PushID("AddNewFloatVariable");
			showInputText("Name", _newVariableFloatName);
			ImGui::DragFloat("Value", &_newVariableFloatValue);
			if (ImGui::Button("Add new"))
			{
				GameEnvironment::Variables::floatVaribles[_newVariableFloatName] = _newVariableFloatValue;

				_newVariableFloatName = "";
				_newVariableFloatValue = 0.0f;
			}
			ImGui::PopID();
		}
		if (ImGui::CollapsingHeader("String variables", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto& variable : GameEnvironment::Variables::stringVaribles)
			{
				showInputText(variable.first, variable.second);
			}

			ImGui::Separator();

			ImGui::PushID("AddNewStringVariable");
			showInputText("Name", _newVariablesStringName);
			showInputText("Value", _newVariablesStringValue);
			if (ImGui::Button("Add new"))
			{
				GameEnvironment::Variables::stringVaribles[_newVariablesStringName] = _newVariablesStringValue;

				_newVariablesStringName = "";
				_newVariablesStringValue = "";
			}
			ImGui::PopID();
		}
	}
	ImGui::End();
}
