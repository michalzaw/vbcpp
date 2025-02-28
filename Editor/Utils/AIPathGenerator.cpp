#include "AIPathGenerator.h"

#include "../../Game/GameLogicSystem.h"
#include "../../Game/AI/PathComponent.h"

#include "../../Graphics/BezierCurve.h"
#include "../../Graphics/RoadIntersectionComponent.h"
#include "../../Graphics/RoadGenerator.h"
#include "../../Graphics/RoadObject.h"

#include "../../Scene/SceneManager.h"

#include "../../Utils/BezierCurvesUtils.h"


namespace AIPathGenerator
{
	SceneObject* getCollectionSceneObject(RoadObject* roadComponent, SceneManager* sceneManager)
	{
		// todo: use existing collection object
		return sceneManager->addSceneObject(roadComponent->getSceneObject()->getName() + " - AI paths");
	}

	SceneObject* getCollectionSceneObject(RoadIntersectionComponent* roadIntersectionComponent, SceneManager* sceneManager)
	{
		// todo: use existing collection object
		return sceneManager->addSceneObject(roadIntersectionComponent->getSceneObject()->getName() + " - AI paths");
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

	void generateAIPathForIntersection(const std::string& name, RoadIntersectionComponent* roadIntersectionComponent, const AIPath& aiPath, int road1Index, int road2Index, int quality, float curveFactor,
									   const std::vector<glm::vec3>& roadsCenterPoints, const std::vector<glm::vec3>& roadsDirections, const std::vector<glm::vec3>& roadsRightVectors,
									   SceneObject* rootObject, SceneManager* sceneManager)
	{
		std::vector<glm::vec3> bezierCurve1(4);
		bezierCurve1[0] = roadsCenterPoints[road1Index] + roadsRightVectors[road1Index] * aiPath.x;
		bezierCurve1[3] = roadsCenterPoints[road2Index] - roadsRightVectors[road2Index] * aiPath.x;
		bezierCurve1[1] = bezierCurve1[0] + roadsDirections[road1Index] * roadIntersectionComponent->getArc(road1Index) * curveFactor;
		bezierCurve1[2] = bezierCurve1[3] + roadsDirections[road2Index] * roadIntersectionComponent->getArc(road1Index) * curveFactor;

		std::vector<int> segmentsPointsCount = { quality };

		BezierCurve* bezierCurve = sceneManager->getGraphicsManager()->addBezierCurve(bezierCurve1, segmentsPointsCount, 0.0f, 0.0f, glm::vec2(0.0f, aiPath.y));
		PathComponent* path = sceneManager->getGameLogicSystem()->addPathComponent(PD_FORWARD);

		SceneObject* pathObject = sceneManager->addSceneObject(name);
		pathObject->addComponent(bezierCurve);
		pathObject->addComponent(path);

		rootObject->addChild(pathObject);
	}

	void generateAIPaths(RoadIntersectionComponent* roadIntersectionComponent, SceneManager* sceneManager, int pathQuality/* = 20*/, float innerPathCurveFactor/* = 2.0f*/, float outerPathCurveFactor/* = 1.0f*/)
	{
		std::vector<RoadConnectionPointData*> temp(2, nullptr);

		SceneObject* rootObject = getCollectionSceneObject(roadIntersectionComponent, sceneManager);

		const std::vector<AIPath> aiPaths = roadIntersectionComponent->getEdgeRoadProfile()->getAIPaths();
		const std::vector<RoadConnectedToIntersection>& roads = roadIntersectionComponent->getConnectedRoads();

		std::vector<glm::vec3> roadsCenterPoints(roads.size());
		std::vector<glm::vec3> roadsDirections(roads.size());
		std::vector<glm::vec3> roadsRightVectors(roads.size());

		for (int i = 0; i < roads.size(); ++i)
		{
			const auto& roadCurvePoints = roads[i].road->getCurvePoints();
			int road1CenterPointIndex = roads[i].connectionPointInRoadIndex == 0 ? 0 : roadCurvePoints.size() - 1;

			glm::vec3 roadCenterPoint = roadCurvePoints[road1CenterPointIndex];
			glm::vec2 dir = RoadGenerator::calculateDirection(roadCurvePoints, temp, road1CenterPointIndex);
			glm::vec3 direction = glm::vec3(dir.x, 0.0f, dir.y);
			glm::vec2 rightVector = glm::vec2(-direction.z, direction.x);

			// jesli punkt poczatkowy drogi to odwracamy direction i rightVector (traktujemy kazda droge jak wchodzaca do skrzyzowania, wiec jej kierunek powinien byc w strone srodka skrzyzowania)
			if (roads[i].connectionPointInRoadIndex == 0)
			{
				direction = -direction;
				rightVector = -rightVector;
			}

			roadsCenterPoints[i] = roadCenterPoint + glm::vec3(0.0f, 0.0f, 0.0f);;
			roadsDirections[i] = glm::vec3(direction.x, 0.0f, direction.z);
			roadsRightVectors[i] = glm::vec3(rightVector.x, 0.0f, rightVector.y);
		}

		for (const AIPath& aiPath : aiPaths)
		{
			// jesli sciezka wewnetrzna to generujemy poloczenie do kazdej drogi podlaczonej do skrzyzowania
			if (aiPath.intersectionMode == APIM_INNER)
			{
				for (int i = 0; i < roads.size(); ++i)
				{
					for (int j = 1; j < roads.size(); ++j)
					{
						int road1Index = i;
						int road2Index = (i + j) % roads.size();

						std::string pathName = roadIntersectionComponent->getSceneObject()->getName() + " - " + aiPath.name + " - " + Strings::toString(road1Index) + "." + Strings::toString(road2Index);

						generateAIPathForIntersection(pathName, roadIntersectionComponent, aiPath, road1Index, road2Index, pathQuality, innerPathCurveFactor,
							roadsCenterPoints, roadsDirections, roadsRightVectors,
							rootObject, sceneManager);
					}
				}
			}
			// jesli sciezka zewnetrzna to generujemy poloczenie tylko do jednej, nastepnej drogi
			else if (aiPath.intersectionMode == APIM_OUTER)
			{
				for (int i = 0; i < roads.size(); ++i)
				{
					int road1Index = i;
					int road2Index = (i + 1) % roads.size();

					std::string pathName = roadIntersectionComponent->getSceneObject()->getName() + " - " + aiPath.name + " - " + Strings::toString(road1Index) + "." + Strings::toString(road2Index);

					generateAIPathForIntersection(pathName, roadIntersectionComponent, aiPath, road1Index, road2Index, pathQuality, outerPathCurveFactor,
						roadsCenterPoints, roadsDirections, roadsRightVectors,
						rootObject, sceneManager);
				}
			}
		}
	}
}
