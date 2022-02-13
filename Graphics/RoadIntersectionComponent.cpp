#include "RoadIntersectionComponent.h"

#include <iterator>

#include "ModelGenerator.h"
#include "ShapePolygonComponent.h"
#include "RoadGenerator.h"
#include "RoadObject.h"

#include "../Scene/SceneObject.h"
#include "../Scene/SceneManager.h"

#include "../Utils/BezierCurvesUtils.h"
#include "../Utils/ResourceManager.h"


struct RoadWithAngle
{
	int roadIndex;
	float roadAgle;
};


RoadIntersectionComponent::RoadIntersectionComponent(RRoadProfile* edgeRadProfile)
	: Component(CT_ROAD_INTERSECTION),
	_quality(11),
	//_length(10.0f), _width(5.0f), _arc(2.0f), _quality(11),
	_originalEdgeRoadProfile(nullptr), _edgeRoadProfile(nullptr), _edgeRoadProfileNumberOfLanesToRemove(1),
	_needRebuildIntersectionModel(false), _needRebuildConnectedRoad(false), _needSortRoads(false), _needRecreateModel(false), _modificationTimer(0.0f),
	_inverseModelMatrix(1.0f), _inverseModelMatrixIsCalculated(false),
	_generatedModel(nullptr),
	_isCreated(true)
{
	setEdgeRoadProfile(edgeRadProfile);
}


RoadIntersectionComponent::~RoadIntersectionComponent()
{
	_isCreated = false;

	for (const auto& connectRoad : _roads)
	{
		connectRoad.road->setConnectionPoint(connectRoad.connectionPointInRoadIndex, nullptr);
	}

	delete _edgeRoadProfile;
}


void RoadIntersectionComponent::connectRoad(RoadObject* roadObject, int connectionPointInRoadIndex)
{
	if (!_isCreated)
	{
		return;
	}

	_roads.push_back(RoadConnectedToIntersection(roadObject, connectionPointInRoadIndex));

	if (_roads.size() == 1)
	{
		_length.push_back(0.0f);
		_width.push_back(0.0f);
		_arc.push_back(2.0f);
	}
	else
	{
		_length.push_back(_roads.size() == 2 && _length[0] == 0.0f ? 10.0f : _length[0]);
		_width.push_back(_width[0]);
		_arc.push_back(_arc[0]);

		needRebuildConnectedRoad();
	}

	setLengthInternal(_roads.size() - 1, _length[_length.size() - 1]);

	if (_roads.size() == 2 && _length[0] == 0.0f)
	{
		setLengthInternal(0, _length[1]);
	}

	SceneObject* roadSceneObject = getSceneObject()->getSceneManager()->addSceneObject("intersectionEdge");
	getSceneObject()->addChild(roadSceneObject);
	getSceneObject()->getSceneManager()->getGraphicsManager()->addRoadObject(RoadType::POINTS, _edgeRoadProfile, {}, {}, false, roadSceneObject);
	roadSceneObject->setIsActive(false);

	_needSortRoads = true;
}


void RoadIntersectionComponent::disconnectRoad(RoadObject* roadObject, int connectionPointInRoadIndex)
{
	if (!_isCreated)
	{
		return;
	}

	int index = 0;
	for (std::vector<RoadConnectedToIntersection>::iterator i = _roads.begin(); i != _roads.end(); ++i)
	{
		if (i->road == roadObject && i->connectionPointInRoadIndex == connectionPointInRoadIndex)
		{
			i = _roads.erase(i);

			_length.erase(_length.begin() + index);
			_width.erase(_width.begin() + index);
			_arc.erase(_arc.begin() + index);

			auto objectToRemoveIterator = std::next(getSceneObject()->getChildren().begin(), index);
			if (GameConfig::getInstance().mode == GM_EDITOR) ++objectToRemoveIterator;
			getSceneObject()->getChildren().erase(objectToRemoveIterator);

			break;
		}

		++index;
	}

	needRebuildConnectedRoad();
}


const std::vector<RoadConnectedToIntersection>& RoadIntersectionComponent::getConnectedRoads()
{
	return _roads;
}


void RoadIntersectionComponent::setLengthInternal(int index, float length)
{
	_length[index] = length;

	if (_roads[index].connectionPointInRoadIndex == 0)
	{
		_roads[index].road->setMarginBegin(length);
	}
	else
	{
		_roads[index].road->setMarginEnd(length);
	}
}


void RoadIntersectionComponent::setLength(float length)
{
	for (int i = 0; i < _length.size(); ++i)
	{
		setLengthInternal(i, length);
	}

	needRebuildConnectedRoad();
}


