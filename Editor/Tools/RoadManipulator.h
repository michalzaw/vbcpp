#ifndef ROADMANIPULATOR_H_INCLUDED
#define ROADMANIPULATOR_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>


struct RoadSegment;


// todo: zmienic nazwe - juz nie tylko road
namespace RoadManipulator
{
	enum class RoadType
	{
		ARC,
		BEZIER_CURVE,
		POLYGON
	};

	// call BeginFrame right after ImGui_XXXX_NewFrame();
	void BeginFrame();

	void SetRect(float x, float y, float width, float height);
	void SetAvailableConnectionPoints(std::vector<glm::vec3>* availableConnectionPoints);

	void SetCurvePoints(const std::vector<glm::vec3>& curvePoints);

	int IsModified();
	int GetModifiedPointIndex();
	glm::vec3 GetModifiedPointNewPostion();
	int IsCreatedNewConnection();
	int GetNewConnectionIndex();

	int GetActiveSegment();
	int GetActivePoint();

	void Manipulate(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& matrix, const glm::vec3& cameraPosition,
					const std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, RoadType roadType, float* deltaMatrix = 0);
};


#endif // ROADMANIPULATOR_H_INCLUDED