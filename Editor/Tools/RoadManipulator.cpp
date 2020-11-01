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
#include "../../Utils/Collision.h"


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
		int modifiedPointIndex = -1;
		glm::vec3 modifiedPointNewPosition;

		int activePoint = 0;
		int activeSegment = 0;
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

	int GetModifiedPointIndex()
	{
		return context.modifiedPointIndex;
	}

	glm::vec3 GetModifiedPointNewPostion()
	{
		return context.modifiedPointNewPosition;
	}

	int GetActiveSegment()
	{
		return context.activeSegment;
	}

	int GetActivePoint()
	{
		return context.activePoint;
	}

	static void ComputeContext(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments)
	{
		context.viewMatrix = view;
		context.projectionMatrix = projection;
		context.modelMatrix = matrix;

		context.viewProjection = context.projectionMatrix * context.viewMatrix;
		context.MVP = context.viewProjection * context.modelMatrix;

		context.isAnyPointModified = false;

		if (context.activePoint >= points.size())
		{
			context.activePoint = points.size() - 1;
		}
		if (context.activeSegment >= segments.size())
		{
			context.activeSegment = segments.size() - 1;
		}
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

	void HandleMouseWheelInput(std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments)
	{
		if (segments.size() == 0)
			return;

		ImGuiIO& io = ImGui::GetIO();

		segments[context.activeSegment].r += io.MouseWheel;

		glm::vec3 segmentBegin = points[context.activeSegment];
		glm::vec3 segmentEnd = points[context.activeSegment + 1];

		float beginToEndHalfDistance1 = glm::length(segmentEnd - segmentBegin) / 2.0f;
		if (abs(segments[context.activeSegment].r) < beginToEndHalfDistance1)
		{
			segments[context.activeSegment].r = beginToEndHalfDistance1 * sign(segments[context.activeSegment].r);

			if (io.MouseWheel != 0.0f)
			{
				segments[context.activeSegment].r = -segments[context.activeSegment].r;
			}
		}
		if (context.activeSegment < segments.size() - 1)
		{
			segmentBegin = points[context.activeSegment + 1];
			segmentEnd = points[context.activeSegment + 2];

			float beginToEndHalfDistance2 = glm::length(segmentEnd  - segmentBegin) / 2.0f;
			if (abs(segments[context.activeSegment + 1].r) < beginToEndHalfDistance2)
			{
				int a = 1;
				a = a + 1;

				segments[context.activeSegment + 1].r = (beginToEndHalfDistance2 * sign(segments[context.activeSegment + 1].r));
			}
		}

		if (io.MouseWheel != 0.0f)
		{
			context.isAnyPointModified = true;
		}
	}

	void Manipulate(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, float* deltaMatrix)
	{
		ComputeContext(view, projection, matrix, points, segments);

		ImGuiIO& io = ImGui::GetIO();

		std::vector<ImVec2> pointsTransformImGui(points.size());
		std::vector<bool> pointsVisibility(points.size());

		for (unsigned int i = 0; i < points.size(); ++i)
		{
			glm::vec4 position(points[i].x, points[i].y, points[i].z, 1.0f);

			pointsVisibility[i] = DrawPoint(position, pointsTransformImGui[i]);

			glm::vec3 newPosition = HandleInputForPoint(position, pointsTransformImGui[i], i);

			if (points[i] != newPosition)
			{
				context.isAnyPointModified = true;
				context.modifiedPointIndex = i;
				context.modifiedPointNewPosition = newPosition;
			}
		}

		for (unsigned int i = 0; i < points.size(); ++i)
		{
			int p1 = i;
			int p2 = i;
			if (i % 3 == 1)
			{
				p2 = i - 1;
			}
			else if (i % 3 == 2)
			{
				p2 = i + 1;
			}
			else
			{
				continue;
			}

			bool pointInvisible = false;
			if (!pointsVisibility[p1] && !pointsVisibility[p2])
			{
				continue;
			}
			else if (!pointsVisibility[p1])
			{
				pointInvisible = true;
			}
			else if (!pointsVisibility[p2])
			{
				pointInvisible = true;
				std::swap(p1, p2);
			}

			ImVec2 point1 = pointsTransformImGui[p1];
			ImVec2 point2 = pointsTransformImGui[p2];
			/*if (pointInvisible)
			{
				//std::cout << "p1: " << points[p1].x << ", " << points[p1].y << ", " << points[p1].z << std::endl;
				//std::cout << "p2: " << points[p2].x << ", " << points[p2].y << ", " << points[p2].z << std::endl;

				glm::vec3 rayDirection = glm::normalize(points[p2] - points[p1]);
				Plane plane;
				plane.set(0, 0, 1, -0.001f);

				float t;
				float vd;
				if (collisionRayToPlane(points[p1], rayDirection, plane, t, vd))
				{
					glm::vec3 newPoint1 = points[p1] + t * rayDirection;

					glm::vec4 newPoint1Vec4 = context.MVP * glm::vec4(newPoint1.x, newPoint1.y, newPoint1.z, 1.0f);

					std::cout << "p1: " << points[p1].x << ", " << points[p1].y << ", " << points[p1].z << std::endl;
					std::cout << t << "     " << newPoint1Vec4.x << ", " << newPoint1Vec4.y << ", " << newPoint1Vec4.z << std::endl;

					point1 = transformToImGuiScreenSpace(newPoint1Vec4);
				}
			}*/

			if (!pointInvisible)
			{
				ImDrawList* drawList = context.drawList;
				drawList->AddLine(point1, point2, 0xFFFFFFFF);
			}
		}

		HandleMouseWheelInput(points, segments);
	}

};