void RoadIntersectionComponent::setLength(int index, float length, bool modifyAll)
{
	if (modifyAll)
	{
		setLength(length);
		return;
	}

	if (index >= _length.size())
	{
		LOG_ERROR("Error while calling setLength. Invalid road index!");
		return;
	}

	setLengthInternal(index, length);

	needRebuildConnectedRoad();
}


void RoadIntersectionComponent::setWidth(float width)
{
	for (int i = 0; i < _width.size(); ++i)
	{
		_width[i] = width;
	}

	_needRebuildIntersectionModel = true;
}


void RoadIntersectionComponent::setWidth(int index, float width, bool modifyAll)
{
	if (modifyAll)
	{
		setWidth(width);
		return;
	}

	if (index >= _length.size())
	{
		LOG_ERROR("Error while calling setWidth. Invalid road index!");
		return;
	}
	_width[index] = width;

	_needRebuildIntersectionModel = true;
}


void RoadIntersectionComponent::setArc(float arc)
{
	for (int i = 0; i < _arc.size(); ++i)
	{
		_arc[i] = arc;
	}

	_needRebuildIntersectionModel = true;
}


void RoadIntersectionComponent::setArc(int index, float arc, bool modifyAll)
{
	if (modifyAll)
	{
		setArc(arc);
		return;
	}

	if (index >= _length.size())
	{
		LOG_ERROR("Error while calling setArc. Invalid road index!");
		return;
	}
	_arc[index] = arc;

	_needRebuildIntersectionModel = true;
}


void RoadIntersectionComponent::setQuality(int quality)
{
	_quality = quality;

	_needRebuildIntersectionModel = true;
}


float RoadIntersectionComponent::getLength(int index)
{
	if (index >= _length.size())
	{
		LOG_ERROR("Error while calling getLength. Invalid road index!");
		return 0.0f;
	}
	return _length[index];
}


float RoadIntersectionComponent::getWidth(int index)
{
	if (index >= _width.size())
	{
		LOG_ERROR("Error while calling getWidth. Invalid road index!");
		return 0.0f;
	}
	return _width[index];
}


float RoadIntersectionComponent::getArc(int index)
{
	if (index >= _arc.size())
	{
		LOG_ERROR("Error while calling getArc. Invalid road index!");
		return 0.0f;
	}
	return _arc[index];
}


int RoadIntersectionComponent::getQuality()
{
	return _quality;
}


void RoadIntersectionComponent::setEdgeRoadProfile(RRoadProfile* roadProfile)
{
	_originalEdgeRoadProfile = roadProfile;

	updateEdgeRoadProfile();
}


void RoadIntersectionComponent::updateEdgeRoadProfile()
{
	if (_edgeRoadProfile != nullptr)
	{
		delete _edgeRoadProfile;
	}

	// copy of original road profile;
	_edgeRoadProfile = new RRoadProfile(_originalEdgeRoadProfile->getPath(), _originalEdgeRoadProfile->getAuthor(), _originalEdgeRoadProfile->getName(), _originalEdgeRoadProfile->getComment(),
										_originalEdgeRoadProfile->getIntersectionMaterial(), _originalEdgeRoadProfile->getIntersectionRoadY());

	const std::vector<RoadLane>& roadLanes = _originalEdgeRoadProfile->getRoadLanes();
	int numberOfRemovedLane = 0;
	float offset = 0.0f;

	for (int i = 0; i < roadLanes.size(); ++i)
	{
		if (roadLanes[i].r1 < 0.0f && roadLanes[i].r2 < 0.0f)
		{
			continue;
		}
		else if (numberOfRemovedLane < _edgeRoadProfileNumberOfLanesToRemove)
		{
			++numberOfRemovedLane;
			continue;
		}

		if (offset == 0.0f)
		{
			offset = roadLanes[i].r1;
		}

		RoadLane newLane = roadLanes[i];
		newLane.r1 = newLane.r1 - offset;
		newLane.r2 = newLane.r2 - offset;
		_edgeRoadProfile->getRoadLanes().push_back(newLane);
	}

	if (getSceneObject() != nullptr)
	{
		for (auto childObject : getSceneObject()->getChildren())
		{
			RoadObject* roadObject = dynamic_cast<RoadObject*>(childObject->getComponent(CT_ROAD_OBJECT));
			if (roadObject != nullptr)
			{
				roadObject->setRoadProfile(_edgeRoadProfile);
			}
		}

		_needRebuildIntersectionModel = true;
		_needRecreateModel = true;
	}
}


