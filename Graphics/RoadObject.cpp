#include "RoadObject.h"

#include <algorithm>

#include "CrossroadComponent.h"
#include "RoadGenerator.h"

#include "../Scene/SceneObject.h"

#include "../Utils/BezierCurvesUtils.h"
#include "../Utils/Logger.h"


RoadObject::RoadObject(RoadType roadType, RRoadProfile* _roadProfile, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, bool buildModelAfterCreate)
	: _roadType(roadType), _roadProfile(_roadProfile), _points(points), _segments(segments), _connectionPoints(2)
{
	_type = CT_ROAD_OBJECT;
	
	if (buildModelAfterCreate)
		buildModel();
}


RoadObject::RoadObject(RRoadProfile* _roadProfile, std::vector<glm::vec3>& points, bool buildModelAfterCreate)
	: _roadType(RoadType::BEZIER_CURVES), _roadProfile(_roadProfile), _points(points), _connectionPoints(2)
{
	_type = CT_ROAD_OBJECT;

	if (buildModelAfterCreate)
		buildModel();
}


RoadObject::~RoadObject()
{
	LOG_INFO("RoadObject destructor");
	if (_modelsDatas.size() > 0)
	{
		delete _modelsDatas[0].model;
	}
}


void RoadObject::buildModel(bool reuseExistingModel)
{
	
	std::vector<RoadConnectionPointData*> connectionPointsData;
	for (int i = 0; i < 2; ++i)
	{
		if (_connectionPoints[i].crossroadComponent != nullptr)
		{
			RoadConnectionPointData* data = new RoadConnectionPointData;

			glm::vec3 pos = _connectionPoints[i].crossroadComponent->getConnectionPoint(_connectionPoints[i].index).position;
			data->position = _connectionPoints[i].crossroadComponent->getSceneObject()->transformLocalPointToGlobal(pos);

			glm::vec3 dir = _connectionPoints[i].crossroadComponent->getConnectionPoint(_connectionPoints[i].index).direction;
			data->direction = _connectionPoints[i].crossroadComponent->getSceneObject()->transformLocalVectorToGlobal(dir);

			connectionPointsData.push_back(data);
		}
		else
		{
			connectionPointsData.push_back(nullptr);
		}
	}

	if (_modelsDatas[0].modelRootNode != nullptr)
	{
		delete _modelsDatas[0].modelRootNode;
	}

	if (!reuseExistingModel && _modelsDatas[0].model != nullptr)
	{
		delete _modelsDatas[0].model;
		_modelsDatas[0].model = nullptr;
	}

	if (_roadType == RoadType::LINES_AND_ARC)
	{
		buildModelLinesAndArcMode(connectionPointsData, reuseExistingModel);
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
	{
		buildModelBezierCurvesMode(connectionPointsData, reuseExistingModel);
	}

	for (int i = 0; i < 2; ++i)
	{
		if (connectionPointsData[i] != nullptr)
			delete connectionPointsData[i];
	}
}


void RoadObject::buildModelLinesAndArcMode(std::vector<RoadConnectionPointData*>& connectionPointsData, bool reuseExistingModel)
{
	setModel(createRoadModel(_roadProfile->getRoadLanes(), _points, _segments, connectionPointsData, _modelsDatas[0].model));
}


void RoadObject::buildModelBezierCurvesMode(std::vector<RoadConnectionPointData*>& connectionPointsData, bool reuseExistingModel)
{
	if (_points.size() > 0 && (_points.size() - 1) % 3 != 0)
	{
		LOG_ERROR("RoadObject: Invalid number of control points");
		return;
	}
	if (_points.size() / 3 != _segments.size())
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
			BezierCurvesUtils::generateBezierCurvePoints(_points[i], _points[i + 1], _points[i + 2], _points[i + 3], _segments[segmentIndex].pointsCount, segmentPoints);
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

	setModel(RoadGenerator::createRoadModel(_roadProfile->getRoadLanes(), _curvePoints, connectionPointsData, _modelsDatas[0].model));
}


RoadType RoadObject::getRoadType()
{
	return _roadType;
}


RRoadProfile* RoadObject::getRoadProfile()
{
	return _roadProfile;
}


std::vector<glm::vec3>& RoadObject::getPoints()
{
	return _points;
}


std::vector<RoadSegment>& RoadObject::getSegments()
{
	return _segments;
}


void RoadObject::setRoadProfile(RRoadProfile* roadProfile)
{
	_roadProfile = roadProfile;
}


void RoadObject::setSegments(std::vector<RoadSegment> segments)
{
	_segments = segments;
}


void RoadObject::addPoint(glm::vec3 position)
{
	if (_roadType == RoadType::LINES_AND_ARC)
	{
		_points.push_back(position);

		if (_points.size() > 1)
		{
			_segments.push_back(RoadSegment());
		}
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
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

			RoadSegment segment;
			segment.type = RST_BEZIER_CURVE;
			_segments.push_back(segment);
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

			RoadSegment segment;
			segment.type = RST_BEZIER_CURVE;
			_segments.push_back(segment);
		}
	}
}


