#pragma once

#include <vector>

#include "../../ImGui/imgui.h"

#include "../../Scene/SceneManager.h"

namespace ImGui
{
	void RoadProfileGraph(const char* label, std::vector<ImVec2>& values, float scale_min, float scale_max, ImVec2 frame_size);

	void DrawRoadShape(std::vector<RoadSegment>& segments);
}