#include "GeometryUtils.h"


namespace GeometryUtils
{
	// oblicza odleglosc punktu p od prostej wyznaczonej przez punty pointA i pointB
	float calculatePointDistanceToStraightLine(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& p)
	{
		glm::vec3 directionVector = glm::normalize(pointB - pointA);
		glm::vec3 v = p - pointA;

		float distanceFromAToProjectionP = glm::dot(directionVector, v);

		glm::vec3 projectionPointP = pointA + directionVector * distanceFromAToProjectionP;

		float distance = glm::distance(p, projectionPointP);

		return distance;
	}

	void mapToVec2(const std::vector<glm::vec3>& polygonPoints, std::vector<glm::vec2>& outPolygonPointsVec2)
	{
		outPolygonPointsVec2.clear();

		for (const glm::vec3& point : polygonPoints)
		{
			outPolygonPointsVec2.emplace_back(point.x, point.z);
		}
	}

	void findMinAndMaxCoord(const std::vector<glm::vec2>& polygonPoints, glm::vec2& outMinCoords, glm::vec2& outMaxCoords)
	{
		outMinCoords = glm::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		outMaxCoords = glm::vec2(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

		for (const glm::vec2& point : polygonPoints)
		{
			if (point.x < outMinCoords.x)
			{
				outMinCoords.x = point.x;
			}
			if (point.x > outMaxCoords.x)
			{
				outMaxCoords.x = point.x;
			}

			if (point.y < outMinCoords.y)
			{
				outMinCoords.y = point.y;
			}
			if (point.y > outMaxCoords.y)
			{
				outMaxCoords.y = point.y;
			}
		}
	}

	bool isPointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygonPoints) {
		bool inside = false;
		size_t n = polygonPoints.size();

		for (size_t i = 0, j = n - 1; i < n; j = i++)
		{
			const glm::vec2& p1 = polygonPoints[i];
			const glm::vec2& p2 = polygonPoints[j];

			if (((p1.y > point.y) != (p2.y > point.y)) &&
				(point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x))
			{
				inside = !inside;
			}
		}

		return inside;
	}
}
