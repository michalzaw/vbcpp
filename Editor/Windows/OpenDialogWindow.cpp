#include "OpenDialogWindow.h"

#include "../../Game/Directories.h"

#include "../../Utils/FilesHelper.h"


OpenDialogWindow::OpenDialogWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen)
    : EditorWindow(sceneManager, selectedSceneObject, isOpen, events),
    _listBoxMapSelectedItem(0), _openModalDialog(isOpen)
{
	if (isOpen)
	{
		open();
	}
}


void OpenDialogWindow::drawWindow()
{
	if (_openModalDialog)
	{
		ImGui::OpenPopup("Open map");
		_openModalDialog = false;
	}

	if (ImGui::BeginPopupModal("Open map", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        std::vector<const char*> availableMapsNamesCstr;
        for (int i = 0; i < _availableMapsNames.size(); ++i)
        {
            availableMapsNamesCstr.push_back(_availableMapsNames[i].c_str());
        }

        ImGui::ListBox("Map", &_listBoxMapSelectedItem, &availableMapsNamesCstr[0], availableMapsNamesCstr.size(), 4);

        if (ImGui::Button("Open", ImVec2(120, 0)))
        {
            std::string mapName = (_availableMapsNames[_listBoxMapSelectedItem]);
            mapName = trim(mapName);

            _events->push_back(EditorEvent(EET_OPEN_CLICK, mapName));

			_isOpen = false;
			ImGui::CloseCurrentPopup();
        }

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			_isOpen = false;
			ImGui::CloseCurrentPopup();
		}
    }
    ImGui::EndPopup();
}


void OpenDialogWindow::open()
{
    _selectedSceneObject = 0;

    _availableMapsNames.clear();
    _availableMapsNames = FilesHelper::getDirectoriesList(GameDirectories::MAPS);

    _isOpen = true;
	_openModalDialog = true;
}