void RoadIntersectionComponent::setEdgeRoadProfileNumberOfLanesToRemove(unsigned int edgeRoadProfileNumberOfLanesToRemove)
{
	_edgeRoadProfileNumberOfLanesToRemove = edgeRoadProfileNumberOfLanesToRemove;

	updateEdgeRoadProfile();
}


RRoadProfile* RoadIntersectionComponent::getEdgeRoadProfile()
{
	return _edgeRoadProfile;
}


unsigned int RoadIntersectionComponent::getEdgeRoadProfileNumberOfLanesToRemove()
{
	return _edgeRoadProfileNumberOfLanesToRemove;
}


void RoadIntersectionComponent::onAttachedToScenObject()
{
	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SceneObject* helperSceneObject = getSceneObject()->getSceneManager()->addSceneObject("editor#Road intersection helper");

		Material* material = new Material;
		material->shader = NOTEXTURE_MATERIAL;
		material->shininess = 96.0f;
		material->diffuseColor = glm::vec4(0.32f, 0.32f, 0.32f, 1.0f);

		Cube* cube = new Cube(1, material);
		cube->init();
		cube->setIsCastShadows(false);
		getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(cube, helperSceneObject);

		getSceneObject()->addChild(helperSceneObject);
	}
}


void RoadIntersectionComponent::createDebugPolygonComponent(const std::vector<std::vector<glm::vec3>>& pointsOnRoadAxis, const std::vector<std::vector<glm::vec3>>& bezierCurves)
{
	auto shapePolygonComponent = new ShapePolygonComponent;
	getSceneObject()->removeComponent(getSceneObject()->getComponent(CT_SHAPE_POLYGON));
	getSceneObject()->addComponent(shapePolygonComponent);

	glm::vec3 centerPoint = getSceneObject()->getGlobalPosition();

	for (int i = 0; i < _roads.size(); ++i)
	{
		int road1Index = i;
		int road2Index = (i + 1) % _roads.size();

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
	}
}


float RoadIntersectionComponent::getRealWidth(int index, bool isRight)
{
	float width = _width[index];
	float realWidth = width;
	if (width == 0.0f)
	{
		if (isRight && _roads[index].connectionPointInRoadIndex == 1 ||
			!isRight && _roads[index].connectionPointInRoadIndex == 0)
			realWidth = _roads[index].road->getRoadProfile()->getMaxX();
		else
			realWidth = -_roads[index].road->getRoadProfile()->getMinX();
	}

	return realWidth - _edgeRoadProfile->getMaxX();
}


void RoadIntersectionComponent::sortConnectedRoads()
{
	glm::vec3 centerPoint = getSceneObject()->getGlobalPosition();

	std::vector<RoadWithAngle> roadsAngles;
	for (int i = 0; i < _roads.size(); ++i)
	{
		const auto& roadCurvePoints = _roads[i].road->getCurvePoints();
		int road1CenterPointIndex = _roads[i].connectionPointInRoadIndex == 0 ? 0 : roadCurvePoints.size() - 1;
		glm::vec3 roadCenterPoint = roadCurvePoints[road1CenterPointIndex];

		glm::vec3 direction = glm::normalize(roadCenterPoint - centerPoint);

		float angle = radToDeg(atan2(direction.z, direction.x));
		roadsAngles.push_back({ i, angle });
	}


	std::sort(roadsAngles.begin(), roadsAngles.end(), [](const RoadWithAngle& r1, const RoadWithAngle& r2)
		{
			return r1.roadAgle > r2.roadAgle;
		});

	std::vector<RoadConnectedToIntersection> roads = _roads;
	std::vector<float> length = _length;
	std::vector<float> width = _width;
	std::vector<float> arc = _arc;

	for (int i = 0; i < roadsAngles.size(); ++i)
	{
		int roadIndex = roadsAngles[i].roadIndex;
		_roads[i] = roads[roadIndex];
		_length[i] = length[roadIndex];
		_width[i] = width[roadIndex];
		_arc[i] = arc[roadIndex];
	}
}


const glm::mat4& RoadIntersectionComponent::getInverseModelMatrix()
{
	if (!_inverseModelMatrixIsCalculated)
	{
		_inverseModelMatrix = glm::inverse(getSceneObject()->getGlobalTransformMatrix());

		_inverseModelMatrixIsCalculated = true;
	}

	return _inverseModelMatrix;
}


glm::vec3 RoadIntersectionComponent::transformPointToLocalSpace(const glm::vec3& point)
{
	glm::vec4 p = getInverseModelMatrix() * glm::vec4(point.x, point.y, point.z, 1.0f);
	return glm::vec3(p.x, p.y, p.z);
}


