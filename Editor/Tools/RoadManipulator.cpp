#include "RoadManipulator.h"

#include <algorithm>

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

#include "../../Utils/Helpers.hpp"


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

		bool isAnyPointModified = false;
		int activePoint = -1;
		int activeSegment = -1;
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

	int IsModified()
	{
		return context.isAnyPointModified;
	}

	int GetActiveSegment()
	{
		return context.activeSegment;
	}

	static void ComputeContext(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix)
	{
		context.viewMatrix = view;
		context.projectionMatrix = projection;
		context.modelMatrix = matrix;

		context.viewProjection = context.projectionMatrix * context.viewMatrix;
		context.MVP = context.viewProjection * context.modelMatrix;

		context.isAnyPointModified = false;
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

	// return true - if the point is drawn
	bool DrawPoint(glm::vec4 position, ImVec2& trans)
	{
		ImDrawList* drawList = context.drawList;

		position = context.MVP * position;

		if (position.z < 0.001f)
		{
			return false;
		}

		trans = transformToImGuiScreenSpace(position);

		if (trans.x <= 0.0f || trans.x >= context.width || trans.y <= 0.0f || trans.y >= context.height)
		{
			return false;
		}

		//drawList->AddCircleFilled(trans, 10.0f, 0xFF000000);
		//drawList->AddCircleFilled(trans, 7.0f, 0xFFFFFF00);

		//drawList->AddCircleFilled(trans, 10.0f, IM_COL32(255, 0, 0, 255));
		drawList->AddCircleFilled(trans, 6.0f, IM_COL32(255, 255, 255, 255));

		return true;
	}

	// return new point position
	glm::vec3 HandleInputForPoint(glm::vec4 position, ImVec2& trans, int index)
	{
		glm::vec3 newPosition = glm::vec3(position);

		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* drawList = context.drawList;

		// draw guizmo
		if (context.activePoint == index)
		{
			glm::mat4 modelMatrix = glm::translate(glm::vec3(position));

			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			ImGuizmo::Manipulate(glm::value_ptr(context.viewMatrix), glm::value_ptr(context.projectionMatrix),
				ImGuizmo::TRANSLATE, ImGuizmo::WORLD,
				glm::value_ptr(modelMatrix),
				NULL,
				NULL,
				NULL,
				NULL
			);

			newPosition = modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		float mouseToPointDistance = glm::length(glm::vec2(io.MousePos.x - trans.x, io.MousePos.y - trans.y));

		// mouse is over point
		if (mouseToPointDistance <= 6.0f)
		{
			drawList->AddCircleFilled(trans, 6.0f, 0x8A1080FF);
		}

		// click
		if (mouseToPointDistance <= 6.0f && CanActivate())
		{
			context.activePoint = index;
			context.activeSegment = std::max(index - 1, 0);
		}

		return newPosition;
	}

	void HandleMouseWheelInput(std::vector<RoadSegment>& segments)
	{
		ImGuiIO& io = ImGui::GetIO();

		segments[context.activeSegment].r += io.MouseWheel;

		float beginToEndHalfDistance1 = glm::length(segments[context.activeSegment].end - segments[context.activeSegment].begin) / 2.0f;
		if (abs(segments[context.activeSegment].r) < beginToEndHalfDistance1)
		{
			segments[context.activeSegment].r = beginToEndHalfDistance1 * sign(segments[context.activeSegment].r);

			if (io.MouseWheel != 0.0f)
			{
				segments[context.activeSegment].r = -segments[context.activeSegment].r;
			}
		}
		if (context.activeSegment < segments.size())
		{
			float beginToEndHalfDistance2 = glm::length(segments[context.activeSegment + 1].end - segments[context.activeSegment + 1].begin) / 2.0f;
			if (abs(segments[context.activeSegment + 1].r) < beginToEndHalfDistance2)
			{
				segments[context.activeSegment + 1].r = (beginToEndHalfDistance2 * sign(segments[context.activeSegment + 1].r));
			}
		}
	}

	void Manipulate(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix, std::vector<RoadSegment>& segments, float* deltaMatrix)
	{
		ComputeContext(view, projection, matrix);

		ImGuiIO& io = ImGui::GetIO();

		for (unsigned int i = 0; i < segments.size(); ++i)
		{
			glm::vec4 position(segments[i].begin.x, segments[i].begin.y, segments[i].begin.z, 1.0f);

			ImVec2 trans;
			DrawPoint(position, trans);

			glm::vec3 newPosition = HandleInputForPoint(position, trans, i);
			segments[i].begin = newPosition;
			if (i > 0)
			{
				segments[i - 1].end = newPosition;
			}

			// last segment - draw end point
			if (i == segments.size() - 1)
			{
				glm::vec4 position(segments[i].end.x, segments[i].end.y, segments[i].end.z, 1.0f);

				ImVec2 trans;
				DrawPoint(position, trans);

				glm::vec3 newPosition = HandleInputForPoint(position, trans, i + 1);
				segments[i].end = newPosition;
			}
		}

		HandleMouseWheelInput(segments);
	}

};
