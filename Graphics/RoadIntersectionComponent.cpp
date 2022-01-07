#include "RoadIntersectionComponent.h"

#include "ShapePolygonComponent.h"
#include "RoadGenerator.h"
#include "RoadObject.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Utils/BezierCurvesUtils.h"


RoadIntersectionComponent::RoadIntersectionComponent()
	: Component(CT_ROAD_INTERSECTION),
	_length(10.0f), _width(5.0f), _arc(2.0f), _quality(11),
	_needRebuildConnectedRoad(false)
{

}


RoadIntersectionComponent::~RoadIntersectionComponent()
{

}


void RoadIntersectionComponent::connectRoad(RoadObject* roadObject, int connectionPointInRoadIndex)
{
	_roads.push_back(RoadConnectedToIntersection(roadObject, connectionPointInRoadIndex));
}


void RoadIntersectionComponent::disconnectRoad(RoadObject* roadObject, int connectionPointInRoadIndex)
{
	for (std::vector<RoadConnectedToIntersection>::iterator i = _roads.begin(); i != _roads.end(); ++i)
	{
		if (i->road == roadObject && i->connectionPointInRoadIndex == connectionPointInRoadIndex)
		{
			i = _roads.erase(i);
			break;
		}
	}
}


const std::vector<RoadConnectedToIntersection>& RoadIntersectionComponent::getConnectedRoads()
{
	return _roads;
}


void RoadIntersectionComponent::setLength(float length)
{
	_length = length;

	needRebuildConnectedRoad();
}


void RoadIntersectionComponent::setWidth(float width)
{
	_width = width;
}


void RoadIntersectionComponent::setArc(float arc)
{
	_arc = arc;
}


void RoadIntersectionComponent::setQuality(int quality)
{
	_quality = quality;
}


float RoadIntersectionComponent::getLength()
{
	return _length;
}


float RoadIntersectionComponent::getWidth()
{
	return _width;
}


float RoadIntersectionComponent::getArc()
{
	return _arc;
}


int RoadIntersectionComponent::getQuality()
{
	return _quality;
}


