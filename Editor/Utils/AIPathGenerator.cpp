#include "AIPathGenerator.h"

#include "../../Game/GameLogicSystem.h"
#include "../../Game/PathComponent.h"

#include "../../Graphics/BezierCurve.h"
#include "../../Graphics/RoadObject.h"

#include "../../Scene/SceneManager.h"


namespace AIPathGenerator
{
	SceneObject* getCollectionSceneObject(RoadObject* roadComponent, SceneManager* sceneManager)
	{
		// todo: use existing collection object
		return sceneManager->addSceneObject(roadComponent->getSceneObject()->getName() + " - AI paths");
	}

	void generateAIPath(const std::string& name, BezierCurve* baseBezierCurve, PathDirection direction, const::glm::vec2& offsetFromBaseCurve, SceneObject* rootObject, SceneManager* sceneManager)
	{
		std::vector<glm::vec3> points;
		std::vector<int> segmentsPointsCount;
		baseBezierCurve->cutAndGetCurve(baseBezierCurve->getMarginBegin(), baseBezierCurve->getMarginEnd(), points, segmentsPointsCount);

		glm::vec2 offset = offsetFromBaseCurve;

		if (direction == PD_BACKWARD)
		{
			std::reverse(points.begin(), points.end());
			std::reverse(segmentsPointsCount.begin(), segmentsPointsCount.end());

			offset.x = -offset.x;
		}

		BezierCurve* bezierCurve = sceneManager->getGraphicsManager()->addBezierCurve(points, segmentsPointsCount, 0.0f, 0.0f, offset);
		PathComponent* path = sceneManager->getGameLogicSystem()->addPathComponent(PD_FORWARD);

		SceneObject* pathObject = sceneManager->addSceneObject(name);
		pathObject->addComponent(bezierCurve);
		pathObject->addComponent(path);

		rootObject->addChild(pathObject);
	}

	void generateAIPaths(RoadObject* roadComponent, SceneManager* sceneManager)
	{
		SceneObject* rootObject = getCollectionSceneObject(roadComponent, sceneManager);
		BezierCurve* bezierCurve = dynamic_cast<BezierCurve*>(roadComponent->getSceneObject()->getComponent(CT_BEZIER_CURVE));

		const std::vector<AIPath> aiPaths = roadComponent->getRoadProfile()->getAIPaths();

		for (const auto& aiPath : aiPaths)
		{
			generateAIPath(roadComponent->getSceneObject()->getName() + " - " + aiPath.name,
						   bezierCurve,
						   (PathDirection) aiPath.direction,
						   glm::vec2(aiPath.x, aiPath.y),
						   rootObject,
						   sceneManager);
		}
	}
}
