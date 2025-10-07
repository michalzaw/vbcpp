#include "FoliagePatch.h"

#include "../Graphics/ShapePolygonComponent.h"

#include "../Scene/SceneManager.h"

#include "../Utils/GeometryUtils.h"
#include "../Utils/Logger.h"
#include "../Utils/PoissonDiscSampler.h"
#include "../Utils/ResourceManager.h"


FoliagePatch::FoliagePatch()
	: Component(CT_FOLIAGE_PATCH)
{

}


FoliagePatch::~FoliagePatch()
{

}


void FoliagePatch::generateFoliage(const std::vector<glm::vec3>& polygonPoints, const FoliagePatchLayer& foliageData, SceneObject* parentObject, SceneManager* sceneManager)
{
	std::vector<glm::vec2> polygonPointsVec2;
	GeometryUtils::mapToVec2(polygonPoints, polygonPointsVec2);

	glm::vec2 minCoords;
	glm::vec2 maxCoords;
	GeometryUtils::findMinAndMaxCoord(polygonPointsVec2, minCoords, maxCoords);

	for (glm::vec2& point : polygonPointsVec2)
	{
		point = point - minCoords;
	}

	glm::vec2 size = maxCoords - minCoords;

	PoissonDiscSampler sampler(size.x, size.y, foliageData.minDistance, foliageData.seed);
	sampler.setPointValidationFunction([&polygonPointsVec2](const glm::vec2& point) { return GeometryUtils::isPointInPolygon(point, polygonPointsVec2); });
	std::vector<glm::vec2> points;
	sampler.generate(points);

	//RObject* objectDefinition = ResourceManager::getInstance().loadRObject("trees2023/spruce");
	/*RObject* objectDefinition = ResourceManager::getInstance().loadRObject("trees/spruce");
	for (const auto& point : points)
	{
		glm::vec2 newPoint = point + minCoords;
		SceneObject* newObject = RObjectLoader::createSceneObjectFromRObject(objectDefinition, objectDefinition->getName(), glm::vec3(newPoint.x, 0.0f, newPoint.y), glm::vec3(0.0f, 0.0f, 0.0f), sceneManager);
		parentObject->addChild(newObject);
	}*/


	RObject* objectDefinition = ResourceManager::getInstance().loadRObject(foliageData.objectName);
	SceneObject* newObject = RObjectLoader::createSceneObjectFromRObject(objectDefinition, objectDefinition->getName(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), sceneManager);
	RenderObject* renderObject = newObject->getComponentWithCasting<RenderObject>(CT_RENDER_OBJECT);
	MultiRenderObject* multiRenderObject = sceneManager->getGraphicsManager()->addMultiRenderObject(renderObject);
	for (const auto& point : points)
	{
		glm::vec2 newPoint = point + minCoords;
		multiRenderObject->getInstancesPositions().emplace_back(newPoint.x, 0.0f, newPoint.y);
	}
	multiRenderObject->recreateInstancesPositionVBO();

	newObject->addComponent(multiRenderObject);

	parentObject->addChild(newObject);

	newObject->setFlags(SOF_NOT_SERIALIZABLE);

	LOG_DEBUG("Generated " + Strings::toString(points.size()) + " objects.");
}


FoliagePatchLayer& FoliagePatch::addLayer(const std::string& objectName, float minDistance/* = 5.0f*/, unsigned int seed/* = (unsigned int)std::time(0)*/)
{
	_layers.emplace_back(objectName, minDistance, seed);

	return _layers[_layers.size() - 1];
}


FoliagePatchLayer& FoliagePatch::getLayer(unsigned int index)
{
	if (index < 0 || index >= _layers.size())
	{
		LOG_ERROR("Invalid layer index=" + Strings::toString(index) + ". Layers size=" + Strings::toString(_layers.size()));
	}

	return _layers[index];
}


void FoliagePatch::removeLayer(unsigned int index)
{
	if (index < 0 || index >= _layers.size())
	{
		LOG_ERROR("Invalid layer index=" + Strings::toString(index) + ". Layers size=" + Strings::toString(_layers.size()));
	}

	_layers.erase(_layers.begin() + index);
}


unsigned int FoliagePatch::getLayersCount()
{
	return _layers.size();
}


void FoliagePatch::generateFoliage()
{
	ShapePolygonComponent* shapePolygon = _object->getComponentWithCasting<ShapePolygonComponent>(CT_SHAPE_POLYGON);
	if (shapePolygon == nullptr)
	{
		LOG_ERROR("Object does not contain polygon component.");
		return;
	}

	_object->removeAllChildrenFromScene();

	for (const auto& layer : _layers)
	{
		LOG_DEBUG("Generate foliage layer: " + layer.objectName);
		generateFoliage(shapePolygon->getPoints(), layer, _object, _object->getSceneManager());
	}
}