void RoadIntersectionComponent::createPolygon()
{
	auto shapePolygonComponent = new ShapePolygonComponent;
	getSceneObject()->removeComponent(getSceneObject()->getComponent(CT_SHAPE_POLYGON));
	getSceneObject()->addComponent(shapePolygonComponent);

	std::vector<RoadConnectionPointData*> temp(2, nullptr);

	glm::vec3 centerPoint = getSceneObject()->getGlobalPosition();

	std::vector<glm::vec3> roadsCenterPoints(_roads.size());
	std::vector<glm::vec3> roadsDirections(_roads.size());
	std::vector<glm::vec3> roadsRightVectors(_roads.size());
	std::vector<std::vector<glm::vec3>> pointsOnRoadAxis(_roads.size());
	std::vector<std::vector<glm::vec3>> bezierCurves(_roads.size());

	int numberOfPointsOnRoadAxis = (_quality - 1) / 2;

	for (int i = 0; i < _roads.size(); ++i)
	{
		const auto& roadCurvePoints = _roads[i].road->getCurvePoints();
		int road1CenterPointIndex = _roads[i].connectionPointInRoadIndex == 0 ? 0 : roadCurvePoints.size() - 1;

		glm::vec3 roadCenterPoint = roadCurvePoints[road1CenterPointIndex];
		glm::vec2 direction = RoadGenerator::calculateDirection(roadCurvePoints, temp, road1CenterPointIndex);
		glm::vec2 rightVector = glm::vec2(-direction.y, direction.x);

		// jesli punkt poczatkowy drogi to odwracamy direction i rightVector (traktujemy kazda droge jak wchodzaca do skrzyzowania, wiec jej kierunek powinien byc w strone srodka skrzyzowania)
		if (_roads[i].connectionPointInRoadIndex == 0)
		{
			direction = -direction;
			rightVector = -rightVector;
		}

		roadsCenterPoints[i] = roadCenterPoint;
		roadsDirections[i] = glm::vec3(direction.x, 0.0f, direction.y);
		roadsRightVectors[i] = glm::vec3(rightVector.x, 0.0f, rightVector.y);

		glm::vec3 roadToIntersectionCenterVector = centerPoint - roadsCenterPoints[i];
		float delta = glm::length(roadToIntersectionCenterVector) / numberOfPointsOnRoadAxis;
		roadToIntersectionCenterVector = glm::normalize(roadToIntersectionCenterVector);

		for (int j = 0; j < numberOfPointsOnRoadAxis; ++j)
		{
			pointsOnRoadAxis[i].push_back(roadsCenterPoints[i] + roadToIntersectionCenterVector * delta * j);
		}
	}

	for (int i = 0; i < _roads.size(); ++i)
	{
		int road1Index = i;
		int road2Index = (i + 1) % _roads.size();

		std::vector<glm::vec3> bezierCurve1(4);
		bezierCurve1[0] = roadsCenterPoints[road1Index] + roadsRightVectors[road1Index] *_width;
		bezierCurve1[3] = roadsCenterPoints[road2Index] - roadsRightVectors[road2Index] * _width;
		bezierCurve1[1] = bezierCurve1[0] + roadsDirections[road1Index] * _arc;
		bezierCurve1[2] = bezierCurve1[3] + roadsDirections[road2Index] * _arc;

		BezierCurvesUtils::generateBezierCurvePoints(bezierCurve1[0], bezierCurve1[1], bezierCurve1[2], bezierCurve1[3], _quality, bezierCurves[i]);

		// debug
		// -----------
		for (int j = 0; j < bezierCurves[i].size(); ++j)
		{
			shapePolygonComponent->addPoint(bezierCurves[i][j]);
		}

		for (int j = 0; j < pointsOnRoadAxis[road1Index].size(); ++j)
		{
			shapePolygonComponent->addPoint(pointsOnRoadAxis[road1Index][j]);
		}

		shapePolygonComponent->addPoint(centerPoint);

		for (int j = 0; j < pointsOnRoadAxis[road2Index].size(); ++j)
		{
			shapePolygonComponent->addPoint(pointsOnRoadAxis[road2Index][j]);
		}
		// -----------
	}


	// vertices
	unsigned int numberOfPointsInAllBezierCurves = _roads.size() * _quality;
	unsigned int numberOfVertices =
		numberOfPointsInAllBezierCurves + // liczba punktów w krzywych beziera
		_roads.size() * numberOfPointsOnRoadAxis + // liczba punktów na osiach drog
		1; // punkt œrodkowy

	Vertex* vertices = new Vertex[numberOfVertices];

	for (int i = 0; i < _roads.size(); ++i)
	{
		for (int j = 0; j < bezierCurves[i].size(); ++j)
		{
			vertices[i * _quality + j].position = bezierCurves[i][j];
		}

		for (int j = 0; j < pointsOnRoadAxis[i].size(); ++j)
		{
			vertices[numberOfPointsInAllBezierCurves + i * numberOfPointsOnRoadAxis + j].position = pointsOnRoadAxis[i][j];
		}
	}

	vertices[numberOfVertices - 1].position = centerPoint;

	// indices
	unsigned int numberOfIndices = _roads.size() * (_quality -1) * 6;
	unsigned int* indices = new unsigned int[numberOfIndices];

	LOG_DEBUG(LOG_VARIABLE(numberOfIndices));

	int index = 0;
	for (int i = 0; i < _roads.size(); ++i)
	//for (int i = 0; i < 1; ++i)
	{
		int i2 = (i + 1) % _roads.size();
		for (int j = 0; j < _quality - 1; ++j)
		{
			indices[index++] = (i * _quality) + j;
			indices[index++] = (i * _quality) + j + 1;
			if (j < numberOfPointsOnRoadAxis)
			{
				indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i + j;
			}
			else if (j == numberOfPointsOnRoadAxis)
			{
				indices[index++] = numberOfVertices - 1;
			}
			else
			{
				int k = j - numberOfPointsOnRoadAxis - 1;
				indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i2 + numberOfPointsOnRoadAxis - k - 1;
			}



			indices[index++] = (i * _quality) + j + 1;

			//indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i + j + 1;
			int k = j + 1;
			if (k < numberOfPointsOnRoadAxis)
			{
				indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i + k;
			}
			else if (k == numberOfPointsOnRoadAxis)
			{
				indices[index++] = numberOfVertices - 1;
			}
			else
			{
				int l = k - numberOfPointsOnRoadAxis - 1;
				indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i2 + numberOfPointsOnRoadAxis - l - 1;
			}

			//indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i + j;
			if (j < numberOfPointsOnRoadAxis)
			{
				indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i + j;
			}
			else if (j == numberOfPointsOnRoadAxis)
			{
				indices[index++] = numberOfVertices - 1;
			}
			else
			{
				int k = j - numberOfPointsOnRoadAxis - 1;
				indices[index++] = numberOfPointsInAllBezierCurves + numberOfPointsOnRoadAxis * i2 + numberOfPointsOnRoadAxis - k - 1;
			}
		}
	}
	LOG_DEBUG(LOG_VARIABLE(index));




	/*unsigned int numberOfVertices = bezierCurves[0].size() + pointsOnRoadAxis[0].size() + pointsOnRoadAxis[1].size() + 1;
	unsigned int numberOfIndices = (numberOfVertices + 2) * 3;
	Vertex* vertices = new Vertex[numberOfVertices];
	unsigned int* indices = new unsigned int[numberOfIndices];

	int index = 0;
	for (int j = 0; j < bezierCurves[0].size(); ++j)
	{
		vertices[index++].position = bezierCurves[0][j];
	}

	for (int j = 0; j < pointsOnRoadAxis[0].size(); ++j)
	{
		vertices[index++].position = pointsOnRoadAxis[0][j];
	}

	vertices[index++].position = centerPoint;

	for (int j = pointsOnRoadAxis[1].size() - 1; j >= 0; --j)
	{
		vertices[index++].position = pointsOnRoadAxis[1][j];
	}

	int a = bezierCurves[0].size();
	index = 0;
	for (int j = 0; j < a - 1; ++j)
	{
		indices[index++] = j;
		indices[index++] = j + 1;
		indices[index++] = a + j;

		indices[index++] = j + 1;
		indices[index++] = a + j;
		indices[index++] = a + j + 1;
	}*/

	Component* cmp = getSceneObject()->getComponent(CT_RENDER_OBJECT);
	if (cmp != nullptr)
	{
		getSceneObject()->removeComponent(cmp);
	}

	StaticModelMesh* meshes = new StaticModelMesh[1];
	meshes[0].setMeshData(vertices, numberOfVertices, indices, numberOfIndices, 0, WIREFRAME_MATERIAL, true);

	std::vector<Material*> materials;
	materials.push_back(new Material);
	materials[0]->shader = WIREFRAME_MATERIAL;

	StaticModelNode* modelNode = new StaticModelNode;
	modelNode->name = "road";
	modelNode->meshes = meshes;
	modelNode->meshesCount = 1;
	modelNode->parent = nullptr;

	RStaticModel* _generatedModel = new RStaticModel("", modelNode, materials, GL_TRIANGLES);

	RenderObject* renderObject = getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(new RenderObject(_generatedModel), getSceneObject());

	/*for (int i = 0; i < _roads.size(); ++i)
	//for (int i = 0; i < 1; ++i)
	{
		int road1Index = i;
		int road2Index = (i + 1) % _roads.size();

		const auto& road1CurvePoints = _roads[road1Index].road->getCurvePoints();
		const auto& road2CurvePoints = _roads[road2Index].road->getCurvePoints();
		int road1CenterPointIndex = _roads[road1Index].connectionPointInRoadIndex == 0 ? 0 : road1CurvePoints.size() - 1;
		int road2CenterPointIndex = _roads[road2Index].connectionPointInRoadIndex == 0 ? 0 : road2CurvePoints.size() - 1;
		glm::vec3 road1CenterPoint = road1CurvePoints[road1CenterPointIndex];
		glm::vec3 road2CenterPoint = road2CurvePoints[road2CenterPointIndex];

		glm::vec2 direction1 = RoadGenerator::calculateDirection(road1CurvePoints, temp, road1CenterPointIndex);
		glm::vec2 rightVector1 = glm::vec2(-direction1.y, direction1.x);

		glm::vec2 direction2 = RoadGenerator::calculateDirection(road2CurvePoints, temp, road2CenterPointIndex);
		glm::vec2 rightVector2 = glm::vec2(-direction2.y, direction2.x);

		if (_roads[road1Index].connectionPointInRoadIndex == 0)
		{
			rightVector1 = -rightVector1;
		}
		if (_roads[road2Index].connectionPointInRoadIndex == 1)
		{
			rightVector2 = -rightVector2;
		}




		// krzywe beziera - krawêdzie
		if (_roads[road1Index].connectionPointInRoadIndex == 0)
		{
			direction1 = -direction1;
		}
		if (_roads[road2Index].connectionPointInRoadIndex == 0)
		{
			direction2 = -direction2;
		}
		std::vector<glm::vec3> bezierCurve1(4);
		bezierCurve1[0] = road1CenterPoint + glm::vec3(rightVector1.x, 0.0f, rightVector1.y) * _width;
		bezierCurve1[3] = road2CenterPoint + glm::vec3(rightVector2.x, 0.0f, rightVector2.y) * _width;
		bezierCurve1[1] = bezierCurve1[0] + glm::vec3(direction1.x, 0.0f, direction1.y) * _arc;
		bezierCurve1[2] = bezierCurve1[3] + glm::vec3(direction2.x, 0.0f, direction2.y) * _arc;

		std::vector<glm::vec3> bezierCurvePoints;
		BezierCurvesUtils::generateBezierCurvePoints(bezierCurve1[0], bezierCurve1[1], bezierCurve1[2], bezierCurve1[3], _quality, bezierCurvePoints);
		

		shapePolygonComponent->addPoint(centerPoint);
		shapePolygonComponent->addPoint(road1CenterPoint);
		//shapePolygonComponent->addPoint(road1CenterPoint + glm::vec3(rightVector1.x, 0.0f, rightVector1.y) * _width);
		//shapePolygonComponent->addPoint(road2CenterPoint + glm::vec3(rightVector2.x, 0.0f, rightVector2.y) * _width);

		for (int j = 0; j < bezierCurvePoints.size(); ++j)
		{
			shapePolygonComponent->addPoint(bezierCurvePoints[j]);
		}

		shapePolygonComponent->addPoint(road2CenterPoint);
	}*/

}


void RoadIntersectionComponent::needRebuildConnectedRoad()
{
	_needRebuildConnectedRoad = true;

	LOG_DEBUG("needRebuildConnectedRoad");
}


void RoadIntersectionComponent::changedTransform()
{
	for (const auto& connectedRoad : _roads)
	{
		connectedRoad.road->resetConnectedPointPositions();
	}
}


void RoadIntersectionComponent::update(float deltaTime)
{
	static float roadModificationTimer = 0.0f;
	roadModificationTimer += deltaTime;

	if (roadModificationTimer >= 0.2f && _needRebuildConnectedRoad)
	{
		LOG_DEBUG("BUILD ROAD: BEGIN");
		for (int i = 0; i < _roads.size(); ++i)
		{
			_roads[i].road->buildModel();
		}

		LOG_DEBUG("BUILD ROAD: END");
		roadModificationTimer = 0.0f;
		_needRebuildConnectedRoad = false;
	}
}
