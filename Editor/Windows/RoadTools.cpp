#include "RoadTools.h"

#include "../../ImGui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../../ImGui/imgui_internal.h"

namespace vbEditor
{
	extern SceneObject* _selectedSceneObject;
	extern CameraFPS* _camera;
}

void ImGui::RoadProfileGraph(const char* label, std::vector<ImVec2>& values, float scale_min, float scale_max, ImVec2 frame_size)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	if (frame_size.x == 0.0f)
		frame_size.x = CalcItemWidth();
	if (frame_size.y == 0.0f)
		frame_size.y = label_size.y + (style.FramePadding.y * 2);

	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
	const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, 0, &frame_bb))
		return;
	const bool hovered = ItemHoverable(frame_bb, id);

	ImGui::Text("frame_bb.Min.x: %f, frame_bb.Min.y: %f", frame_bb.Min.x, frame_bb.Min.y);
	ImGui::Text("frame_bb.Max.x: %f, frame_bb.Max.y: %f", frame_bb.Max.x, frame_bb.Max.y);
	
	// Determine scale from values if not specified
	if (scale_min == FLT_MAX || scale_max == FLT_MAX)
	{
		float v_min = FLT_MAX;
		float v_max = -FLT_MAX;
		for (int i = 0; i < values.size(); i++)
		{
			const float v = values[i].y;
			if (v != v) // Ignore NaN values
				continue;
			v_min = ImMin(v_min, v);
			v_max = ImMax(v_max, v);
		}
		if (scale_min == FLT_MAX)
			scale_min = v_min;
		if (scale_max == FLT_MAX)
			scale_max = v_max;
	}
	
	ImGui::Text("scale_min: %f, scale_max: %f", scale_min, scale_max);

	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);

	float range = abs(scale_max) + abs(scale_min);
	ImGui::Text("Range: %f", range);

	/*
	OldRange = (OldMax - OldMin)  
	NewRange = (NewMax - NewMin)  
	NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin*/

	for (int i = 0; i < values.size(); i++)
	{
		ImVec2 p = values[i];

		//const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));
		//ImVec2 pn = ImVec2(0.0f, 1.0f - ImSaturate((p.y - scale_min) * inv_scale));
		
		ImVec2 new_p = ImVec2((((p.x - scale_min) * (frame_bb.Max.x - frame_bb.Min.x) / range) + frame_bb.Min.x),
			((((p.y * -1.0f) - scale_min) * (frame_bb.Max.y - frame_bb.Min.y) / range) + frame_bb.Min.y) );
		
		window->DrawList->AddCircleFilled(new_p, 4.0f, color);
	}

	
	//static float dotSize = 1.0f;

	//ImGui::DragFloat("Dot Size", &dotSize, 0.01f, 0.0f, 6.0f);

	//window->DrawList->AddCircleFilled(tp0, 4.0f, color);
	//window->DrawList->AddCircleFilled(tp1, 4.0f, color);
	//window->DrawList->AddCircleFilled(pos0, 3.0f, ImGuiCol_PlotLines);

	//window->DrawList->AddLine(tp0, tp1, color);

}

void ImGui::DrawRoadShape(std::vector<RoadSegment>& segments)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;



}
