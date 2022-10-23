#include "PathComponent.h"

#include "GameConfig.h"

#include "../Graphics/GraphicsManager.h"

#include "../Scene/SceneManager.h"

#include "../Utils/BezierCurvesUtils.h"
#include "../Utils/ResourceManager.h"


PathComponent::PathComponent(const std::vector<glm::vec3>& baseBezierCurveControlPoints, const glm::vec2& distanceFromBaseBezierCurve, float marginBegin/* = 0.0f*/, float marginEnd/* = 0.0f*/)
	: Component(CT_PATH),
	_distanceFromBaseBezierCurve(distanceFromBaseBezierCurve), _baseBezierCurveControlPoints(baseBezierCurveControlPoints),
	_marginBegin(marginBegin), _marginEnd(marginEnd)
{

}


void PathComponent::onAttachedToScenObject()
{

	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper

		RRoadProfile* helperProfile = new RRoadProfile("", "", "", "");
		RoadLane tempLane;
		tempLane.r1 = -0.5f;
		tempLane.r2 = 0.5f;
		tempLane.height1 = 0.05f;
		tempLane.height2 = 0.05f;
		tempLane.material = new Material;
		tempLane.material->scale = glm::vec2(1.0f, -0.5f);
		tempLane.material->shader = SOLID_MATERIAL;
		tempLane.material->diffuseTexture = ResourceManager::getInstance().loadTexture("Data/editor/path1.png");
		tempLane.material->shininess = 96.0f;
		helperProfile->getRoadLanes().push_back(tempLane);
		std::vector<RoadSegment> segments;
		SceneObject* pathHelperSceneObject = getSceneObject();

		RoadObject* pathHelperComponent = getSceneObject()->getSceneManager()->getGraphicsManager()->addRoadObject(RoadType::POINTS, helperProfile, _curvePoints, segments, true, pathHelperSceneObject);
		pathHelperComponent->buildModel();

		for (int i = 0; i < _baseBezierCurveControlPoints.size() / 3; ++i)
		{
			RoadSegment segment;
			segment.type = RST_BEZIER_CURVE;
			segment.pointsCount = 50;
			segments.push_back(segment);
		}

		//RoadObject* pathHelperComponent = getSceneObject()->getSceneManager()->getGraphicsManager()->addRoadObject(RoadType::BEZIER_CURVES, helperProfile, _baseBezierCurveControlPoints, segments, true, pathHelperSceneObject);
		//pathHelperComponent->buildModel();
	}
}


void PathComponent::recalculate()
{
	if (_baseBezierCurveControlPoints.size() > 0 && (_baseBezierCurveControlPoints.size() - 1) % 3 != 0)
	{
		LOG_ERROR("Invalid number of control points");
		return;
	}

	int quality = 50;

	_curvePoints.clear();

	if (_baseBezierCurveControlPoints.size() > 0)
	{
		for (int i = 0; i < _baseBezierCurveControlPoints.size() - 1; i += 3)
		{
			int segmentIndex = i / 3;
			std::vector<glm::vec3> segmentPoints;
			BezierCurvesUtils::generateBezierCurvePoints(_baseBezierCurveControlPoints[i],
														 _baseBezierCurveControlPoints[i + 1],
														 _baseBezierCurveControlPoints[i + 2],
														 _baseBezierCurveControlPoints[i + 3],
														 quality,
														 segmentPoints);

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

	if (_marginBegin > 0.0f)
	{
		BezierCurvesUtils::cutBezierCurveFromBegin(_curvePoints, _marginBegin);
	}
	if (_marginEnd > 0.0f)
	{
		BezierCurvesUtils::cutBezierCurveFromEnd(_curvePoints, _marginEnd);
	}

	for (int i = 0; i < _curvePoints.size() - 1; ++i)
	{
		glm::vec3 dir = glm::normalize(_curvePoints[i + 1] - _curvePoints[i]);
		glm::vec3 right = glm::cross(dir, glm::vec3(0, 1, 0));
		glm::vec3 realUp = glm::cross(right, dir);

		_curvePoints[i] += glm::vec3(_distanceFromBaseBezierCurve.x) * right;
		_curvePoints[i] += glm::vec3(_distanceFromBaseBezierCurve.y) * realUp;
	}
}


void PathComponent::update(float deltaTime)
{

}
