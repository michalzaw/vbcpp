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

	void generateBezierCurvePointsWithConstDistance(int resolution, float distance, std::vector<glm::vec3>& outPoints, const std::function<glm::vec3(float)>& function)
	{
		outPoints.clear();

		float distanceFromLastPoint = distance;
		glm::vec3 lastPoint;

		const float deltaPosition = 1.0f / static_cast<float>(resolution);

		for (int i = 0; i < resolution; ++i)
		{
			const float t = i * deltaPosition;
			const glm::vec3 currentPoint = function(t);

			if (i != 0)
			{
				distanceFromLastPoint += glm::distance(currentPoint, lastPoint);
			}

			while (distanceFromLastPoint >= distance)
			{
				distanceFromLastPoint -= distance;
				outPoints.push_back(currentPoint);
			}

			lastPoint = currentPoint;
		}
	}

	float getPointByDistanceFromCurveBegin(int resolution, float expectedDistanceFromBegin, float& outDistanceToReturnedPoint, const std::function<glm::vec3(float)>& function)
	{
		float distance = 0.0f;
		glm::vec3 lastPoint = function(0.0f);

		const float deltaPosition = 1.0f / static_cast<float>(resolution);

		for (int i = 1; i < resolution; ++i)
		{
			const float t = i * deltaPosition;
			const glm::vec3 currentPoint = function(t);

			distance += glm::distance(currentPoint, lastPoint);

			lastPoint = currentPoint;

			if (distance >= expectedDistanceFromBegin)
			{
				return t;
			}
		}

		outDistanceToReturnedPoint = distance;
		return -1.0f;
	}

	float getPointByDistanceFromCurveEnd(int resolution, float expectedDistanceFromEnd, float& outDistanceToReturnedPoint, const std::function<glm::vec3(float)>& function)
	{
		float distance = 0.0f;
		glm::vec3 lastPoint = function(1.0f);

		const float deltaPosition = 1.0f / static_cast<float>(resolution);

		for (int i = resolution - 1; i >= 0; --i)
		{
			const float t = i * deltaPosition;
			const glm::vec3 currentPoint = function(t);

			distance += glm::distance(currentPoint, lastPoint);

			lastPoint = currentPoint;

			if (distance >= expectedDistanceFromEnd)
			{
				return t;
			}
		}

		outDistanceToReturnedPoint = distance;
		return -1.0f;
	}

	void cutBezierCurveFromBegin(std::vector<glm::vec3>& curvePoints, float distance)
	{
		float length = 0.0f;
		int i;
		for (i = 0; i < curvePoints.size() - 2; ++i)
		{
			length += glm::distance(curvePoints[i], curvePoints[i + 1]);
			if (length >= distance)
			{
				break;
			}
		}

		curvePoints.erase(curvePoints.begin(), curvePoints.begin() + i);
	}

	void cutBezierCurveFromEnd(std::vector<glm::vec3>& curvePoints, float distance)
	{
		float length = 0.0f;
		int i;
		for (i = 0; i < curvePoints.size() - 2; ++i)
		{
			length += glm::distance(curvePoints[curvePoints.size() - i - 1], curvePoints[curvePoints.size() - i - 2]);
			if (length >= distance)
			{
				break;
			}
		}

		curvePoints.erase(curvePoints.end() - i, curvePoints.end());
	}

	void splitBezierCurve(const std::vector<glm::vec3>& curveControlPoints, float t, std::vector<glm::vec3>& outNewCurve1ControlPoints, std::vector<glm::vec3>& outNewCurve2ControlPoints)
	{
		glm::vec3 splitPointPosition = calculateBezierCurvePoint(curveControlPoints[0], curveControlPoints[1], curveControlPoints[2], curveControlPoints[3], t);

		glm::vec3 p1 = glm::mix(curveControlPoints[0], curveControlPoints[1], t);
		glm::vec3 p2 = glm::mix(curveControlPoints[1], curveControlPoints[2], t);
		glm::vec3 p3 = glm::mix(curveControlPoints[2], curveControlPoints[3], t);

		glm::vec3 p4 = glm::mix(p1, p2, t);
		glm::vec3 p5 = glm::mix(p2, p3, t);

		outNewCurve1ControlPoints.push_back(curveControlPoints[0]);
		outNewCurve1ControlPoints.push_back(p1);
		outNewCurve1ControlPoints.push_back(p4);
		outNewCurve1ControlPoints.push_back(splitPointPosition);

		outNewCurve2ControlPoints.push_back(splitPointPosition);
		outNewCurve2ControlPoints.push_back(p5);
		outNewCurve2ControlPoints.push_back(p3);
		outNewCurve2ControlPoints.push_back(curveControlPoints[3]);
	}
}