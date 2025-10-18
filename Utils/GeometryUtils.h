#ifndef GEOMETRYUTILS_H_INCLUDED
#define GEOMETRYUTILS_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>


namespace GeometryUtils
{
	// oblicza odleglosc punktu p od prostej wyznaczonej przez punty pointA i pointB
	float calculatePointDistanceToStraightLine(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& p);

	void mapToVec2(const std::vector<glm::vec3>& polygonPoints, std::vector<glm::vec2>& outPolygonPointsVec2);

	void findMinAndMaxCoord(const std::vector<glm::vec2>& polygonPoints, glm::vec2& outMinCoords, glm::vec2& outMaxCoords);

	bool isPointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygonPoints);
}


#endif // GEOMETRYUTILS_H_INCLUDED
