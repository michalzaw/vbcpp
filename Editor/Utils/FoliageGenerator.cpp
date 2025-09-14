#include "FoliageGenerator.h"

#include "../../Graphics/ShapePolygonComponent.h"

#include "../../Scene/SceneManager.h"

#include "../../Utils/PoissonDiscSampler.h"
#include "../../Utils/ResourceManager.h"


namespace FoliageGenerator
{
	void mapToVec2(const std::vector<glm::vec3>& polygonPoints, std::vector<glm::vec2>& outPolygonPointsVec2)
	{
		outPolygonPointsVec2.clear();

		for (const glm::vec3& point : polygonPoints)
		{
			outPolygonPointsVec2.emplace_back(point.x, point.z);
		}
	}

	void findMinAndMaxCoord(const std::vector<glm::vec2>& polygonPoints, glm::vec2& outMinCoords, glm::vec2& outMaxCoords)
	{
		outMinCoords = glm::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		outMaxCoords = glm::vec2(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

		for (const glm::vec2& point : polygonPoints)
		{
			if (point.x < outMinCoords.x)
			{
				outMinCoords.x = point.x;
			}
			if (point.x > outMaxCoords.x)
			{
				outMaxCoords.x = point.x;
			}

			if (point.y < outMinCoords.y)
			{
				outMinCoords.y = point.y;
			}
			if (point.y > outMaxCoords.y)
			{
				outMaxCoords.y = point.y;
			}
		}
	}

	bool isPointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygonPoints) {
		bool inside = false;
		size_t n = polygonPoints.size();

		for (size_t i = 0, j = n - 1; i < n; j = i++)
		{
			const glm::vec2& p1 = polygonPoints[i];
			const glm::vec2& p2 = polygonPoints[j];

			if (((p1.y > point.y) != (p2.y > point.y)) &&
				(point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x))
			{
				inside = !inside;
			}
		}

		return inside;
	}

	void generateFoliage(const std::vector<glm::vec3>& polygonPoints, const FoliageGeneratorData& generatorData, SceneObject* parentObject, SceneManager* sceneManager)
	{
		parentObject->removeAllChildrenFromScene();

		std::vector<glm::vec2> polygonPointsVec2;
		mapToVec2(polygonPoints, polygonPointsVec2);

		glm::vec2 minCoords;
		glm::vec2 maxCoords;
		findMinAndMaxCoord(polygonPointsVec2, minCoords, maxCoords);

		for (glm::vec2& point : polygonPointsVec2)
		{
			point = point - minCoords;
		}

		glm::vec2 size = maxCoords - minCoords;

		PoissonDiscSampler sampler(size.x, size.y, generatorData.minDistance);
		sampler.setPointValidationFunction([&polygonPointsVec2](const glm::vec2& point) { return isPointInPolygon(point, polygonPointsVec2); });
		std::vector<glm::vec2> points;
		sampler.generate(points);

		//RObject* objectDefinition = ResourceManager::getInstance().loadRObject("trees2023/spruce");
		RObject* objectDefinition = ResourceManager::getInstance().loadRObject(generatorData.objectName);
		for (const auto& point : points)
		{
			glm::vec2 newPoint = point + minCoords;
			SceneObject* newObject = RObjectLoader::createSceneObjectFromRObject(objectDefinition, objectDefinition->getName(), glm::vec3(newPoint.x, 0.0f, newPoint.y), glm::vec3(0.0f, 0.0f, 0.0f), sceneManager);
			parentObject->addChild(newObject);
		}

		LOG_DEBUG("Generated " + Strings::toString(points.size()) + " objects.");
	}
}