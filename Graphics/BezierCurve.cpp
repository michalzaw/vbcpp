#include "BezierCurve.h"

#include "../Utils/Logger.h"
#include "../Utils/BezierCurvesUtils.h"

const int DEFAULT_SEGMENT_POINTS_COUNT = 50;


BezierCurve::BezierCurve()
	: Component(CT_BEZIER_CURVE),
	_marginBegin(0.0f), _marginEnd(0.0f),
	_curvePointsIsCalculated(false)
{

}


void BezierCurve::calculateCurvePoints()
{
	if (_points.size() > 0 && (_points.size() - 1) % 3 != 0)
	{
		LOG_ERROR("RoadObject: Invalid number of control points");
		return;
	}
	if (_points.size() / 3 != _segmentsPointsCount.size())
	{
		LOG_ERROR("RoadObject: Invalid number of segments");
		return;
	}

	_curvePoints.clear();

	if (_points.size() > 0)
	{
		for (int i = 0; i < _points.size() - 1; i += 3)
		{
			int segmentIndex = i / 3;
			std::vector<glm::vec3> segmentPoints;
			BezierCurvesUtils::generateBezierCurvePoints(_points[i], _points[i + 1], _points[i + 2], _points[i + 3], _segmentsPointsCount[segmentIndex], segmentPoints);
			/*BezierCurvesUtils::generateBezierCurvePointsWithConstDistance(10000, 4.0f, segmentPoints, [this, i](float t)
				{
					return BezierCurvesUtils::calculateBezierCurvePoint(_points[i], _points[i + 1], _points[i + 2], _points[i + 3], t);
				});*/

			if (i == 0)
			{
				_curvePoints.insert(_curvePoints.end(), segmentPoints.begin(), segmentPoints.end());
			}
			else
			{
				_curvePoints.insert(_curvePoints.end(), segmentPoints.begin() + 1, segmentPoints.end());
			}
		}
	}

	cutCurvePointsToMargin(_curvePoints);
}


void BezierCurve::cutCurvePointsToMargin(std::vector<glm::vec3>& curvePoints)
{
	if (_marginBegin > 0.0f)
	{
		BezierCurvesUtils::cutBezierCurveFromBegin(curvePoints, _marginBegin);
	}

	if (_marginEnd > 0.0f)
	{
		BezierCurvesUtils::cutBezierCurveFromEnd(curvePoints, _marginEnd);
	}
}


void BezierCurve::addPoint(const glm::vec3& position)
{
	if (_points.size() == 0)
	{
		_points.push_back(position);
	}
	else if (_points.size() == 1)
	{
		glm::vec3 controlPointPosition = (_points[_points.size() - 1] + position) * 0.5f;
		_points.push_back(controlPointPosition);
		_points.push_back(controlPointPosition);
		_points.push_back(position);

		_segmentsPointsCount.push_back(DEFAULT_SEGMENT_POINTS_COUNT);
	}
	else if (_points.size() > 1)
	{
		int pointsCount = _points.size();

		float distanceLastCurvePointToNewPoint = glm::length(glm::vec3(position.x, 0.0f, position.z) - glm::vec3(_points[pointsCount - 1].x, 0.0f, _points[pointsCount - 1].z));
		float distanceLastCurvePointToLastControlPoint = glm::length(glm::vec3(_points[pointsCount - 1].x, 0.0f, _points[pointsCount - 1].z) - glm::vec3(_points[pointsCount - 2].x, 0.0f, _points[pointsCount - 2].z));

		glm::vec3 a = glm::normalize(_points[pointsCount - 4] - _points[pointsCount - 1]);
		glm::vec3 b = glm::normalize(position - _points[pointsCount - 1]);
		a.y = 0;
		b.y = 0;

		glm::vec3 angleBisector = glm::normalize(a + b);

		glm::vec3 controlPointsDirection(-angleBisector.z, 0.0f, angleBisector.x);

		if (glm::dot(controlPointsDirection, a) < 0.0f)
		{
			controlPointsDirection = -controlPointsDirection;
		}

		// modify last control point
		_points[pointsCount - 2] = _points[pointsCount - 1] + controlPointsDirection * distanceLastCurvePointToLastControlPoint;


		// first control point
		_points.push_back(_points[pointsCount - 1] - controlPointsDirection * distanceLastCurvePointToNewPoint * 0.5f);

		// second control point
		_points.push_back(position - b * distanceLastCurvePointToNewPoint * 0.5f);

		_points.push_back(position);

		_segmentsPointsCount.push_back(DEFAULT_SEGMENT_POINTS_COUNT);
	}

	_curvePointsIsCalculated = false;
}


