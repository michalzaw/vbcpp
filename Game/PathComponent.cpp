#include "PathComponent.h"

#include "GameConfig.h"

#include "../Graphics/GraphicsManager.h"

#include "../Scene/SceneManager.h"

#include "../Utils/BezierCurvesUtils.h"
#include "../Utils/ResourceManager.h"


RRoadProfile* helperProfile = nullptr;


RRoadProfile* getHelperProfile()
{
	if (helperProfile == nullptr)
	{
		helperProfile = new RRoadProfile("", "", "", "");

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
	}

	return helperProfile;
}


PathComponent::PathComponent(const std::vector<glm::vec3>& baseBezierCurveControlPoints, const glm::vec2& distanceFromBaseBezierCurve, PathDirection direction,
							 float marginBegin/* = 0.0f*/, float marginEnd/* = 0.0f*/)
	: Component(CT_PATH),
	_distanceFromBaseBezierCurve(distanceFromBaseBezierCurve), _baseBezierCurveControlPoints(baseBezierCurveControlPoints),
	_direction(direction),
	_marginBegin(marginBegin), _marginEnd(marginEnd)
{
	recalculate();
}


void PathComponent::onAttachedToScenObject()
{
	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SceneManager* sceneManager = getSceneObject()->getSceneManager();

		RRoadProfile* helperProfile = getHelperProfile();
		SceneObject* pathHelperSceneObject = sceneManager->addSceneObject("Path helper");
		pathHelperSceneObject->setFlags(SOF_NOT_SERIALIZABLE | SOF_NOT_SELECTABLE | SOF_NOT_SELECTABLE_ON_SCENE);
		getSceneObject()->addChild(pathHelperSceneObject);

		RoadObject* pathHelperComponent = getSceneObject()->getSceneManager()->getGraphicsManager()->addRoadObject(RoadType::POINTS, helperProfile, _curvePoints, {}, true, pathHelperSceneObject);
		pathHelperComponent->setCastShadows(false);
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

	glm::vec3 dir;
	glm::vec3 right;
	glm::vec3 realUp;
	for (int i = 0; i < _curvePoints.size() - 1; ++i)
	{
		dir = glm::normalize(_curvePoints[i + 1] - _curvePoints[i]);
		right = glm::cross(dir, glm::vec3(0, 1, 0));
		realUp = glm::cross(right, dir);

		_curvePoints[i] += glm::vec3(_distanceFromBaseBezierCurve.x) * right;
		_curvePoints[i] += glm::vec3(_distanceFromBaseBezierCurve.y) * realUp;
	}

	// modify last point using values from previous point
	_curvePoints[_curvePoints.size() - 1] += glm::vec3(_distanceFromBaseBezierCurve.x) * right;
	_curvePoints[_curvePoints.size() - 1] += glm::vec3(_distanceFromBaseBezierCurve.y) * realUp;

	if (_direction == PD_BACKWARD)
	{
		std::reverse(_curvePoints.begin(), _curvePoints.end());
	}
}


void PathComponent::update(float deltaTime)
{

}
