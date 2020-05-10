#include "RoadManipulator.h"

#include "../../ImGui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../../ImGui/imgui_internal.h"

#include "../../Graphics/Roads.h"

namespace RoadManipulator
{
	struct Context
	{
		ImDrawList* drawList;

		float x = 0.f;
		float y = 0.f;
		float width = 0.f;
		float height = 0.f;
		float xMax = 0.f;
		float yMax = 0.f;
		float displayRatio = 1.f;
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

	void Manipulate(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix, std::vector<RoadSegment>& segments, float* deltaMatrix)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* drawList = context.drawList;

		for (unsigned int i = 0; i < segments.size(); ++i)
		{
			glm::vec4 position(segments[i].begin.x, segments[i].begin.y, segments[i].begin.z, 1.0f);
			position = (projection * view) * position;

			//std::cout << "Editor: " << position.z << ", w = " << position.w << std::endl;


			//std::cout << position.z << std::endl;
			if (position.z < 0.001f)
			{
				continue;
			}

			position /= position.w;
			position *= 0.5;

			position += glm::vec4(0.5f, 0.5f, 0.0f, 0.0f);
			position.y = 1.f - position.y;
			position.x *= context.width;
			position.y *= context.height;
			position.x += context.x;
			position.y += context.y;
			ImVec2 trans(position.x, position.y);

			if (trans.x <= 0.0f || trans.x >= context.width || trans.y <= 0.0f || trans.y >= context.height)
			{
				continue;
			}

			//std::cout << trans.x << ", " << trans.y << ", " << position.z << std::endl;

			drawList->AddCircleFilled(trans, 10.0f, 0xFF000000);
			drawList->AddCircleFilled(trans, 7.0f, 0xFFFFFF00);
		}
	}

};
