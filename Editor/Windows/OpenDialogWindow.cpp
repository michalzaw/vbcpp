#include "OpenDialogWindow.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


int currentItem = -1;


bool openDialogWindow(const std::string& title, const std::string& buttonOkTitle, const std::vector<std::string>& items, const std::function<void(int)> onOkClickCallback)
{
	if (currentItem < 0)
	{
		ImGui::OpenPopup(title.c_str());
		currentItem = 0;
	}

	int items_count = items.size();
	std::vector<const char*> map_list;

	for (int i = 0; i < items.size(); ++i)
	{
		map_list.push_back(items[i].c_str());
	}

	if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::ListBox("", &currentItem, map_list.data(), items_count, 5);

		if (ImGui::Button(buttonOkTitle.c_str(), ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();

			onOkClickCallback(currentItem);

			currentItem = -1;

			return false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			currentItem = -1;

			return false;
		}
		ImGui::EndPopup();
	}

	return true;
}
