#include "OpenDialogWindow.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../../Utils/FilesHelper.h"
#include "../../Utils/Strings.h"


int currentItem = -1;

struct File
{
	std::string name;
	std::string path;
	std::vector<File> children;
	bool isSelectable;
	bool isChildrenInitialized;

	File(const std::string& name = "", const std::string& path = "")
		: name(name), path(path), isSelectable(true), isChildrenInitialized(false)
	{}
};


File rootFile;
File* selectedFile;


void inspectPath(File& file, const std::function<bool(const std::string&)>& directoryFilter)
{
	if (!file.isChildrenInitialized)
	{
		std::vector<std::string> childrenPaths = FilesHelper::getDirectoriesList(file.path + "/");
		for (const auto& childPath : childrenPaths)
		{
			file.children.push_back(File(childPath, file.path + "/" + childPath));

			File& childFile = file.children[file.children.size() - 1];

			if (directoryFilter != nullptr)
			{
				childFile.isSelectable = directoryFilter(childFile.path);
			}
		}

		file.isChildrenInitialized = true;
	}

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (file.isChildrenInitialized && file.children.size() == 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (&file == selectedFile)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	if (&file == &rootFile)
	{
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
	}

	bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)(&file), nodeFlags, file.name.c_str());

	if (ImGui::IsItemClicked() && file.isSelectable)
	{
		selectedFile = &file;
	}

	if (isNodeOpen)
	{
		for (auto& child : file.children)
		{
			inspectPath(child, directoryFilter);
		}

		ImGui::TreePop();
	}
}


bool openDialogWindow2(const std::string& title, const std::string& buttonOkTitle, const std::string& rootPath, const std::function<void(const std::string&)>& onOkClickCallback,
					   const std::function<bool(const std::string&)>& directoryFilter)
{
	if (currentItem < 0)
	{
		ImGui::OpenPopup(title.c_str());
		currentItem = 0;
		rootFile = File(rootPath, rootPath);
	}

	bool isOpend = true;
	if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		{
			ImGui::BeginGroup();
			if (ImGui::BeginChild("tree", ImVec2(200.0f, 300.0f), true))
			{
				inspectPath(rootFile, directoryFilter);
			}
			ImGui::EndChild();
			ImGui::EndGroup();
		}
		//ImGui::ListBox("", &currentItem, map_list.data(), items_count, 5);
		//inspectPath(rootFile, directoryFilter);

		ImGui::SameLine();

		{
			ImGui::BeginGroup();
			if (ImGui::BeginChild("details", ImVec2(200.0f, 300.0f), true))
			{
				ImGui::Text("Name: %s", "TODO");
				ImGui::Text("Description: %s", "TODO");
				ImGui::Text("Author: %s", "TODO");
			}
			ImGui::EndChild();
			ImGui::EndGroup();
		}

		if (ImGui::Button(buttonOkTitle.c_str(), ImVec2(0, 0)))
		{
			ImGui::CloseCurrentPopup();

			onOkClickCallback(selectedFile->path.substr(rootFile.path.length() + 1));
			
			currentItem = -1;

			isOpend = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(0, 0)))
		{
			ImGui::CloseCurrentPopup();
			currentItem = -1;

			isOpend = false;
		}
	}
	ImGui::EndPopup();

	return isOpend;
}


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

	bool isOpend = true;
	if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::ListBox("##listBox", &currentItem, map_list.data(), items_count, 5);

		if (ImGui::Button(buttonOkTitle.c_str(), ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();

			onOkClickCallback(currentItem);

			currentItem = -1;

			isOpend = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			currentItem = -1;

			isOpend = false;
		}
	}
	ImGui::EndPopup();

	return isOpend;
}
