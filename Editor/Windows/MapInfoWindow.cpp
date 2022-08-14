#include "MapInfoWindow.h"

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>

#include <glm/glm.hpp>
#include "../../Graphics/Renderer.h"

bool MapInfoWindow(MapInfo& mapInfo, std::string buttonText)
{
	glm::uvec2 mainWindowSize(Renderer::getInstance().getWindowDimensions());

	ImGui::SetNextWindowSize(ImVec2(250, 100));
	ImGui::SetNextWindowPos(ImVec2(mainWindowSize.x / 2 - 100, mainWindowSize.y / 2 - 50));

	ImGuiWindow* window = ImGui::GetCurrentWindow();

	bool result = false;
	bool isOpened = true;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("Map Info", &isOpened, windowFlags))
	{
		char nameBuffer[50];
		char authorBuffer[50];

		MapInfo tmp = {mapInfo.name, mapInfo.author};


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

		if (ImGui::Button(buttonText.c_str()))
		{
			mapInfo.name = tmp.name;
			mapInfo.author = tmp.author;
			result = true;
			isOpened = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			tmp.name = mapInfo.name;
			tmp.author = mapInfo.author;

			result = true;
			isOpened = false;
		}
	}
	ImGui::End();

	return result;
}
