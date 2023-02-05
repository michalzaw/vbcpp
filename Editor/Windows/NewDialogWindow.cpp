#include "NewDialogWindow.h"

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>

#include <glm/glm.hpp>
#include "../../Graphics/Renderer.h"

/*

NewDialogWindow::NewDialogWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::list<EditorEvent>* events, bool isOpen)
    : EditorWindow(sceneManager, selectedSceneObject, isOpen, events),
    _inputTextNameBuffer(""), _inputTextAuthorBuffer("")
{

}


void NewDialogWindow::drawWindow()
{
    if (ImGui::Begin("New map", &_isOpen))
    {
        ImGui::InputText("Map name", _inputTextNameBuffer, IM_ARRAYSIZE(_inputTextNameBuffer));
        ImGui::InputText("Author", _inputTextAuthorBuffer, IM_ARRAYSIZE(_inputTextAuthorBuffer));

        if (ImGui::Button("Create"))
        {
            _events->push_back(EditorEvent(EET_NEW_CLICK));

            _isOpen = false;
        }
    }
    ImGui::End();
}


void NewDialogWindow::open()
{
    memset(_inputTextNameBuffer, 0, strlen(_inputTextNameBuffer));
    memset(_inputTextAuthorBuffer, 0, strlen(_inputTextAuthorBuffer));

    _isOpen = true;
}
*/

bool NewDialogWindow()
{
	glm::uvec2 mainWindowSize(Renderer::getInstance().getWindowDimensions());

	ImGui::SetNextWindowSize(ImVec2(250, 100));
	ImGui::SetNextWindowPos(ImVec2(mainWindowSize.x / 2 - 100, mainWindowSize.y / 2 - 50));

	ImGuiWindow* window = ImGui::GetCurrentWindow();

	bool result = false;
	bool isOpened = true;
	/*
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("Create new map...", &isOpened, windowFlags))
	{
		char nameBuffer[50];
		char authorBuffer[50];

		//static MapInfo tmp = { mapInfo.name, mapInfo.author };


		strncpy(nameBuffer, tmp.name.c_str(), sizeof nameBuffer);
		strncpy(authorBuffer, tmp.author.c_str(), sizeof authorBuffer);

		nameBuffer[sizeof nameBuffer - 1] = '\0';
		authorBuffer[sizeof authorBuffer - 1] = '\0';

		if (ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer)))
		{
			tmp.name = std::string(nameBuffer);
		}

		if (ImGui::InputText("Author", authorBuffer, IM_ARRAYSIZE(authorBuffer)))
		{
			tmp.author = std::string(authorBuffer);
		}

		if (ImGui::Button("Create"))
		{
			//mapInfo.name = tmp.name;
			//mapInfo.author = tmp.author;
			result = true;
			isOpened = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			//tmp.name = mapInfo.name;
			//tmp.author = mapInfo.author;

			result = true;
			isOpened = false;
		}
	}
	ImGui::End();
	*/
	return result;
}