#ifndef GEOMETRYUTILS_H_INCLUDED
#define GEOMETRYUTILS_H_INCLUDED


#include <glm/glm.hpp>


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
}


#endif // GEOMETRYUTILS_H_INCLUDED
