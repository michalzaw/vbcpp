#include "RoadPolygonsGenerator.h"

#include "../../Graphics/RoadObject.h"
#include "../../Graphics/RoadGenerator.h"
#include "../../Graphics/ShapePolygonComponent.h"

#include "../../Scene/SceneManager.h"


namespace RoadPolygonsGenerator
{
	glm::vec3 calculateRoadDirectoionXZ(RoadObject* road)
	{
		const auto& curvePoints = road->getCurvePoints();
		int lastPointIndex = curvePoints.size() - 1;

		glm::vec3 p1 = glm::vec3(curvePoints[0].x, 0.0f, curvePoints[0].z);
		glm::vec3 p2 = glm::vec3(curvePoints[lastPointIndex].x, 0.0f, curvePoints[lastPointIndex].z);

		return glm::normalize(p2 - p1);
	}

	glm::vec3 calculateDirectionBetweenRoads(RoadObject* road1, RoadObject* road2)
	{
		const auto& curvePoints1 = road1->getCurvePoints();
		const auto& curvePoints2 = road2->getCurvePoints();

		int lastPointIndex = curvePoints1.size() - 1;

		glm::vec3 p1 = glm::vec3(curvePoints1[lastPointIndex].x, 0.0f, curvePoints1[lastPointIndex].z);
		glm::vec3 p2 = glm::vec3(curvePoints2[0].x, 0.0f, curvePoints2[0].z);

		return glm::normalize(p2 - p1);
	}

	void generatePolygon(const std::vector<RoadObject*>& roads, SceneManager* sceneManager)
	{
		LOG_DEBUG("Roads: Generate polygon");

		std::vector<RoadConnectionPointData*> temp(2, nullptr);

		std::vector<glm::vec3> roadDirections(roads.size());

		std::vector<glm::vec3> points;

		for (int i = 0; i < roads.size(); ++i)
		{
			RoadObject* road = roads[i];

			const std::vector<glm::vec3>& curvePoints = road->getCurvePoints();

			roadDirections[i] = calculateRoadDirectoionXZ(roads[i]);

			bool inverseVector = false;
			if (i > 0)
			{
				glm::vec3 v1 = roadDirections[i - 1];
				//glm::vec3 v2 = calculateDirectionBetweenRoads(roads[i - 1], roads[i]);
				glm::vec3 v2 = glm::normalize(glm::vec3(curvePoints[0].x, 0.0f, curvePoints[0].z) - glm::vec3(points[points.size() - 1].x, 0.0f, points[points.size() - 1].z));
				glm::vec3 v3 = roadDirections[i];

				glm::vec3 crossProduct1 = glm::cross(v1, v2);
				glm::vec3 crossProduct2 = glm::cross(v2, v3);

				LOG_DEBUG(LOG_VARIABLE(crossProduct1));
				LOG_DEBUG(LOG_VARIABLE(crossProduct2));

				if ((crossProduct1.y < 0.0f) != (crossProduct2.y < 0.0f))
				{
					inverseVector = true;

					roadDirections[i] = -roadDirections[i];
				}
			}

			int firstIndex = points.size();
			points.resize(points.size() + curvePoints.size());

			for (int j = 0; j < curvePoints.size(); ++j)
			{
				glm::vec2 currentCurvePoint = glm::vec2(curvePoints[j].x, curvePoints[j].z);
				glm::vec2 direction = RoadGenerator::calculateDirection(curvePoints, temp, j);
				glm::vec2 rightVector = glm::vec2(-direction.y, direction.x);

				glm::vec2 point = currentCurvePoint + road->getRoadProfile()->getEdges()[1].x * rightVector;

				int index = inverseVector ? (firstIndex + curvePoints.size() - 1 - j) : (firstIndex + j);
				points[index] = glm::vec3(point.x, curvePoints[j].y + road->getRoadProfile()->getEdges()[1].y, point.y);
			}
		}

		// weryfikacja czy kolejnosc jest zgodna z ruchem wskazowek zegara - jesli nie to dowroc kolejnosc
		// https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order
		// https://www.element84.com/blog/determining-the-winding-of-a-polygon-given-as-a-set-of-ordered-points
		float sum = 0.0f;
		for (int i = 0; i < points.size(); ++i)
		{
			float edge = (points[(i + 1) % points.size()].x - points[i].x) * (points[(i + 1) % points.size()].z + points[i].z);
			sum += edge;
		}

		LOG_DEBUG(LOG_VARIABLE(sum));
		if (sum < 0)
		{
			LOG_DEBUG("Clockwise order: YES");
		}
		else
		{
			LOG_DEBUG("Clockwise order: NO");

			std::reverse(points.begin(), points.end());
		}

		SceneObject* polygonSceneObject = sceneManager->addSceneObject("Polygon");
		polygonSceneObject->addComponent(new ShapePolygonComponent(points));
	}
}