glm::vec3 RoadIntersectionComponent::transformVectorToLocalSpace(const glm::vec3& point)
{
	glm::vec4 p = getInverseModelMatrix() * glm::vec4(point.x, point.y, point.z, 0.0f);
	return glm::vec3(p.x, p.y, p.z);
}


void RoadIntersectionComponent::createPolygon()
{
	std::vector<RoadConnectionPointData*> temp(2, nullptr);

	glm::vec3 centerPoint = glm::vec3(0.0f, 0.0f, 0.0f);

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

		glm::vec3 roadCenterPoint = transformPointToLocalSpace(roadCurvePoints[road1CenterPointIndex]);
		glm::vec2 dir = RoadGenerator::calculateDirection(roadCurvePoints, temp, road1CenterPointIndex);
		glm::vec3 direction = transformVectorToLocalSpace(glm::vec3(dir.x, 0.0f, dir.y));
		glm::vec2 rightVector = glm::vec2(-direction.z, direction.x);

		// jesli punkt poczatkowy drogi to odwracamy direction i rightVector (traktujemy kazda droge jak wchodzaca do skrzyzowania, wiec jej kierunek powinien byc w strone srodka skrzyzowania)
		if (_roads[i].connectionPointInRoadIndex == 0)
		{
			direction = -direction;
			rightVector = -rightVector;
		}

		roadsCenterPoints[i] = roadCenterPoint + glm::vec3(0.0f, 0.0f, 0.0f);;
		roadsDirections[i] = glm::vec3(direction.x, 0.0f, direction.z);
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
		bezierCurve1[0] = roadsCenterPoints[road1Index] + roadsRightVectors[road1Index] * getRealWidth(road1Index, true);
		bezierCurve1[3] = roadsCenterPoints[road2Index] - roadsRightVectors[road2Index] * getRealWidth(road2Index, false);
		bezierCurve1[1] = bezierCurve1[0] + roadsDirections[road1Index] * _arc[road1Index];
		bezierCurve1[2] = bezierCurve1[3] + roadsDirections[road2Index] * _arc[road1Index];

		BezierCurvesUtils::generateBezierCurvePoints(bezierCurve1[0], bezierCurve1[1], bezierCurve1[2], bezierCurve1[3], _quality, bezierCurves[i]);
	}

	//createDebugPolygonComponent(pointsOnRoadAxis, bezierCurves);

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
			vertices[i * _quality + j].texCoord = glm::vec2(bezierCurves[i][j].x, bezierCurves[i][j].z) * _edgeRoadProfile->getIntersectionMaterial()->scale;
			vertices[i * _quality + j].normal = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		for (int j = 0; j < pointsOnRoadAxis[i].size(); ++j)
		{
			vertices[numberOfPointsInAllBezierCurves + i * numberOfPointsOnRoadAxis + j].position = pointsOnRoadAxis[i][j];
			vertices[numberOfPointsInAllBezierCurves + i * numberOfPointsOnRoadAxis + j].texCoord = glm::vec2(pointsOnRoadAxis[i][j].x, pointsOnRoadAxis[i][j].z) * _edgeRoadProfile->getIntersectionMaterial()->scale;
			vertices[numberOfPointsInAllBezierCurves + i * numberOfPointsOnRoadAxis + j].normal = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	vertices[numberOfVertices - 1].position = centerPoint;

	// indices
	unsigned int numberOfIndices = _roads.size() * _quality * 2 + _roads.size();
	unsigned int* indices = new unsigned int[numberOfIndices];

	LOG_DEBUG(LOG_VARIABLE(numberOfIndices));

	int index = 0;
	for (int i = 0; i < _roads.size(); ++i)
	{
		int i2 = (i + 1) % _roads.size();
		for (int j = 0; j < _quality; ++j)
		{
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

			indices[index++] = (i * _quality) + j;

			if (j > 0)
			{
				unsigned int i0 = indices[index - 4];
				unsigned int i1 = indices[index - 3];
				unsigned int i2 = indices[index - 2];
				unsigned int i3 = indices[index - 1];

				glm::vec3 normal1 = glm::cross(glm::normalize(vertices[i1].position - vertices[i0].position), glm::normalize(vertices[i2].position - vertices[i0].position));
				glm::vec3 normal2 = glm::cross(glm::normalize(vertices[i2].position - vertices[i3].position), glm::normalize(vertices[i1].position - vertices[i3].position));

				vertices[i0].normal += normal1;		vertices[i0].normal /= 2.0f;
				vertices[i1].normal += normal1;		vertices[i1].normal /= 2.0f;
				vertices[i2].normal += normal1;		vertices[i2].normal /= 2.0f;

				vertices[i1].normal += normal1;		vertices[i1].normal /= 2.0f;
				vertices[i2].normal += normal1;		vertices[i2].normal /= 2.0f;
				vertices[i3].normal += normal1;		vertices[i3].normal /= 2.0f;
			}
		}
		indices[index++] = OGLDriver::PRIMITIVE_RESTART_INDEX;
	}

	LOG_DEBUG(LOG_VARIABLE(index));

	if (_needRecreateModel)
	{
		delete _generatedModel;
		_generatedModel = nullptr;

		getSceneObject()->removeComponent(getSceneObject()->getComponent(CT_RENDER_OBJECT));
	}

	// meshes
	const bool isModelExist = _generatedModel != nullptr;
	const bool isGame = GameConfig::getInstance().mode == GM_GAME;

	StaticModelMesh* meshes = isModelExist ? _generatedModel->getRootNode()->meshes : new StaticModelMesh[1];
	meshes[0].setMeshData(vertices, numberOfVertices, indices, numberOfIndices, 0, SOLID_MATERIAL, isGame, DEFAULT_BUFFER_SIZE, DEFAULT_BUFFER_SIZE, false);

	// model
	if (isModelExist)
	{
		_generatedModel->recalculateAABB();

		RenderObject* currentRenderComponent = static_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
		currentRenderComponent->setModel(_generatedModel);
	}
	else
	{
		std::vector<Material*> materials;
		materials.push_back(new Material(*(_edgeRoadProfile->getIntersectionMaterial())));

		StaticModelNode* modelNode = new StaticModelNode;
		modelNode->name = "road intersection";
		modelNode->meshes = meshes;
		modelNode->meshesCount = 1;
		modelNode->parent = nullptr;

		_generatedModel = new RStaticModel("", modelNode, materials, GL_TRIANGLE_STRIP);

		RenderObject* renderObject = getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(new RenderObject(_generatedModel), getSceneObject());
	}

	{
		int index = 0;
		for (auto child : getSceneObject()->getChildren())
		{
			// update existing children
			RoadObject* roadObject = dynamic_cast<RoadObject*>(child->getComponent(CT_ROAD_OBJECT));
			if (roadObject != nullptr)
			{
				auto& roadObjectPoints = roadObject->getPoints();
				roadObjectPoints.resize(bezierCurves[index].size());

				for (int i = 0; i < bezierCurves[index].size(); ++i)
				{
					roadObjectPoints[i] = bezierCurves[index][i];
				}

				roadObject->setCustomConnectionPointData(0, glm::vec3(0.0f, 0.0f, 0.0f), roadsDirections[index]);
				roadObject->setCustomConnectionPointData(1, glm::vec3(0.0f, 0.0f, 0.0f), roadsDirections[(index + 1) % _roads.size()]);
				roadObject->buildModel(!_needRecreateModel);

				child->setIsActive(true);

				++index;
			}
		}

		_needRecreateModel = false;
	}
}


