#include "RoadManipulator.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>

#include <ImGuizmo.h>

#include "../../Graphics/Roads.h"

#include "../../Utils/Helpers.hpp"


namespace RoadManipulator
{
	struct Context
	{
		RoadType roadType;

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

		std::vector<glm::vec3>* availableConnectionPoints = nullptr;
		const std::vector<glm::vec3>* curvePoints = nullptr;

		bool isAnyPointModified = false;
		int modifiedPointIndex = -1;
		glm::vec3 modifiedPointNewPosition;

		bool isCreatedNewConnection = false;
		int newConnectionIndex = -1;

		int activePoint = 0;
		int activeSegment = 0;

		bool isUsingGizmo = false;
		bool isUsingGizmoLastFrame = false;
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

	void SetAvailableConnectionPoints(std::vector<glm::vec3>* availableConnectionPoints)
	{
		context.availableConnectionPoints = availableConnectionPoints;
	}

	void SetCurvePoints(const std::vector<glm::vec3>& curvePoints)
	{
		context.curvePoints = &curvePoints;
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

	int IsCreatedNewConnection()
	{
		return context.isCreatedNewConnection;
	}

	int GetNewConnectionIndex()
	{
		return context.newConnectionIndex;
	}

	int GetActiveSegment()
	{
		return context.activeSegment;
	}

	int GetActivePoint()
	{
		return context.activePoint;
	}

	int CalculateActiveSegment(int activePoint)
	{
		if (context.roadType == RoadType::ARC)
			return std::max(activePoint - 1, 0);
		else
			return std::max((activePoint - 1) / 3, 0);
	}

	static void ComputeContext(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& matrix,
							   const std::vector<glm::vec3>& points, const std::vector<RoadSegment>& segments, RoadType roadType)
	{
		context.roadType = roadType;

		context.viewMatrix = view;
		context.projectionMatrix = projection;
		context.modelMatrix = matrix;

		context.viewProjection = context.projectionMatrix * context.viewMatrix;
		context.MVP = context.viewProjection * context.modelMatrix;

		context.isAnyPointModified = false;
		context.isCreatedNewConnection = false;

		if (context.activePoint >= points.size())
		{
			context.activePoint = points.size() - 1;
		}
		if (context.activeSegment >= segments.size())
		{
			context.activeSegment = CalculateActiveSegment(context.activePoint);
		}

		context.isUsingGizmoLastFrame = context.isUsingGizmo;
		context.isUsingGizmo = false;
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

	// return true - if the point is visible
	bool CalculatePointTransformInScreenSpace(const glm::vec4& position, ImVec2& outTrans)
	{
		glm::vec4 globalPosition = context.MVP * position;

		outTrans = transformToImGuiScreenSpace(globalPosition);

		if (globalPosition.z < 0.1f) // todo: < near and > far
		{
			return false;
		}

		if (outTrans.x <= 0.0f || outTrans.x >= context.width || outTrans.y <= 0.0f || outTrans.y >= context.height)
		{
			//return false;
		}

		return true;
	}

	void DrawPoint(const ImVec2& trans, ImU32 color = IM_COL32(255, 255, 255, 255))
	{
		ImDrawList* drawList = context.drawList;

		//drawList->AddCircleFilled(trans, 10.0f, 0xFF000000);
		//drawList->AddCircleFilled(trans, 7.0f, 0xFFFFFF00);

		//drawList->AddCircleFilled(trans, 10.0f, IM_COL32(255, 0, 0, 255));
		drawList->AddCircleFilled(trans, 6.0f, color);
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

			context.isUsingGizmo = ImGuizmo::IsUsing();
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
			context.activeSegment = CalculateActiveSegment(context.activePoint);
		}

		return newPosition;
	}

	void HandleMouseWheelInput(const std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments)
	{
		if (segments.size() == 0 || context.roadType != RoadType::ARC)
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

	void drawAvailableConnectionPoints(std::vector<ImVec2>& roadPointsTransform)
	{
		if (context.availableConnectionPoints == nullptr || roadPointsTransform.size() == 0)
		{
			return;
		}

		for (int i = 0; i < context.availableConnectionPoints->size(); ++i)
		{
			ImVec2 transform;
			glm::vec3 pos = (*context.availableConnectionPoints)[i];

			int alpha = (ImGui::IsMouseDown(0) && context.isUsingGizmoLastFrame) ? 255 : 0;
			bool pointVisibility = CalculatePointTransformInScreenSpace(glm::vec4(pos.x, pos.y, pos.z, 1.0f), transform);
			if (pointVisibility)
			{
				DrawPoint(transform, IM_COL32(255, 255, 255, alpha));
			}

			if (context.activePoint == 0 || context.activePoint == roadPointsTransform.size() - 1)
			{
				if (glm::length(glm::vec2(roadPointsTransform[context.activePoint].x, roadPointsTransform[context.activePoint].y) - glm::vec2(transform.x, transform.y)) <= 10)
				{
					if (ImGui::IsMouseDown(0))
					{
						context.drawList->AddCircleFilled(transform, 10.0f, IM_COL32(255, 0, 0, 128));
					}
					
					if (ImGui::IsMouseReleased(0) && context.isUsingGizmoLastFrame)
					{
						context.isCreatedNewConnection = true;
						context.newConnectionIndex = i;
					}
				}
			}
		}
	}

	ImVec2 fixPointOutsideCamera(const ImVec2& pointToBeFixed, const ImVec2& secondPoint, const glm::vec3& cameraPosition, const glm::vec3& pointToBeFixedWorldPosition, float windowHeight)
	{
		const float a = (secondPoint.y - pointToBeFixed.y) / (secondPoint.x - pointToBeFixed.x);
		const float b = pointToBeFixed.y - (a * pointToBeFixed.x);

		if (cameraPosition.y - pointToBeFixedWorldPosition.y > 0)
		{
			return ImVec2((windowHeight - b) / a, windowHeight);
		}
		else
		{
			return ImVec2(-b / a, 0.0f);
		}
	}

	void drawLine(int p1, int p2, const std::vector<glm::vec3>& points, const std::vector<ImVec2>& pointsTransformImGui, const std::vector<bool>& pointsVisibility, const glm::vec3& cameraPosition,
				  ImU32 color = IM_COL32(255, 255, 255, 255))
	{
		ImVec2 point1 = pointsTransformImGui[p1];
		ImVec2 point2 = pointsTransformImGui[p2];

		ImVec2 fixedPoint1 = point1;
		ImVec2 fixedPoint2 = point2;

		if (!pointsVisibility[p1] && !pointsVisibility[p2])
		{
			return;
		}
		if (!pointsVisibility[p1])
		{
			fixedPoint1 = fixPointOutsideCamera(point1, point2, cameraPosition, points[p1], context.height);
		}
		if (!pointsVisibility[p2])
		{
			fixedPoint2 = fixPointOutsideCamera(point2, point1, cameraPosition, points[p2], context.height);
		}

		ImDrawList* drawList = context.drawList;
		drawList->AddLine(fixedPoint1, fixedPoint2, color);
	}

	void drawCurve(const std::vector<glm::vec3>& points, const glm::vec3& cameraPosition)
	{
		std::vector<ImVec2> pointsTransformImGui(points.size()); // todo: ten wektor chyba trzeba przeniesc do Context
		std::vector<bool> pointsVisibility(points.size());

		for (unsigned int i = 0; i < points.size(); ++i)
		{
			glm::vec4 position(points[i].x, points[i].y, points[i].z, 1.0f);

			pointsVisibility[i] = CalculatePointTransformInScreenSpace(position, pointsTransformImGui[i]);
		}

		for (int i = 0; i < points.size() - 1; ++i)
		{
			int p1 = i;
			int p2 = i + 1;

			drawLine(p1, p2, points, pointsTransformImGui, pointsVisibility, cameraPosition, IM_COL32(255, 0, 0, 255));
		}
	}

	void Manipulate(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& matrix, const glm::vec3& cameraPosition,
					const std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, RoadType roadType, float* deltaMatrix)
	{
		ComputeContext(view, projection, matrix, points, segments, roadType);

		ImGuiIO& io = ImGui::GetIO();

		std::vector<ImVec2> pointsTransformImGui(points.size()); // todo: ten wektor chyba trzeba przeniesc do Context
		std::vector<bool> pointsVisibility(points.size());

		for (unsigned int i = 0; i < points.size(); ++i)
		{
			glm::vec4 position(points[i].x, points[i].y, points[i].z, 1.0f);

			pointsVisibility[i] = CalculatePointTransformInScreenSpace(position, pointsTransformImGui[i]);
			if (pointsVisibility[i])
			{
				DrawPoint(pointsTransformImGui[i]);
			}

			glm::vec3 newPosition = HandleInputForPoint(position, pointsTransformImGui[i], i);

			if (points[i] != newPosition)
			{
				context.isAnyPointModified = true;
				context.modifiedPointIndex = i;
				context.modifiedPointNewPosition = newPosition;
			}
		}

		// lines to control points
		if (context.roadType == RoadType::BEZIER_CURVE)
		{
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

				drawLine(p1, p2, points, pointsTransformImGui, pointsVisibility, cameraPosition);
			}
		}
		else if (context.roadType == RoadType::POLYGON)
		{
			// draw lines between two points
			for (int i = 0; i < points.size(); ++i)
			{
				int p1 = i;
				int p2 = (i + 1) % points.size();

				//drawLine(p1, p2, points, pointsTransformImGui, pointsVisibility, cameraPosition);
			}
		}

		HandleMouseWheelInput(points, segments);

		drawAvailableConnectionPoints(pointsTransformImGui);

		if (context.roadType == RoadType::BEZIER_CURVE &&
			context.curvePoints != nullptr && context.curvePoints->size() > 1)
		{
			drawCurve(*context.curvePoints, cameraPosition);
		}
	}

};
