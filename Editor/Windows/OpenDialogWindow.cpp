#include "OpenDialogWindow.h"

#include "../../ImGui/imgui.h"
#include "../../ImGui/imgui_impl_glfw.h"
#include "../../ImGui/imgui_impl_opengl3.h"



bool openMapDialog(const std::vector<std::string>& items, int& current_item)
{
	bool closeWindow = false;

	int items_count = items.size();
	std::vector<const char*> map_list;

	for (int i = 0; i < items.size(); ++i)
	{
		map_list.push_back(items[i].c_str());
	}

	ImGui::OpenPopup("Open map");

	if (ImGui::BeginPopupModal("Open map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::ListBox("Map", &current_item, map_list.data(), items_count, 5);

		if (ImGui::Button("Open", ImVec2(120, 0)))
		{
			closeWindow = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			//_isOpen = false;
			closeWindow = true;
			ImGui::CloseCurrentPopup();
		}
	}
	ImGui::EndPopup();

	return closeWindow;
}

bool openMapDialog(std::string title, std::string buttonTitle, const std::vector<std::string>& items, int & current_item)
{
	bool closeWindow = false;

	int items_count = items.size();
	std::vector<const char*> map_list;

	for (int i = 0; i < items.size(); ++i)
	{
		map_list.push_back(items[i].c_str());
	}

	ImGui::OpenPopup(title.c_str());

	if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::ListBox("", &current_item, map_list.data(), items_count, 5);

		if (ImGui::Button(buttonTitle.c_str(), ImVec2(120, 0)))
		{
			closeWindow = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			//_isOpen = false;
			closeWindow = true;
			ImGui::CloseCurrentPopup();
		}
	}
	ImGui::EndPopup();

	return closeWindow;
}
