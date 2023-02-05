#include "ObjectsGenerator.h"

#include <random>

#include "../../Graphics/BezierCurve.h"

#include "../../Scene/SceneManager.h"

#include "../../Utils/BezierCurvesUtils.h"
#include "../../Utils/Logger.h"
#include "../../Utils/ResourceManager.h"
#include "../../Utils/Strings.h"


namespace ObjectsGenerator
{
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());

	SceneObject* getExistingCollectionSceneObject(Component* component, const std::string& collectionName)
	{
		const auto& children = component->getSceneObject()->getChildren();
		for (SceneObject* child : children)
		{
			if (child->getName() == collectionName)
			{
				return child;
			}
		}

		return nullptr;
	}

	void generateCurvePoints(BezierCurve* bezierCurve, float distance, std::vector<glm::vec3>& outPoints)
	{
		const std::vector<glm::vec3>& controlPoints = bezierCurve->getPoints();

		for (int i = 0; i < controlPoints.size() - 1; i += 3)
		{
			std::vector<glm::vec3> segmentPoints;
			BezierCurvesUtils::generateBezierCurvePointsWithConstDistance(10000, distance, segmentPoints, [&controlPoints, i](float t)
				{
					return BezierCurvesUtils::calculateBezierCurvePoint(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], t);
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

	glm::vec3 calculateDirection(const glm::vec3& currentPoint, const glm::vec3& previousPoint, const glm::vec3& nextPoint)
	{
		glm::vec3 v1 = currentPoint - previousPoint;
		glm::vec3 v2 = nextPoint - currentPoint;

		return glm::normalize(v1 + v2);
	}

	glm::vec3 calculateRightVector(const glm::vec3& direction)
	{
		return glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 calculateUpVector(const glm::vec3& direction, const glm::vec3& rightVector)
	{
		return glm::cross(rightVector, direction);
	}

	glm::vec3 calculateRotation(const glm::mat3& tangentMatrix, const glm::vec3 rotationAngles)
	{
		glm::mat4 localRotationMatrix;
		localRotationMatrix = glm::rotate(degToRad(rotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		localRotationMatrix *= glm::rotate(degToRad(rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		localRotationMatrix *= glm::rotate(degToRad(rotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat3 localRotationMatrix3x3 = glm::mat3(localRotationMatrix/*[0], localRotationMatrix[1], localRotationMatrix[2],
													 localRotationMatrix[4], localRotationMatrix[5], localRotationMatrix[6],
													 localRotationMatrix[8], localRotationMatrix[9], localRotationMatrix[10]*/);

		glm::mat3 globalRotationMatrix3x3 = tangentMatrix * localRotationMatrix3x3;
		glm::mat4 globalRotationMatrix = glm::mat4(globalRotationMatrix3x3/*[0], globalRotationMatrix3x3[1], globalRotationMatrix3x3[2], 0.0f,
												   globalRotationMatrix3x3[3], globalRotationMatrix3x3[4], globalRotationMatrix3x3[5], 0.0f,
												   globalRotationMatrix3x3[6], globalRotationMatrix3x3[7], globalRotationMatrix3x3[8], 0.0f,
												   0.0f, 0.0f, 0.0f, 1.0f*/);

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(globalRotationMatrix, scale, rotation, translation, skew, perspective);

		glm::vec3 rotationEulerAngles = glm::eulerAngles(rotation);
		rotationEulerAngles = glm::vec3(radToDeg(-rotationEulerAngles.x), radToDeg(-rotationEulerAngles.y), radToDeg(-rotationEulerAngles.z));

		return rotationEulerAngles;
		//return glm::vec3(0.0f, 90.0f, 0.0f);
	}

	void addObject(RObject* rObject, const glm::vec3& position, const glm::vec3& rotation, SceneManager* sceneManager)
	{

	}

	float generateRandomFloat(float minValue, float maxValue)
	{
		std::uniform_real_distribution<> distribution(minValue, maxValue);

		return distribution(randomGenerator);
	}

	float generateRandomInt(int minValue, int maxValue)
	{
		std::uniform_int_distribution<> distribution(minValue, maxValue);

		return distribution(randomGenerator);
	}

	glm::vec3 generateRandomVec3(const glm::vec3& minValue, const glm::vec3& maxValue)
	{
		return glm::vec3(generateRandomFloat(minValue.x, maxValue.x),
						 generateRandomFloat(minValue.y, maxValue.y),
						 generateRandomFloat(minValue.z, maxValue.z));
	}

	void generateObjectsAlongCurve(BezierCurve* bezierCurve, ObjectsAlongCurveGeneratorData* generatorData, SceneManager* sceneManager)
	{
		if (generatorData->objectsNames.size() == 0)
		{
			LOG_WARNING("Empty objectsNames array");
			return;
		}

		const std::string& objectsCollectionName = generatorData->objectsCollectionName;

		std::vector<glm::vec3> curvePoints;
		generateCurvePoints(bezierCurve, 1.0f, curvePoints);
		LOG_INFO("Generate objects: " + objectsCollectionName);
		LOG_INFO("Number of curve points: " + Strings::toString(curvePoints.size()) + " with distance: 1.0");
		LOG_INFO("Number of objects: " + Strings::toString(generatorData->objectsNames.size()));

		
		SceneObject* rootObject = getExistingCollectionSceneObject(bezierCurve, objectsCollectionName);
		if (rootObject != nullptr)
		{
			rootObject->removeAllChildrenFromScene();
		}
		else
		{
			rootObject = sceneManager->addSceneObject(bezierCurve->getSceneObject()->getName() + " - " + objectsCollectionName);
			// todo: attach object to roadObject. Support in map file
			//roadComponent->getSceneObject()->addChild(rootObject);
		}

		int objectsCounter = 0;
		for (int i = 0; i < curvePoints.size(); i += generatorData->distance)
		{
			const glm::vec3& currentPoint = curvePoints[i];
			const glm::vec3& previousPoint = i - 1 >= 0 ? curvePoints[i - 1] : currentPoint;
			const glm::vec3& nextPoint = i + 1 < curvePoints.size() ? curvePoints[i + 1] : currentPoint;

			const glm::vec3 direction = calculateDirection(currentPoint, previousPoint, nextPoint);
			const glm::vec3 rightVector = calculateRightVector(direction);
			const glm::vec3 upVector = calculateUpVector(direction, rightVector);

			/*glm::mat3 tangentMatrix(direction.x, direction.y, direction.z,
									upVector.x, upVector.y, upVector.z,
									rightVector.x, rightVector.y, rightVector.z);*/

			glm::mat3 tangentMatrix(rightVector.x, rightVector.y, rightVector.z,
									upVector.x, upVector.y, upVector.z,
									-direction.x, -direction.y, -direction.z);


			//const glm::vec3 offset = rightVector * generatorData->distanceFromCenter;
			if (generatorData->rightSide)
			{
				const std::string& objectName = generatorData->objectsNames[generateRandomInt(0, generatorData->objectsNames.size() - 1)];
				RObject* rObject = ResourceManager::getInstance().loadRObject(objectName);

				const glm::vec3 randomOffset = generateRandomVec3(-generatorData->positionOffset, generatorData->positionOffset);
				const glm::vec3 randomRotation = generateRandomVec3(-generatorData->rotationOffset, generatorData->rotationOffset);

				const glm::vec3 offset = tangentMatrix * glm::vec3(generatorData->offsetFromCenter.x + randomOffset.x, generatorData->offsetFromCenter.y + randomOffset.y, 0.0f + randomOffset.z);
				const glm::vec3 position = curvePoints[i] + offset;
				const glm::vec3 rotation = calculateRotation(tangentMatrix, generatorData->rotation + randomRotation);

				LOG_INFO("Index=" + Strings::toString(i) + ", "	+ LOG_VARIABLE(objectName) + ", " + LOG_VARIABLE(position) + ", " + LOG_VARIABLE(rotation));

				SceneObject* sceneObject = RObjectLoader::createSceneObjectFromRObject(rObject, objectName, position, rotation, sceneManager);

				rootObject->addChild(sceneObject);

				++objectsCounter;
			}
			if (generatorData->leftSide)
			{
				const std::string& objectName = generatorData->objectsNames[generateRandomInt(0, generatorData->objectsNames.size() - 1)];
				RObject* rObject = ResourceManager::getInstance().loadRObject(objectName);

				const glm::vec3 randomOffset = generateRandomVec3(-generatorData->positionOffset, generatorData->positionOffset);
				const glm::vec3 randomRotation = generateRandomVec3(-generatorData->rotationOffset, generatorData->rotationOffset);

				const glm::vec3 offset = tangentMatrix * glm::vec3(-generatorData->offsetFromCenter.x + randomOffset.x, generatorData->offsetFromCenter.y + randomOffset.y, 0.0f + randomOffset.z);
				const glm::vec3 position = curvePoints[i] + offset;
				const glm::vec3 rotation = calculateRotation(tangentMatrix, -(generatorData->rotation + randomRotation));

				LOG_INFO("Index=" + Strings::toString(i) + ", " + LOG_VARIABLE(objectName) + ", " + LOG_VARIABLE(position) + ", " + LOG_VARIABLE(rotation));

				SceneObject* sceneObject = RObjectLoader::createSceneObjectFromRObject(rObject, objectName, position, rotation, sceneManager);

				rootObject->addChild(sceneObject);

				++objectsCounter;
			}
		}

		LOG_INFO("Successfully created " + Strings::toString(objectsCounter) + " objects");
	}
}