void BezierCurve::deletePoint(int index)
{
	if (index >= _points.size())
	{
		return;
	}

	if (index % 3 == 1 || index % 3 == 2)
	{
		return;
	}

	if (index == 0 && _points.size() == 1)
	{
		//setConnectionPoint(0, nullptr);

		_points.erase(_points.begin());
	}
	else if (index == 0)
	{
		//setConnectionPoint(0, nullptr);

		_points.erase(_points.begin());
		_points.erase(_points.begin());
		_points.erase(_points.begin());
	}
	else if (index == _points.size() - 1)
	{
		//setConnectionPoint(1, nullptr);

		_points.erase(_points.end() - 1);
		_points.erase(_points.end() - 1);
		_points.erase(_points.end() - 1);
	}
	else
	{
		auto nextPointIterator = _points.erase(_points.begin() + index);
		// next control point
		if (nextPointIterator != _points.end())
		{
			nextPointIterator = _points.erase(nextPointIterator);
		}
		// previous control point
		if (nextPointIterator - 1 != _points.end())
		{
			nextPointIterator = _points.erase(nextPointIterator - 1);
		}
	}

	unsigned int segmentIndex = std::max(((int)index - 1) / 3, 0);

	if (segmentIndex < _segmentsPointsCount.size())
	{
		_segmentsPointsCount.erase(_segmentsPointsCount.begin() + segmentIndex);
	}

	_curvePointsIsCalculated = false;
}


void BezierCurve::setPointPostion(int index, const glm::vec3& newPosition)
{
	if (index >= _points.size())
	{
		return;
	}

	/*if (index == 0 && isConnectionExist(0) ||
		index == _points.size() - 1 && isConnectionExist(1))
	{
		return;
	}*/

	if (index % 3 == 0)	// point on curve
	{
		glm::vec3 deltaPosition = newPosition - _points[index];

		if (index - 1 >= 0)
		{
			_points[index - 1] += deltaPosition;
		}
		if (index + 1 < _points.size())
		{
			_points[index + 1] += deltaPosition;
		}
	}
	else if (index % 3 == 1) // control point
	{
		if (index - 2 >= 0)
		{
			glm::vec3 direction = glm::normalize(_points[index] - _points[index - 1]);

			float length = glm::length(_points[index - 2] - _points[index - 1]);

			_points[index - 2] = _points[index - 1] - direction * length;
		}
	}
	else if (index % 3 == 2)
	{
		if (index + 2 < _points.size())
		{
			glm::vec3 direction = glm::normalize(_points[index] - _points[index + 1]);

			float length = glm::length(_points[index + 2] - _points[index + 1]);

			_points[index + 2] = _points[index + 1] - direction * length;
		}
	}

	//rebuild road intersections
	/*if (_connectionPoints[0].roadIntersectionComponent != nullptr && index < 5)
	{
		_connectionPoints[0].roadIntersectionComponent->needRebuild();
	}
	if (_connectionPoints[1].roadIntersectionComponent != nullptr && index > static_cast<int>(_points.size()) - 6)
	{
		_connectionPoints[1].roadIntersectionComponent->needRebuild();
	}*/

	_points[index] = newPosition;

	_curvePointsIsCalculated = false;
}


const std::vector<glm::vec3>& BezierCurve::getCurvePoints()
{
	if (!_curvePointsIsCalculated)
	{
		calculateCurvePoints();

		_curvePointsIsCalculated = true;
	}

	return _curvePoints;
}

void BezierCurve::setMarginBegin(float marginBegin) 
{
	_marginBegin = marginBegin;
	_curvePointsIsCalculated = false;
}


void BezierCurve::setMarginEnd(float marginEnd)
{
	_marginEnd = marginEnd;
	_curvePointsIsCalculated = false;
}
