#include "AIPathGenerator.h"

#include "../../Game/GameLogicSystem.h"
#include "../../Game/PathComponent.h"

#include "../../Graphics/RoadObject.h"

#include "../../Scene/SceneManager.h"


namespace AIPathGenerator
{
	SceneObject* getCollectionSceneObject(RoadObject* roadComponent, SceneManager* sceneManager)
	{
		// todo: use existing collection object
		return sceneManager->addSceneObject(roadComponent->getSceneObject()->getName() + " - AI paths");
	}

	void generateAIPaths(RoadObject* roadComponent, SceneManager* sceneManager)
	{
		SceneObject* rootObject = getCollectionSceneObject(roadComponent, sceneManager);

		const std::vector<AIPath> aiPaths = roadComponent->getRoadProfile()->getAIPaths();

		for (const auto& aiPath : aiPaths)
		{
			const std::vector<glm::vec3>& roadControlPoints = roadComponent->getPoints();
			glm::vec2 distanceFromRoadAxis(aiPath.x, aiPath.y);
			PathComponent* path = sceneManager->getGameLogicSystem()->addPathComponent(roadControlPoints, distanceFromRoadAxis, (PathDirection) aiPath.direction,
																					   roadComponent->getMarginBegin(), roadComponent->getMarginEnd());

			SceneObject* pathObject = sceneManager->addSceneObject(roadComponent->getSceneObject()->getName() + " - " + aiPath.name);
			pathObject->addComponent(path);

			rootObject->addChild(pathObject);
		}
	}
}
