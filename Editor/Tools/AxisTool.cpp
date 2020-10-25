#include "AxisTool.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../ImGui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../../ImGui/imgui_internal.h"

namespace AxisTool
{
	struct Context
	{
		ImDrawList* drawList;

		float x;
		float y;
		float width;
		float height;
	};

	static Context context;

	static ImVec2 transformToImGuiScreenSpace(glm::vec4 position)
	{
		position *= 0.5 / position.w;
		position += glm::vec4(0.5f, 0.5f, 0.0f, 0.0f);
		position.y = 1.f - position.y;
		position.x *= context.width;
		position.y *= context.height;
		position.x += context.x;
		position.y += context.y;
		return ImVec2(position.x, position.y);
	}

	void BeginFrame()
	{
		ImGuiIO& io = ImGui::GetIO();

		const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::SetNextWindowSize(io.DisplaySize);
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
		ImGui::PushStyleColor(ImGuiCol_Border, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("axisTool", NULL, flags);
		context.drawList = ImGui::GetWindowDrawList();
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(2);
	}

	void SetRect(float x, float y, float width, float height)
	{
		context.x = x;
		context.y = y;
		context.width = width;
		context.height = height;
	}

	void Show(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraDirection)
	{
		glm::mat4 MVP = projection * view;

		glm::vec4 center(4.0f * cameraDirection.x, 4.0f * cameraDirection.y, 4.0f * cameraDirection.z, 1.0f);
		glm::vec4 xAxis = MVP * (center + glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		glm::vec4 yAxis = MVP * (center + glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		glm::vec4 zAxis = MVP * (center + glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

		center = MVP * center;

		ImVec2 center2 = transformToImGuiScreenSpace(center);
		ImVec2 xAxis2 = transformToImGuiScreenSpace(xAxis);
		ImVec2 yAxis2 = transformToImGuiScreenSpace(yAxis);
		ImVec2 zAxis2 = transformToImGuiScreenSpace(zAxis);

		context.drawList->AddLine(center2, xAxis2, IM_COL32(255, 0, 0, 255), 3.0f);
		context.drawList->AddLine(center2, yAxis2, IM_COL32(0, 255, 0, 255), 3.0f);
		context.drawList->AddLine(center2, zAxis2, IM_COL32(0, 0, 255, 255), 3.0f);

		context.drawList->AddText(xAxis2, IM_COL32(255, 0, 0, 255), "x");
		context.drawList->AddText(yAxis2 + ImVec2(5.0f, -5.0f), IM_COL32(0, 255, 0, 255), "y");
		context.drawList->AddText(zAxis2, IM_COL32(0, 0, 255, 255), "z");
	}
}
