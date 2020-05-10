#include "RoadManipulator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../ImGui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../../ImGui/imgui_internal.h"

#include "../../ImGui/imGizmo.h"

#include "../../Graphics/Roads.h"

namespace RoadManipulator
{
	struct Context
	{
		ImDrawList* drawList;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 modelMatrix;
		glm::mat4 MVP;
		glm::mat4 viewProjection;

		float x = 0.f;
		float y = 0.f;
		float width = 0.f;
		float height = 0.f;
		float xMax = 0.f;
		float yMax = 0.f;
		float displayRatio = 1.f;

		int activePoint = -1;

		std::vector<ImVec2> roadPoints;
	};

	static Context context;

	void BeginFrame()
	{
		ImGuiIO& io = ImGui::GetIO();

		const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::SetNextWindowSize(io.DisplaySize);
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
		ImGui::PushStyleColor(ImGuiCol_Border, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("roadManipulator", NULL, flags);
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
		context.xMax = context.x + context.width;
		context.yMax = context.y + context.xMax;
		context.displayRatio = width / height;
	}

	static void ComputeContext(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix)
	{
		context.viewMatrix = view;
		context.projectionMatrix = projection;
		context.modelMatrix = matrix;

		context.viewProjection = context.projectionMatrix * context.viewMatrix;
		context.MVP = context.viewProjection * context.modelMatrix;

		context.roadPoints.clear();
	}

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

	static bool CanActivate()
	{
		if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive())
			return true;
		return false;
	}

	void Manipulate(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix, std::vector<RoadSegment>& segments, float* deltaMatrix)
	{
		ComputeContext(view, projection, matrix);

		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* drawList = context.drawList;

		for (unsigned int i = 0; i < segments.size(); ++i)
		{
			glm::vec4 position(segments[i].begin.x, segments[i].begin.y, segments[i].begin.z, 1.0f);
			position = context.MVP * position;

			if (position.z < 0.001f)
			{
				continue;
			}

			ImVec2 trans = transformToImGuiScreenSpace(position);

			if (trans.x <= 0.0f || trans.x >= context.width || trans.y <= 0.0f || trans.y >= context.height)
			{
				continue;
			}

			drawList->AddCircleFilled(trans, 10.0f, 0xFF000000);
			drawList->AddCircleFilled(trans, 7.0f, 0xFFFFFF00);
			if (context.activePoint == i)
			{
				glm::mat4 modelMatrix = glm::translate(glm::vec3(segments[i].begin.x, segments[i].begin.y, segments[i].begin.z));

				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				ImGuizmo::Manipulate(glm::value_ptr(context.viewMatrix), glm::value_ptr(context.projectionMatrix),
					ImGuizmo::TRANSLATE, ImGuizmo::WORLD,
					glm::value_ptr(modelMatrix),
					NULL,
					NULL,
					NULL,
					NULL
				);

				glm::vec3 newPosition = modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				segments[i].begin = newPosition;
			}

			context.roadPoints.push_back(trans);

			// click
			if (glm::length(glm::vec2(io.MousePos.x - trans.x, io.MousePos.y - trans.y)) <= 10.0f &&
				CanActivate())
			{
				context.activePoint = i;
			}
		}
	}

};
