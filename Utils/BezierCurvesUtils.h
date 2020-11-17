#ifndef BEZIERCURVESUTILS_H_INCLUDED
#define BEZIERCURVESUTILS_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>


namespace BezierCurvesUtils
{
	float calculateBezierCurvePoint(float p1, float p2, float p3, float p4, float t);

	glm::vec2 calculateBezierCurvePoint(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, float t);
	glm::vec3 calculateBezierCurvePoint(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, float t);

	void generateBezierCurvePoints(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4,
								   unsigned int numberOfPoints, std::vector<glm::vec2>& outPoints);

	void generateBezierCurvePoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4,
								   unsigned int numberOfPoints, std::vector<glm::vec3>& outPoints);
}


#endif // BEZIERCURVESUTILS_H_INCLUDED