void RoadObject::deletePoint(unsigned int index)
{
	if (index >= _points.size())
	{
		return;
	}

	if (_roadType == RoadType::LINES_AND_ARC)
	{
		_points.erase(_points.begin() + index);

		unsigned int segmentIndex = std::max((int)index - 1, 0);

		if (segmentIndex < _segments.size())
		{
			_segments.erase(_segments.begin() + segmentIndex);
		}
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
	{
		if (index % 3 == 1 || index % 3 == 2)
		{
			return;
		}

		if (index == 0 && _points.size() == 1)
		{
			setConnectionPoint(0, nullptr);

			_points.erase(_points.begin());
		}
		else if (index == 0)
		{
			setConnectionPoint(0, nullptr);

			_points.erase(_points.begin());
			_points.erase(_points.begin());
			_points.erase(_points.begin());
		}
		else if (index == _points.size() - 1)
		{
			setConnectionPoint(1, nullptr);

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

		if (segmentIndex < _segments.size())
		{
			_segments.erase(_segments.begin() + segmentIndex);
		}
	}
}


void RoadObject::setPointPostion(int index, glm::vec3 newPosition)
{
	if (index >= _points.size())
	{
		return;
	}

	if (_roadType == RoadType::BEZIER_CURVES)
	{
		if (index == 0 && _connectionPoints[0].crossroadComponent != nullptr ||
			index == _points.size() - 1 && _connectionPoints[1].crossroadComponent != nullptr)
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
	}

	_points[index] = newPosition;
}


void RoadObject::setConnectionPoint(int index, CrossroadComponent* crossroadComponent, int indexInCrossroad)
{
	if (index >= 2)
		return;

	if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		_connectionPoints[index].crossroadComponent->disconnectRoad(_connectionPoints[index].index, this);
	}

	if (crossroadComponent != nullptr)
	{
		crossroadComponent->connectRoad(indexInCrossroad, this);
	}

	_connectionPoints[index].crossroadComponent = crossroadComponent;
	_connectionPoints[index].index = indexInCrossroad;

	if (_connectionPoints[index].crossroadComponent != nullptr)
	{
		setConnectedPointPosition(index);
	}
}


RoadConnectionPoint& RoadObject::getConnectionPoint(int index)
{
	return _connectionPoints[index];
}


void RoadObject::setConnectedPointPosition(int connectionPointIndex)
{
	CrossroadComponent* crossroadComponent = _connectionPoints[connectionPointIndex].crossroadComponent;
	int indexInCrossroad = _connectionPoints[connectionPointIndex].index;

	if (_roadType == RoadType::LINES_AND_ARC)
	{
		int pointIndex;
		if (connectionPointIndex == 0)
		{
			pointIndex = 0;
		}
		else if (connectionPointIndex == 1)
		{
			pointIndex = _points.size() - 1;
		}

		_points[pointIndex] = crossroadComponent->getGlobalPositionOfConnectionPoint(indexInCrossroad);
	}
	else if (_roadType == RoadType::BEZIER_CURVES)
	{
		int pointIndex;
		int controlPointIndex;
		if (connectionPointIndex == 0)
		{
			pointIndex = 0;
			controlPointIndex = 1;
		}
		else if (connectionPointIndex == 1)
		{
			pointIndex = _points.size() - 1;
			controlPointIndex = pointIndex - 1;
		}

		glm::vec3 newPosition = crossroadComponent->getGlobalPositionOfConnectionPoint(indexInCrossroad);
		glm::vec3 direction = crossroadComponent->getGlobalDirectionOfConnectionPoint(indexInCrossroad);

		float length = glm::length(_points[pointIndex] - _points[controlPointIndex]);

		_points[pointIndex] = newPosition;
		_points[controlPointIndex] = _points[pointIndex] + length * direction;
	}
}


void RoadObject::resetConnectedPointPositions()
{
	for (int i = 0; i < 2; ++i)
	{
		if (_connectionPoints[i].crossroadComponent != nullptr)
		{
			setConnectedPointPosition(i);
		}
	}
}
