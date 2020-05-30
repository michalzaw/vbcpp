#ifndef ROADMANIPULATOR_H_INCLUDED
#define ROADMANIPULATOR_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>


struct RoadSegment;


namespace RoadManipulator
{
	// call BeginFrame right after ImGui_XXXX_NewFrame();
	void BeginFrame();

	void SetRect(float x, float y, float width, float height);

	int IsModified();
	int GetActiveSegment();
	int GetActivePoint();

	void Manipulate(glm::mat4 view, glm::mat4 projection, glm::mat4 matrix, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, float* deltaMatrix = 0);
};


#endif // ROADMANIPULATOR_H_INCLUDED