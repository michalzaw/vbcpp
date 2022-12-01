#include "BezierCurve.h"

#include "../Utils/Helpers.hpp"
#include "../Utils/Logger.h"
#include "../Utils/BezierCurvesUtils.h"

const int DEFAULT_SEGMENT_POINTS_COUNT = 50;


BezierCurve::BezierCurve(const std::vector<glm::vec3>& points/* = {}*/, const std::vector<int>& segmentsPointsCount/* = {}*/,
						 float marginBegin/* = 0.0f*/, float marginEnd/* = 0.0f*/, const glm::vec2& offsetFromBaseCurve/* = glm::vec2(0.0f, 0.0f)*/)
	: Component(CT_BEZIER_CURVE),
	_points(points), _segmentsPointsCount(segmentsPointsCount),
	_marginBegin(marginBegin), _marginEnd(marginEnd), _offsetFromBaseCurve(offsetFromBaseCurve),
	_curvePointsIsCalculated(false)
{

}


BezierCurve::~BezierCurve()
{
	if (_onComponentDeletedListener)
	{
		_onComponentDeletedListener();
	}
}


void BezierCurve::calculateCurvePoints()
{
	if (_points.size() > 0 && (_points.size() - 1) % 3 != 0)
	{
		LOG_ERROR("Invalid number of control points");
		return;
	}
	if (_points.size() / 3 != _segmentsPointsCount.size())
	{
		LOG_ERROR("Invalid number of segments");
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

	if (_onPointAddedListener)
	{
		_onPointAddedListener();
	}
}


void BezierCurve::deletePoint(int index)
{
	if (index >= _points.size())
	{
		LOG_ERROR("Invalid index: " + Strings::toString(index));
		return;
	}

	if (index % 3 == 1 || index % 3 == 2)
	{
		LOG_ERROR("Invalid index: " + Strings::toString(index));
		return;
	}

	if (index == 0 && _points.size() == 1)
	{
		_points.erase(_points.begin());
	}
	else if (index == 0)
	{
		_points.erase(_points.begin());
		_points.erase(_points.begin());
		_points.erase(_points.begin());
	}
	else if (index == _points.size() - 1)
	{
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

	if (_onPointDeletedListener)
	{
		_onPointDeletedListener(index);
	}
}


void BezierCurve::setPointPostion(int index, const glm::vec3& newPosition)
{
	if (index >= _points.size())
	{
		LOG_ERROR("Invalid index: " + Strings::toString(index));
		return;
	}

	if (isVectorContains(_pointsWithLockedPosition, index))
	{
		return;
	}

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

	_points[index] = newPosition;

	_curvePointsIsCalculated = false;

	if (_onPointChangedPositionListener)
	{
		_onPointChangedPositionListener(index, newPosition);
	}
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


int BezierCurve::getSegmentPointsCount(int segmentIndex)
{
	if (segmentIndex >= _segmentsPointsCount.size())
	{
		LOG_ERROR("Invalid segmentIndex: " + Strings::toString(segmentIndex));
		return 0;
	}

	return _segmentsPointsCount[segmentIndex];
}


int BezierCurve::getSegmentsCount()
{
	return _segmentsPointsCount.size();
}


void BezierCurve::setSegmentPointsCount(int segmentIndex, int pointsCount)
{
	if (segmentIndex >= _segmentsPointsCount.size())
	{
		LOG_ERROR("Invalid segmentIndex: " + Strings::toString(segmentIndex));
		return;
	}

	_segmentsPointsCount[segmentIndex] = pointsCount;

	_curvePointsIsCalculated = false;

	if (_onCurveChangedListener)
	{
		_onCurveChangedListener();
	}
}


void BezierCurve::setMarginBegin(float marginBegin) 
{
	_marginBegin = marginBegin;
	_curvePointsIsCalculated = false;

	if (_onCurveChangedListener)
	{
		_onCurveChangedListener();
	}
}


void BezierCurve::setMarginEnd(float marginEnd)
{
	_marginEnd = marginEnd;
	_curvePointsIsCalculated = false;

	if (_onCurveChangedListener)
	{
		_onCurveChangedListener();
	}
}


void BezierCurve::setOffsetFromBaseCurve(const glm::vec2& offsetFromBaseCurve)
{
	_offsetFromBaseCurve = offsetFromBaseCurve;
	_curvePointsIsCalculated = false;

	if (_onCurveChangedListener)
	{
		_onCurveChangedListener();
	}
}


void BezierCurve::lockPointPosition(int index)
{
	auto iterator = std::find(_pointsWithLockedPosition.begin(), _pointsWithLockedPosition.end(), index);
	if (iterator == _pointsWithLockedPosition.end())
	{
		_pointsWithLockedPosition.push_back(index);
	}
}


void BezierCurve::unlockPointPosition(int index)
{
	auto iterator = std::find(_pointsWithLockedPosition.begin(), _pointsWithLockedPosition.end(), index);
	if (iterator != _pointsWithLockedPosition.end())
	{
		_pointsWithLockedPosition.erase(iterator);
	}
}


void BezierCurve::setOnPointAddedListener(const std::function<void()>& onPointAddedListener)
{
	_onPointAddedListener = onPointAddedListener;
}


void BezierCurve::setOnPointDeletedListener(const std::function<void(int)>& onPointDeletedListener)
{
	_onPointDeletedListener = onPointDeletedListener;
}


void BezierCurve::setOnPointChangedPositionListener(const std::function<void(int, const glm::vec3&)>& onPointChangedPositionListener)
{
	_onPointChangedPositionListener = onPointChangedPositionListener;
}


void BezierCurve::setOnCurveChangedListener(const std::function<void()>& onCurveChangedListener)
{
	_onCurveChangedListener = onCurveChangedListener;
}


void BezierCurve::setOnComponentDeletedListener(const std::function<void()>& onComponentDeletedListener)
{
	_onComponentDeletedListener = onComponentDeletedListener;
}
