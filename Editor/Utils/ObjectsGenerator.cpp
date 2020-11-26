#include "ObjectsGenerator.h"

#include "../../Graphics/RoadObject.h"

#include "../../Utils/BezierCurvesUtils.h"
#include "../../Utils/Logger.h"
#include "../../Utils/ResourceManager.h"
#include "../../Utils/Strings.h"


namespace ObjectsGenerator
{
	void generateRoadPoints(RoadObject* roadComponent, float distance, std::vector<glm::vec3>& outPoints)
	{
		const std::vector<glm::vec3>& roadControlPoints = roadComponent->getPoints();

		for (int i = 0; i < roadControlPoints.size() - 1; i += 3)
		{
			std::vector<glm::vec3> segmentPoints;
			BezierCurvesUtils::generateBezierCurvePointsWithConstDistance(10000, distance, segmentPoints, [roadControlPoints, i](float t)
				{
					return BezierCurvesUtils::calculateBezierCurvePoint(roadControlPoints[i], roadControlPoints[i + 1], roadControlPoints[i + 2], roadControlPoints[i + 3], t);
				});

			if (i == 0)
			{
				outPoints.insert(outPoints.end(), segmentPoints.begin(), segmentPoints.end());
			}
			else
			{
				outPoints.insert(outPoints.end(), segmentPoints.begin() + 1, segmentPoints.end());
			}
		}
	}

	glm::vec3 calculateRoadDirection(const glm::vec3& currentPoint, const glm::vec3& previousPoint, const glm::vec3& nextPoint)
	{
		glm::vec3 v1 = currentPoint - previousPoint;
		glm::vec3 v2 = nextPoint - currentPoint;

		return glm::normalize(v1 + v2);
	}

	glm::vec3 calculateRightVector(const glm::vec3& direction)
	{
		return glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void generateObjectsAlongRoad(RoadObject* roadComponent, ObjectsAlongRoadGeneratorData* generatorData, SceneManager* sceneManager)
	{
		if (generatorData->objectsNames.size() == 0)
		{
			Logger::warning("Empty objectsNames array");
			return;
		}

		std::vector<glm::vec3> roadPoints;
		generateRoadPoints(roadComponent, 1.0f, roadPoints);
		Logger::info("Number of road points: " + toString(roadPoints.size()) + " with distance: 1.0");
		Logger::info("Number of objects: " + toString(generatorData->objectsNames.size()));

		int objectsCounter = 0;
		for (int i = 0; i < roadPoints.size(); i += generatorData->distance)
		{
			const glm::vec3& currentPoint = roadPoints[i];
			const glm::vec3& previousPoint = i - 1 >= 0 ? roadPoints[i - 1] : currentPoint;
			const glm::vec3& nextPoint = i + 1 < roadPoints.size() ? roadPoints[i + 1] : currentPoint;

			const glm::vec3 direction = calculateRoadDirection(currentPoint, previousPoint, nextPoint);
			const glm::vec3 rightVector = calculateRightVector(direction);

			const std::string& objectName = generatorData->objectsNames[objectsCounter % generatorData->objectsNames.size()];
			RObject* rObject = ResourceManager::getInstance().loadRObject(objectName);

			const glm::vec3 offset = rightVector * generatorData->distanceFromCenter;
			const glm::vec3 rotation = generatorData->rotation;
			if (generatorData->rightSide)
			{
				const glm::vec3 position = roadPoints[i] + offset;

				Logger::info("Index: " + toString(i) + ", objectName: " + objectName + ", pos: (" + vec3ToString(position) + "), rot: (" + vec3ToString(rotation) + ")");

				SceneObject* sceneObject = RObjectLoader::createSceneObjectFromRObject(rObject, objectName, position, rotation, sceneManager);

				++objectsCounter;
			}
			if (generatorData->leftSide)
			{
				const glm::vec3 position = roadPoints[i] - offset;

				Logger::info("Index: " + toString(i) + ", objectName: " + objectName + ", pos: (" + vec3ToString(position) + "), rot: (" + vec3ToString(rotation) + ")");

				SceneObject* sceneObject = RObjectLoader::createSceneObjectFromRObject(rObject, objectName, position, rotation, sceneManager);

				++objectsCounter;
			}
		}

		Logger::info("Successfully created " + toString(objectsCounter) + " objects");
	}
}
