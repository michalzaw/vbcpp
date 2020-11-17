#include "BezierCurvesUtils.h"


namespace BezierCurvesUtils
{
	float calculateBezierCurvePoint(float p1, float p2, float p3, float p4, float t)
	{
		return (1 - t) * (1 - t) * (1 - t) * p1 +
			   3 * (1 - t) * (1 - t) * t * p2 +
			   3 * (1 - t) * t * t * p3 +
			   t * t * t * p4;
	}

	glm::vec2 calculateBezierCurvePoint(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, float t)
	{
		return glm::vec2(
			calculateBezierCurvePoint(p1.x, p2.x, p3.x, p4.x, t),
			calculateBezierCurvePoint(p1.y, p2.y, p3.y, p4.y, t)
		);
	}

	glm::vec3 calculateBezierCurvePoint(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, float t)
	{
		return glm::vec3(
			calculateBezierCurvePoint(p1.x, p2.x, p3.x, p4.x, t),
			calculateBezierCurvePoint(p1.y, p2.y, p3.y, p4.y, t),
			calculateBezierCurvePoint(p1.z, p2.z, p3.z, p4.z, t)
		);
	}

	void generateBezierCurvePoints(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, unsigned int numberOfPoints, std::vector<glm::vec2>& outPoints)
	{
		outPoints.reserve(numberOfPoints);

		for (int i = 0; i < numberOfPoints; ++i)
		{
			float t = static_cast<float>(i) / static_cast<float>(numberOfPoints - 1);

			outPoints.push_back(calculateBezierCurvePoint(p1, p2, p3, p4, t));
		}
	}

	void generateBezierCurvePoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, unsigned int numberOfPoints, std::vector<glm::vec3>& outPoints)
	{
		outPoints.reserve(numberOfPoints);

		for (int i = 0; i < numberOfPoints; ++i)
		{
			float t = static_cast<float>(i) / static_cast<float>(numberOfPoints - 1);

			outPoints.push_back(calculateBezierCurvePoint(p1, p2, p3, p4, t));
		}
	}
}