RStaticModel* RoadIntersectionComponent::getModel()
{
	return _generatedModel;
}


void RoadIntersectionComponent::needRebuild()
{
	_needRebuildIntersectionModel = true;
	_needSortRoads = true;

	LOG_DEBUG("needRebuild");
}


void RoadIntersectionComponent::needRebuildConnectedRoad()
{
	/*for (int i = 0; i < _roads.size(); ++i)
	{
		_roads[i].road->needRebuildRoad();
	}*/

	_needRebuildIntersectionModel = true;
	_needRebuildConnectedRoad = true;

	LOG_DEBUG("needRebuildConnectedRoad");
}


void RoadIntersectionComponent::changedTransform()
{
	for (const auto& connectedRoad : _roads)
	{
		connectedRoad.road->resetConnectedPointPositions();
	}

	_inverseModelMatrixIsCalculated = false;
}


void RoadIntersectionComponent::update(float deltaTime)
{
	if (GameConfig::getInstance().mode == GM_GAME)
	{
		return;
	}

	_modificationTimer += deltaTime;

	if (_modificationTimer >= 0.2f)
	{
		if (_needRebuildConnectedRoad)
		{
			LOG_DEBUG("BUILD ROAD: BEGIN");
			for (int i = 0; i < _roads.size(); ++i)
			{
				_roads[i].road->buildModel();
			}

			LOG_DEBUG("BUILD ROAD: END");

			_needRebuildConnectedRoad = false;
		}
		if (_needSortRoads)
		{
			sortConnectedRoads();

			_needSortRoads = false;
		}
		if (_needRebuildIntersectionModel)
		{
			createPolygon();

			_needRebuildIntersectionModel = false;
		}
		_modificationTimer = 0.0f;
	}
}
