#include "OpenDialogWindow.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../../Utils/FilesHelper.h"
#include "../../Utils/ResourceDescriptionUtils.h"
#include "../../Utils/Strings.h"


OpenDialogWindow::OpenDialogWindow(const std::string& title, const std::string& buttonOkTitle, const std::vector<std::string>& rootPaths, SceneManager* sceneManager, bool isOpen)
	: ImGuiWindow(nullptr, isOpen),
	_title(title), _buttonOkTitle(buttonOkTitle), _rootPaths(rootPaths),
	_currentItem(-1), _currentRootFileIndex(0), _selectedFile(nullptr)
{

}


void OpenDialogWindow::showBasePathCombo()
{
	if (_rootFiles.size() > 1)
	{
		std::vector<const char*> comboItems;
		for (int i = 0; i < _rootFiles.size(); ++i)
		{
			comboItems.push_back(_rootFiles[i].path.c_str());
		}

		ImGui::Text("Base path:");
		ImGui::SameLine();
		ImGui::Combo("##basePathCombo", &_currentRootFileIndex, comboItems.data(), comboItems.size());
	}
}


void OpenDialogWindow::inspectPath(File& file)
{
	if (!file.isChildrenInitialized)
	{
		std::vector<std::string> childrenPaths = FilesHelper::getDirectoriesList(file.path + "/");
		for (const auto& childPath : childrenPaths)
		{
			file.children.push_back(File(childPath, file.path + "/" + childPath));

			File& childFile = file.children[file.children.size() - 1];

			if (_directoryFilter)
			{
				childFile.isSelectable = _directoryFilter(childFile.path);
			}
		}

		file.isChildrenInitialized = true;
	}

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (file.isChildrenInitialized && file.children.size() == 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (&file == _selectedFile)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	if (&file == &_rootFiles[_currentRootFileIndex])
	{
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
	}

	bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)(&file), nodeFlags, file.name.c_str());

	if (ImGui::IsItemClicked() && file.isSelectable)
	{
		_selectedFile = &file;
		if (!file.isDescriptionInitialized && _descriptionLoader)
		{
			_descriptionLoader(file.path, file.description);
			file.isDescriptionInitialized = true;
		}
	}

	if (isNodeOpen)
	{
		for (auto& child : file.children)
		{
			inspectPath(child);
		}

		ImGui::TreePop();
	}
}


void OpenDialogWindow::drawWindow()
{
	if (_currentItem < 0)
	{
		ImGui::OpenPopup(_title.c_str());
		_currentItem = 0;

		_rootFiles.clear();
		for (const auto& path : _rootPaths)
		{
			_rootFiles.push_back(File(path, path));
		}
		_currentRootFileIndex = 0;

		_selectedFile = nullptr;
	}

	if (ImGui::BeginPopupModal(_title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		showBasePathCombo();

		{
			ImGui::BeginGroup();
			if (ImGui::BeginChild("tree", ImVec2(200.0f, 300.0f), true))
			{
				inspectPath(_rootFiles[_currentRootFileIndex]);
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
				if (_selectedFile != nullptr)
				{
					ImGui::Text("Name: %s", _selectedFile->description.name.c_str());
					ImGui::Text("Author: %s", _selectedFile->description.author.c_str());
					ImGui::TextWrapped("Description: %s", _selectedFile->description.comment.c_str());
				}
			}
			ImGui::EndChild();
			ImGui::EndGroup();
		}

		if (ImGui::Button(_buttonOkTitle.c_str(), ImVec2(0, 0)))
		{
			if (_selectedFile != nullptr)
			{
				ImGui::CloseCurrentPopup();

				if (_onOkClickCallback)
				{
					_onOkClickCallback(_selectedFile->path.substr(_rootFiles[_currentRootFileIndex].path.length() + 1));
				}

				_currentItem = -1;

				_isOpen = false;
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(0, 0)))
		{
			ImGui::CloseCurrentPopup();
			_currentItem = -1;

			_isOpen = false;
		}
	}
	ImGui::EndPopup();
}


void OpenDialogWindow::setOnOkClickCallback(const std::function<void(const std::string&)>& onOkClickCallback)
{
	_onOkClickCallback = onOkClickCallback;
}


void OpenDialogWindow::setDirectoryFilter(const std::function<bool(const std::string&)>& directoryFilter)
{
	_directoryFilter = directoryFilter;
}


void OpenDialogWindow::setDescriptionLoader(const std::function<void(const std::string&, ResourceDescription&)>& descriptionLoader)
{
	_descriptionLoader = descriptionLoader;
}


void OpenDialogWindow::setDefaultDirectoryFilter(const std::string& resourceFileName)
{
	setDirectoryFilter([resourceFileName](const std::string& directoryPath)
		{
			std::vector<std::string> children = FilesHelper::getFilesList(directoryPath + "/");
			for (const auto& fileName : children)
			{
				if (fileName == resourceFileName)
				{
					return true;
				}
			}
			return false;
		});
}


void OpenDialogWindow::setDefaultDescriptionLoader(const std::string& resourceFileName, const std::string& rootElementName)
{
	setDescriptionLoader([resourceFileName, rootElementName](const std::string& directoryPath, ResourceDescription& outDescription)
		{
			ResourceDescriptionUtils::loadResourceDescription(directoryPath + "/" + resourceFileName, rootElementName, outDescription);
		});
}


int currentItem = 0;


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
