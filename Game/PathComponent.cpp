#include "PathComponent.h"

#include "GameConfig.h"

#include "../Graphics/BezierCurve.h"
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


PathComponent::PathComponent(PathDirection direction)
	: Component(CT_PATH),
	_direction(direction),
	_bezierCurveComponent(nullptr), _pathHelperComponent(nullptr)
{

}


void PathComponent::onAttachedToScenObject()
{
	_bezierCurveComponent = dynamic_cast<BezierCurve*>(getSceneObject()->getComponent(CT_BEZIER_CURVE));
	if (_bezierCurveComponent == nullptr)
	{
		LOG_ERROR("Cannot find BezierCurve component in object!");
		return;
	}

	_bezierCurveComponent->setOnPointAddedListener([this]() { recalculate(); });
	_bezierCurveComponent->setOnPointDeletedListener([this](int index) { recalculate(); });
	_bezierCurveComponent->setOnPointChangedPositionListener([this](int index, const glm::vec3& newPosition) { recalculate(); });
	_bezierCurveComponent->setOnCurveChangedListener([this]() { recalculate(); });
	_bezierCurveComponent->setOnComponentDeletedListener([this]() { _bezierCurveComponent = nullptr; });


	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SceneManager* sceneManager = getSceneObject()->getSceneManager();

		RRoadProfile* helperProfile = getHelperProfile();
		SceneObject* pathHelperSceneObject = sceneManager->addSceneObject("Path helper");
		pathHelperSceneObject->setFlags(SOF_NOT_SERIALIZABLE | SOF_NOT_SELECTABLE | SOF_NOT_SELECTABLE_ON_SCENE);
		getSceneObject()->addChild(pathHelperSceneObject);

		_pathHelperComponent = getSceneObject()->getSceneManager()->getGraphicsManager()->addRoadObject(RoadType::POINTS,
																										helperProfile,
																										{},
																										{},
																										false,
																										pathHelperSceneObject);
		_pathHelperComponent->setCastShadows(false);
	}

	recalculate();
}


void PathComponent::setConnection(int index, PathComponent* otherPath, int indexInOtherPath)
{
	if (this == otherPath && index == indexInOtherPath)
	{
		return;
	}

	if (index == indexInOtherPath)
	{
		return;
	}

	LOG_INFO("Create path connection: " + getSceneObject()->getName() + " (" + Strings::toString(index) + ") - " +
		otherPath->getSceneObject()->getName() + " (" + Strings::toString(indexInOtherPath) + ")");

	if (index == 0)
	{
		_previousPaths.push_back({ otherPath, indexInOtherPath });
	}

	if (index == 1)
	{
		_nextPaths.push_back({ otherPath, indexInOtherPath });
	}

	if (indexInOtherPath == 0)
	{
		otherPath->_previousPaths.push_back({ this, index });
	}

	if (indexInOtherPath == 1)
	{
		otherPath->_nextPaths.push_back({ this, index });
	}
}


void PathComponent::recalculate()
{
	_curvePoints.clear();

	if (_bezierCurveComponent != nullptr)
	{
		LOG_DEBUG("Fetch curve points");
		_curvePoints = _bezierCurveComponent->getCurvePoints();
	}
	else
	{
		LOG_WARNING("Null reference to BezierCurve component");
	}

	if (_direction == PD_BACKWARD)
	{
		std::reverse(_curvePoints.begin(), _curvePoints.end());
	}

	if (_pathHelperComponent != nullptr)
	{
		_pathHelperComponent->getPoints() = _curvePoints;
		_pathHelperComponent->buildModel();
	}
}


void PathComponent::update(float deltaTime)
{

}
