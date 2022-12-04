#include "SceneLoader.h"

#include "SceneManager.h"

#include "../Game/AIAgent.h"
#include "../Game/Directories.h"
#include "../Game/GameLogicSystem.h"
#include "../Game/PathComponent.h"

#include "../Graphics/BezierCurve.h"
#include "../Graphics/CrossroadComponent.h"
#include "../Graphics/LoadMaterial.h"
#include "../Graphics/LoadTerrainModel.h"

#include "../Utils/Logger.h"
#include "../Utils/RObject.h"
#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"

#include "../Utils/tinyxml2.h"
using namespace tinyxml2;


SceneLoader::SceneLoader(SceneManager* sceneManager)
	: _sceneManager(sceneManager),
	_terrainCollidesWith(COL_WHEEL | COL_BUS | COL_ENV | COL_DOOR), _roadCollidesWith(COL_WHEEL | COL_BUS | COL_ENV | COL_DOOR)
{

}


void SceneLoader::loadStartPosition(XMLElement* sceneElement)
{
	XMLElement* startElement = sceneElement->FirstChildElement("Start");
	if (startElement == nullptr)
	{
		LOG_WARNING("Start point not found!");
	}
	else
	{
		const char* cPosition(startElement->Attribute("position"));
		_sceneManager->getBusStart().position = XMLstringToVec3(cPosition);

		const char* cRotation(startElement->Attribute("rotation"));
		_sceneManager->getBusStart().rotation = XMLstringToVec3(cRotation);
	}
}


bool SceneLoader::loadTerrain(XMLElement* sceneElement)
{
	XMLElement* terrElement = sceneElement->FirstChildElement("Terrain");
	if (terrElement == nullptr)
	{
		LOG_ERROR("Terrain element not found!");
		return false;
	}

	std::string terrainHeightmap(terrElement->Attribute("heightmap"));
	std::string materialName(terrElement->Attribute("material"));
	float terrainMaxHeight = XmlUtils::getAttributeFloatOptional(terrElement, "maxHeight", 20.0f);
	bool is16bitTexture = XmlUtils::getAttributeBoolOptional(terrElement, "is16bitTexture");

	LOG_INFO("*** TERRAIN DATA ***");
	LOG_INFO("Heightmap: " + terrainHeightmap);
	LOG_INFO("Material: " + materialName);

	SceneObject* terrainObject = _sceneManager->addSceneObject("terrain");
	terrainObject->setFlags(SOF_NOT_SELECTABLE_ON_SCENE);
	Terrain* terrainObj = _sceneManager->getGraphicsManager()->addTerrain(terrainHeightmap, _dirPath, materialName, terrainMaxHeight, is16bitTexture, terrainObject);
	terrainObj->setCastShadows(false);
	PhysicalBodyBvtTriangleMesh* terrainMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(terrainObj->getModel(), COL_TERRAIN, _terrainCollidesWith);
	terrainMesh->setRestitution(0.9f);
	terrainMesh->getRigidBody()->setFriction(1.0f);
	terrainObject->addComponent(terrainMesh);

	terrainMesh->setVisualisationInDebugDrawingStatus(false);

	return true;
}


void SceneLoader::loadGrass(XMLElement* grassElement)
{
	//XMLElement* grassElement = grassElement->FirstChildElement("Grass");
	/*if (grassElement == nullptr)
	{
		LOG_WARNING("Grass element not found!");
	}*/


	if (GameConfig::getInstance().isGrassEnable)
	{
		std::string grassModelFileName(grassElement->Attribute("model"));
		std::string terrainHeightmapForGrassFileName(grassElement->Attribute("terrain_heightmap"));
		std::string grassDensityTextureFileName(grassElement->Attribute("density_texture"));
		glm::vec3 grassColor = XmlUtils::getAttributeVec3Optional(grassElement, "color", glm::vec3(1.0f, 1.0f, 1.0f));
		float renderingDistance = GameConfig::getInstance().grassRenderingDistance;

		std::string terrainHeightNormalMapFileName = TerrainLoader::createTerrainHeightAndNormalMapFileName(terrainHeightmapForGrassFileName);
		if (FilesHelper::isFileExists(_dirPath + terrainHeightNormalMapFileName))
		{
			terrainHeightmapForGrassFileName = terrainHeightNormalMapFileName;
		}

		LOG_INFO("*** GRASS DATA ***");
		LOG_INFO("Model: " + grassModelFileName);
		LOG_INFO("heightmap: " + terrainHeightmapForGrassFileName);
		LOG_INFO("density texture: " + grassDensityTextureFileName);

		RStaticModel* grassModel = ResourceManager::getInstance().loadModel(_dirPath + "grass/" + grassModelFileName, _dirPath + "grass/");

		RTexture2D * heightmapTextureForGrass = ResourceManager::getInstance().loadTexture(_dirPath + terrainHeightmapForGrassFileName, false);
		RTexture2D * grassDensityTexture = ResourceManager::getInstance().loadTexture(_dirPath + grassDensityTextureFileName, false);
		heightmapTextureForGrass->setClampMode(TCM_CLAMP_TO_EDGE);
		heightmapTextureForGrass->setFiltering(TFM_LINEAR, TFM_LINEAR);

		SceneObject * grassObject = _sceneManager->addSceneObject("grass");
		Grass * grassComponent = _sceneManager->getGraphicsManager()->addGrassComponent(grassModel, heightmapTextureForGrass, grassDensityTexture);
		grassComponent->setRenderingDistance(renderingDistance);
		grassComponent->setGrassColor(glm::vec4(grassColor.x, grassColor.y, grassColor.z, 1.0f));
		grassObject->addComponent(grassComponent);


		XMLElement* grassTextureElement = grassElement->FirstChildElement("Texture");
		while (grassTextureElement != nullptr)
		{
			std::string textureName(grassTextureElement->Attribute("path"));
			float scale = atof(grassTextureElement->Attribute("scale"));

			grassComponent->getAdditionalRandomGrassTextures().push_back(ResourceManager::getInstance().loadTexture(_dirPath + "grass/" + textureName, false));
			grassComponent->getAdditionalRandomGrassTexturesScale().push_back(scale);

			grassTextureElement = grassTextureElement->NextSiblingElement("Texture");
		}
	}
	else
		LOG_WARNING("Grass element not found!");
}


void SceneLoader::loadSunLight(XMLElement* sunElement)
{
	//XMLElement* lightElement = sceneElement->FirstChildElement("Light");
	//if (lightElement == nullptr)
	//{
	//	LOG_WARNING("Light element not found!");
	//}
	//else
	//{
		glm::vec3 rotation = XmlUtils::getAttributeVec3(sunElement, "rotation");
		glm::vec3 color = XmlUtils::getAttributeVec3(sunElement, "color");
		float ambientIntensity = atof(sunElement->Attribute("ambientIntensity"));
		float diffuseIntensity = atof(sunElement->Attribute("diffuseIntensity"));

		SceneObject* dirLightObject = _sceneManager->addSceneObject("sun");
		Light* lightComponent = _sceneManager->getGraphicsManager()->addDirectionalLight(color, ambientIntensity, diffuseIntensity);
		dirLightObject->addComponent(lightComponent);
		dirLightObject->setRotation(rotation);
		dirLightObject->setPosition(glm::vec3(0, 0, 0));
		lightComponent->setShadowMapping(GameConfig::getInstance().isShadowmappingEnable);
	//}
}


void SceneLoader::loadSky(XMLElement* skyElement)
{
	//XMLElement* skyElement = sceneElement->FirstChildElement("Sky");
	//if (skyElement)
	//{
		std::string skyboxFileNames;

		if (skyElement->Attribute("texture"))
			skyboxFileNames = std::string(skyElement->Attribute("texture"));

		std::vector<std::string> skyboxFileNamesArray;
		if (!skyboxFileNames.empty())
			skyboxFileNamesArray = split(skyboxFileNames, ',');

		if (skyboxFileNamesArray.size() == 6)
		{
			for (int i = 0; i < skyboxFileNamesArray.size(); ++i)
			{
				skyboxFileNamesArray[i] = GameDirectories::SKYBOX + skyboxFileNamesArray[i];
			}

			SceneObject* skySceneObject = _sceneManager->addSceneObject("sky");

			RTextureCubeMap* skyboxTexture = ResourceManager::getInstance().loadTextureCubeMap(&skyboxFileNamesArray[0]);
			Sky* skyComponent = _sceneManager->getGraphicsManager()->addSky(skyboxTexture, skySceneObject);

			_sceneManager->getGraphicsManager()->addGlobalEnvironmentCaptureComponent(skyboxTexture);

		}
		else
			LOG_ERROR("Number of skybox files is invalid!");
	//}
	//else
	//{
	//	LOG_ERROR("Sky element not found!");
	//	return false;
	//}
}


void SceneLoader::loadObjects(XMLElement* objectsElement, SceneObject* parent)
{
	XMLElement* objectElement = objectsElement->FirstChildElement("Object");
	if (objectElement)
		loadObject(objectElement, parent);

	//XMLElement* objects = sceneElement->FirstChildElement("Objects");

	//XMLElement* objectElement = objectsElement->FirstChildElement("Object");
	/*
	while (objectElement != nullptr)
	{
		std::string name(objectElement->Attribute("name"));
		std::string objectName = XmlUtils::getAttributeStringOptional(objectElement, "id", name);
		glm::vec3 position = XmlUtils::getAttributeVec3(objectElement, "position");
		glm::vec3 rotation = XmlUtils::getAttributeVec3(objectElement, "rotation");

		LOG_INFO("==> SCENE OBJECT");
		LOG_INFO("Name: " + name);
		LOG_INFO("Position: " + vec3ToString(position));
		LOG_INFO("Rotation: " + vec3ToString(rotation));

		RObject* rObject = ResourceManager::getInstance().loadRObject(name);

		SceneObject* sceneObject = RObjectLoader::createSceneObjectFromRObject(rObject, objectName, position, rotation, _sceneManager);

		// Components properties defined per instance
		XMLElement* componentElement = objectElement->FirstChildElement("Component");
		while (componentElement != nullptr)
		{
			std::string componentType = componentElement->Attribute("type");
			std::cout << "Component: " << componentType << std::endl;

			if (componentType == "bus-stop")
			{
				std::string busStopName = componentElement->Attribute("busStopName");
				int busStopId = atoi(componentElement->Attribute("id"));
				std::string announcementFileName = componentElement->Attribute("announcement");

				BusStopComponent* component = static_cast<BusStopComponent*>(sceneObject->getComponent(CT_BUS_STOP));
				component->setName(busStopName);
				component->setId(busStopId);
				component->setAnnouncementFileName(announcementFileName);
			}

			componentElement = componentElement->NextSiblingElement("Component");
		}


		objectElement = objectElement->NextSiblingElement("Object");
	}
	*/
}


void SceneLoader::loadPrefabComponent(XMLElement* componentElement, SceneObject* sceneObject)
{
	PrefabType prefabType = getPrefabTypeFromString(XmlUtils::getAttributeString(componentElement, "prefabType"));

	Material* material = new Material;
	material->shader = SOLID_MATERIAL;
	material->shininess = 96.0f;
	material->diffuseTexture = ResourceManager::getInstance().loadDefaultWhiteTexture();

	if (prefabType == PrefabType::PLANE)
	{
		glm::vec2 size = XmlUtils::getAttributeVec2(componentElement, "size");

		PlanePrefab* plane = new PlanePrefab(size, material);
		plane->init();
		_sceneManager->getGraphicsManager()->addRenderObject(plane, sceneObject);
	}
	else if (prefabType == PrefabType::CUBE)
	{
		float size = XmlUtils::getAttributeFloat(componentElement, "size");

		Cube* cube = new Cube(size, material);
		cube->init();
		_sceneManager->getGraphicsManager()->addRenderObject(cube, sceneObject);
	}
	else if (prefabType == PrefabType::SPHERE)
	{
		float radius = XmlUtils::getAttributeFloat(componentElement, "radius");
		int quality = XmlUtils::getAttributeInt(componentElement, "quality");

		SpherePrefab* sphere = new SpherePrefab(radius, material);
		sphere->setQuality(quality);
		sphere->init();
		_sceneManager->getGraphicsManager()->addRenderObject(sphere, sceneObject);
	}
	else if (prefabType == PrefabType::CYLINDER)
	{
		float radius = XmlUtils::getAttributeFloat(componentElement, "radius");
		float height = XmlUtils::getAttributeFloat(componentElement, "height");
		int axis = XmlUtils::getAttributeInt(componentElement, "axis");
		int quality = XmlUtils::getAttributeInt(componentElement, "quality");

		CylinderPrefab* cylinder = new CylinderPrefab(radius, height, material);
		cylinder->setAxis(axis);
		cylinder->setQuality(quality);
		cylinder->init();
		_sceneManager->getGraphicsManager()->addRenderObject(cylinder, sceneObject);
	}
	else
	{
		delete material;
	}
}


void SceneLoader::loadPathComponent(XMLElement* componentElement, SceneObject* sceneObject)
{
	std::vector<glm::vec3> bezierCurveControlPoints;

	XMLElement* pointsElement = componentElement->FirstChildElement("Points");

	XMLElement* pointElement = pointsElement->FirstChildElement("Point");
	while (pointElement != nullptr)
	{
		bezierCurveControlPoints.push_back(XMLstringToVec3(pointElement->GetText()));

		pointElement = pointElement->NextSiblingElement("Point");
	}

	glm::vec2 distanceFromBaseBezierCurve = XmlUtils::getAttributeVec2(componentElement, "distanceFromBaseBezierCurve");
	PathDirection direction = static_cast<PathDirection>(XmlUtils::getAttributeInt(componentElement, "direction"));
	float marginBegin = XmlUtils::getAttributeFloat(componentElement, "marginBegin");
	float marginEnd = XmlUtils::getAttributeFloat(componentElement, "marginEnd");

	PathComponent* pathComponent = _sceneManager->getGameLogicSystem()->addPathComponent(bezierCurveControlPoints, distanceFromBaseBezierCurve, direction, marginBegin, marginEnd);

	sceneObject->addComponent(pathComponent);
}


void SceneLoader::loadAIAgentComponent(XMLElement* componentElement, SceneObject* sceneObject)
{
	SceneObject* parent = sceneObject->getParent();
	if (parent != nullptr)
	{
		AIAgent* component = static_cast<AIAgent*>(sceneObject->getComponent(CT_AI_AGENT));

		PathComponent* pathComponent = static_cast<PathComponent*>(parent->getComponent(CT_PATH));
		if (pathComponent != nullptr)
		{
			component->setCurrentPath(pathComponent);
		}
	}
}


void SceneLoader::loadBezierCurveComponent(XMLElement* componentElement, SceneObject* sceneObject)
{
	std::vector<glm::vec3> controlPoints;
	std::vector<int> segmentsPointsCount;

	XMLElement* pointsElement = componentElement->FirstChildElement("Points");

	XMLElement* pointElement = pointsElement->FirstChildElement("Point");
	while (pointElement != nullptr)
	{
		controlPoints.push_back(XMLstringToVec3(pointElement->GetText()));

		pointElement = pointElement->NextSiblingElement("Point");
	}

	XMLElement* segmentsElement = componentElement->FirstChildElement("Segments");

	XMLElement* segmentElement = segmentsElement->FirstChildElement("Segment");
	while (segmentElement != nullptr)
	{
		segmentsPointsCount.push_back(XmlUtils::getAttributeInt(segmentElement, "points"));

		segmentElement = segmentElement->NextSiblingElement("Segment");
	}

	glm::vec2 offsetFromBaseCurve = XmlUtils::getAttributeVec2(componentElement, "offsetFromBaseCurve");
	float marginBegin = XmlUtils::getAttributeFloat(componentElement, "marginBegin");
	float marginEnd = XmlUtils::getAttributeFloat(componentElement, "marginEnd");

	BezierCurve* bezierCurve = _sceneManager->getGraphicsManager()->addBezierCurve(controlPoints, segmentsPointsCount, marginBegin, marginEnd, offsetFromBaseCurve);

	sceneObject->addComponent(bezierCurve);
}


void SceneLoader::loadObject(XMLElement* objectElement, SceneObject* parent)
{
	while (objectElement != nullptr)
	{
		std::string name(objectElement->Attribute("name"));
		std::string objectName = XmlUtils::getAttributeStringOptional(objectElement, "id", name);
		glm::vec3 position = XmlUtils::getAttributeVec3(objectElement, "position");
		glm::vec3 rotation = XmlUtils::getAttributeVec3(objectElement, "rotation");

		LOG_INFO("==> SCENE OBJECT");
		LOG_INFO("Name: " + name);
		LOG_INFO("Position: " + Strings::toString(position));
		LOG_INFO("Rotation: " + Strings::toString(rotation));

		SceneObject* sceneObject;
		if (!name.empty())
		{
			RObject* rObject = ResourceManager::getInstance().loadRObject(name);

			sceneObject = RObjectLoader::createSceneObjectFromRObject(rObject, objectName, position, rotation, _sceneManager);
		}
		else
		{
			sceneObject = _sceneManager->addSceneObject(objectName);
			sceneObject->setPosition(position);
			sceneObject->setRotation(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z));
		}

		if (parent != nullptr)
		{
			parent->addChild(sceneObject);
		}

		// Components properties defined per instance
		XMLElement* componentElement = objectElement->FirstChildElement("Component");
		while (componentElement != nullptr)
		{
			std::string componentType = componentElement->Attribute("type");
			LOG_INFO("Component: " + componentType);

			if (componentType == "bus-stop")
			{
				std::string busStopName = componentElement->Attribute("busStopName");
				int busStopId = atoi(componentElement->Attribute("id"));
				std::string announcementFileName = componentElement->Attribute("announcement");

				BusStopComponent* component = static_cast<BusStopComponent*>(sceneObject->getComponent(CT_BUS_STOP));
				component->setName(busStopName);
				component->setId(busStopId);
				component->setAnnouncementFileName(announcementFileName);
			}

			if (componentType == "prefab")
			{
				loadPrefabComponent(componentElement, sceneObject);
			}

			if (componentType == "path")
			{
				loadPathComponent(componentElement, sceneObject);
			}

			if (componentType == "aiAgent")
			{
				loadAIAgentComponent(componentElement, sceneObject);
			}

			if (componentType == "bezierCurve")
			{
				loadBezierCurveComponent(componentElement, sceneObject);
			}

			componentElement = componentElement->NextSiblingElement("Component");
		}

		// children
		loadObjects(objectElement, sceneObject);

		objectElement = objectElement->NextSiblingElement("Object");
	}
}

void SceneLoader::loadRoads(XMLElement* roadsElement)
{
	//XMLElement* roads = sceneElement->FirstChildElement("Roads");

	int version = XmlUtils::getAttributeIntOptional(roadsElement, "version", 1);

	XMLElement* roadElement = roadsElement->FirstChildElement("Road");

	if (roadElement)
	{
		if (version == 1)
			loadRoad(roadElement);
		else if (version == 2)
			loadRoadV2(roadElement);
	}

	XMLElement* roadIntersectionElement = roadsElement->FirstChildElement("RoadIntersection");
	if (roadIntersectionElement)
	{
		loadRoadIntersection(roadIntersectionElement);
	}

	const auto& roadObjects = _sceneManager->getGraphicsManager()->getRoadObjects();
	for (const auto& roadObject : roadObjects)
	{
		if (roadObject->getSceneObject()->getParent() == nullptr)
		{
			roadObject->buildModel();

			if (GameConfig::getInstance().mode == GM_EDITOR)
			{
				roadObject->setInteractiveMode(true);
			}

			PhysicalBodyBvtTriangleMesh* roadMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(roadObject->getModel(), COL_TERRAIN, _roadCollidesWith);
			roadMesh->setRestitution(0.9f);
			roadMesh->getRigidBody()->setFriction(1.0f);
			//terrainMesh->getRigidBody()->setFriction(1.5f);
			roadObject->getSceneObject()->addComponent(roadMesh);
		}
	}
	const auto& roadIntersections = _sceneManager->getGraphicsManager()->getRoadIntersectionComponents();
	for (const auto& roadIntersection : roadIntersections)
	{
		//roadIntersection->sortConnectedRoads();
		roadIntersection->createPolygon();
		if (GameConfig::getInstance().mode == GM_EDITOR)
		{
			roadIntersection->setInteractiveMode(true);
		}

		std::list<RStaticModel*> models;
		models.push_back(roadIntersection->getModel());
		for (auto child : roadIntersection->getSceneObject()->getChildren())
		{
			RoadObject* childRoadObject = dynamic_cast<RoadObject*>(child->getComponent(CT_ROAD_OBJECT));
			if (childRoadObject != nullptr)
			{
				models.push_back(childRoadObject->getModel());
			}
		}

		PhysicalBodyBvtTriangleMesh* roadIntersectionMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(models, COL_TERRAIN, _roadCollidesWith);
		roadIntersectionMesh->setRestitution(0.9f);
		roadIntersectionMesh->getRigidBody()->setFriction(1.0f);
		//terrainMesh->getRigidBody()->setFriction(1.5f);
		roadIntersection->getSceneObject()->addComponent(roadIntersectionMesh);
	}

	/*
	while (roadElement != nullptr)
	{
		std::string name = roadElement->Attribute("name");
		std::string profileName = roadElement->Attribute("profile");

		LOG_INFO("*** ROAD ***");
		LOG_INFO("profile name: " + profileName);

		RRoadProfile* roadProfile = ResourceManager::getInstance().loadRoadProfile(profileName);


		std::vector<RoadSegment> segments;

		XMLElement* segmentElement = roadElement->FirstChildElement("Segment");
		while (segmentElement != nullptr)
		{
			RoadSegment segment;

			const char* type = segmentElement->Attribute("type");
			if (strcmp(type, "arc") == 0)
				segment.type = RST_ARC;
			else if (strcmp(type, "line"))
				segment.type = RST_LINE;
			segment.r = toFloat(segmentElement->Attribute("radius"));
			segment.begin = XMLstringToVec3(segmentElement->Attribute("beginPoint"));
			segment.end = XMLstringToVec3(segmentElement->Attribute("endPoint"));
			segment.pointsCount = toInt(segmentElement->Attribute("points"));
			const char* interpolation = segmentElement->Attribute("interpolation");
			if (strcmp(interpolation, "lin") == 0)
				segment.interpolation = RI_LIN;
			else if (strcmp(interpolation, "cos") == 0)
				segment.interpolation = RI_COS;

			segments.push_back(segment);

			segmentElement = segmentElement->NextSiblingElement("Segment");
		}
		//std::cout << profiles[profileName].size() << std::endl;
		//std::cout << segments.size() << std::endl;
		// create road
		//RStaticModel * roadModel = createRoadModel(profiles[profileName], profiles[profileName].size(), segments);
		//_sceneManager->addModelToRemoveAfterClearScene(roadModel);
		//RModel* roadModel2 = new RModel("", roadModel);
		//RStaticModel* roadModel2 = new RStaticModel;
		SceneObject * roadSceneObject = _sceneManager->addSceneObject(name);
		RenderObject * roadRenderObject = _sceneManager->getGraphicsManager()->addRoadObject(roadProfile, segments, roadSceneObject);
		roadRenderObject->setCastShadows(false);
		//roadSceneObject->addComponent(roadRenderObject);
		PhysicalBodyBvtTriangleMesh * roadMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(roadRenderObject->getModel(), COL_TERRAIN, _roadCollidesWith);
		roadMesh->setRestitution(0.9f);
		roadMesh->getRigidBody()->setFriction(1.0f);
		//terrainMesh->getRigidBody()->setFriction(1.5f);
		roadSceneObject->addComponent(roadMesh);

		roadElement = roadElement->NextSiblingElement("Road");
	}
	*/
}

void SceneLoader::loadRoad(XMLElement* roadElement)
{
	while (roadElement)
	{
		std::string name = roadElement->Attribute("name");
		std::string profileName = roadElement->Attribute("profile");

		LOG_INFO("*** ROAD ***");
		LOG_INFO("profile name: " + profileName);

		RRoadProfile* roadProfile = ResourceManager::getInstance().loadRoadProfile(profileName);


		std::vector<glm::vec3> points;
		std::vector<RoadSegment> segments;

		glm::vec3 lastPoint;

		XMLElement* segmentElement = roadElement->FirstChildElement("Segment");
		while (segmentElement != nullptr)
		{
			RoadSegment segment;

			const char* type = segmentElement->Attribute("type");
			if (strcmp(type, "arc") == 0)
				segment.type = RST_ARC;
			else if (strcmp(type, "line") == 0)
				segment.type = RST_LINE;
			segment.r = toFloat(segmentElement->Attribute("radius"));

			points.push_back(XMLstringToVec3(segmentElement->Attribute("beginPoint")));
			lastPoint = XMLstringToVec3(segmentElement->Attribute("endPoint"));

			segment.pointsCount = toInt(segmentElement->Attribute("points"));
			const char* interpolation = segmentElement->Attribute("interpolation");
			if (strcmp(interpolation, "lin") == 0)
				segment.interpolation = RI_LIN;
			else if (strcmp(interpolation, "cos") == 0)
				segment.interpolation = RI_COS;

			segments.push_back(segment);

			segmentElement = segmentElement->NextSiblingElement("Segment");
		}
		// add point from last segment
		points.push_back(lastPoint);

		//std::cout << profiles[profileName].size() << std::endl;
		//std::cout << segments.size() << std::endl;
		// create road
		//RStaticModel * roadModel = createRoadModel(profiles[profileName], profiles[profileName].size(), segments);
		//_sceneManager->addModelToRemoveAfterClearScene(roadModel);
		//RModel* roadModel2 = new RModel("", roadModel);
		//RStaticModel* roadModel2 = new RStaticModel;
		SceneObject * roadSceneObject = _sceneManager->addSceneObject(name);
		RenderObject * roadRenderObject = _sceneManager->getGraphicsManager()->addRoadObject(RoadType::LINES_AND_ARC, roadProfile, points, segments, true, roadSceneObject);
		roadRenderObject->setCastShadows(false);
		//roadSceneObject->addComponent(roadRenderObject);
		PhysicalBodyBvtTriangleMesh * roadMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(roadRenderObject->getModel(), COL_TERRAIN, _roadCollidesWith);
		roadMesh->setRestitution(0.9f);
		roadMesh->getRigidBody()->setFriction(1.0f);
		//terrainMesh->getRigidBody()->setFriction(1.5f);
		roadSceneObject->addComponent(roadMesh);

		roadElement = roadElement->NextSiblingElement("Road");
	}
}

void SceneLoader::loadRoadV2(XMLElement* roadElement)
{
	while (roadElement)
	{
		std::string name = roadElement->Attribute("name");
		std::string profileName = roadElement->Attribute("profile");

		LOG_INFO("*** ROAD ***");
		LOG_INFO("profile name: " + profileName);

		RRoadProfile* roadProfile = ResourceManager::getInstance().loadRoadProfile(profileName);


		std::vector<glm::vec3> points;
		std::vector<RoadSegment> segments;
		std::vector<int> pointsInSegments;


		XMLElement* pointElement = roadElement->FirstChildElement("Point");
		while (pointElement != nullptr)
		{
			points.push_back(XMLstringToVec3(pointElement->GetText()));

			pointElement = pointElement->NextSiblingElement("Point");
		}


		XMLElement* segmentElement = roadElement->FirstChildElement("Segment");
		while (segmentElement != nullptr)
		{
			RoadSegment segment;

			const char* type = segmentElement->Attribute("type");
			if (strcmp(type, "arc") == 0)
				segment.type = RST_ARC;
			else if (strcmp(type, "line") == 0)
				segment.type = RST_LINE;
			else if (strcmp(type, "bezier") == 0)
				segment.type = RST_BEZIER_CURVE;

			segment.pointsCount = toInt(segmentElement->Attribute("points"));

			segment.r = XmlUtils::getAttributeFloatOptional(segmentElement, "radius");
			std::string interpolation = XmlUtils::getAttributeStringOptional(segmentElement, "interpolation", "lin");
			if (interpolation == "lin")
				segment.interpolation = RI_LIN;
			else if (interpolation == "cos")
				segment.interpolation = RI_COS;

			segments.push_back(segment);
			pointsInSegments.push_back(segment.pointsCount);

			segmentElement = segmentElement->NextSiblingElement("Segment");
		}

		RoadType roadType = RoadType::LINES_AND_ARC;
		if (segments.size() > 0 && segments[0].type == RST_BEZIER_CURVE)
			roadType = RoadType::BEZIER_CURVES;


		SceneObject* roadSceneObject = _sceneManager->addSceneObject(name);
		RoadObject* roadRenderObject;
		if (roadType == RoadType::BEZIER_CURVES)
		{
			BezierCurve* bezierCurve = _sceneManager->getGraphicsManager()->addBezierCurve(points, pointsInSegments);
			roadSceneObject->addComponent(bezierCurve);

			roadRenderObject = _sceneManager->getGraphicsManager()->addRoadObject(roadType, roadProfile, {}, {}, false, roadSceneObject);
		}
		else
		{
			roadRenderObject = _sceneManager->getGraphicsManager()->addRoadObject(roadType, roadProfile, points, segments, false, roadSceneObject);
		}
		roadRenderObject->setCastShadows(false);


		XMLElement* connectionElement = roadElement->FirstChildElement("Connection");
		while (connectionElement != nullptr)
		{
			int type = XmlUtils::getAttributeInt(connectionElement, "type");
			std::string crossroadName = std::string(connectionElement->Attribute("crossroadName"));
			int index = XmlUtils::getAttributeInt(connectionElement, "index");

			LOG_INFO("Load road connection point. Type: " + Strings::toString(type) + ", crossroad: " + crossroadName + ", index: " + Strings::toString(index));

			CrossroadComponent* crossroadComponent = findCrossRoadComponentBySceneObjectName(crossroadName);
			roadRenderObject->setConnectionPoint(type, crossroadComponent, index);

			connectionElement = connectionElement->NextSiblingElement("Connection");
		}

		//roadRenderObject->buildModel(false);

		roadElement = roadElement->NextSiblingElement("Road");
	}
}


void SceneLoader::loadRoadIntersection(tinyxml2::XMLElement* roadIntersectionElement)
{
	while (roadIntersectionElement)
	{
		std::string name = XmlUtils::getAttributeString(roadIntersectionElement, "name");
		glm::vec3 position = XmlUtils::getAttributeVec3(roadIntersectionElement, "position");
		int quality = XmlUtils::getAttributeIntOptional(roadIntersectionElement, "quality", 11);
		std::string profileName = XmlUtils::getAttributeString(roadIntersectionElement, "profile");

		LOG_INFO("==> SCENE OBJECT");
		LOG_INFO("Name: " + name);
		LOG_INFO("Position: " + Strings::toString(position));

		SceneObject* roadIntersectionSceneObject = _sceneManager->addSceneObject(name);
		roadIntersectionSceneObject->setPosition(position);

		RRoadProfile* profile = ResourceManager::getInstance().loadRoadProfile(profileName);
		RoadIntersectionComponent* roadIntersectionComponent = _sceneManager->getGraphicsManager()->addRoadIntersection(profile);
		roadIntersectionComponent->setQuality(quality);

		roadIntersectionSceneObject->addComponent(roadIntersectionComponent);

		int i = 0;
		for (tinyxml2::XMLElement* connectedRoad = roadIntersectionElement->FirstChildElement("ConnectedRoad");
			 connectedRoad != nullptr;
			 connectedRoad = connectedRoad->NextSiblingElement("ConnectedRoad"))
		{
			std::string name = XmlUtils::getAttributeString(connectedRoad, "name");
			int index = XmlUtils::getAttributeInt(connectedRoad, "index");
			float length = XmlUtils::getAttributeFloatOptional(connectedRoad, "length", 10.0f);
			float width = XmlUtils::getAttributeFloatOptional(connectedRoad, "width", 0.0f);
			float arc = XmlUtils::getAttributeFloat(connectedRoad, "arc");

			findRoadObjectBySceneObjectName(name)->setConnectionPointWithRoadIntersection(index, roadIntersectionComponent);

			roadIntersectionComponent->setLength(i, length);
			roadIntersectionComponent->setWidth(i, width);
			roadIntersectionComponent->setArc(i, arc);

			++i;
		}

		roadIntersectionElement = roadIntersectionElement->NextSiblingElement("RoadIntersection");
	}
}


CrossroadComponent* SceneLoader::findCrossRoadComponentBySceneObjectName(const std::string& name)
{
	SceneObject* crossroadSceneObject = _sceneManager->getSceneObject(name);
	if (crossroadSceneObject != nullptr)
	{
		Component* crossroadComponent = crossroadSceneObject->getComponent(CT_CROSSROAD);
		if (crossroadComponent != nullptr)
		{
			return dynamic_cast<CrossroadComponent*>(crossroadComponent);
		}
		else
		{
			LOG_ERROR("Cannot find Crossroad component in object: " + name);
		}
	}
	else
	{
		LOG_ERROR("Cannot find object: " + name);
	}

	return nullptr;
}


RoadObject* SceneLoader::findRoadObjectBySceneObjectName(const std::string& name)
{
	SceneObject* roadSceneObject = _sceneManager->getSceneObject(name);
	if (roadSceneObject != nullptr)
	{
		Component* roadObject = roadSceneObject->getComponent(CT_ROAD_OBJECT);
		if (roadObject != nullptr)
		{
			return dynamic_cast<RoadObject*>(roadObject);
		}
		else
		{
			LOG_ERROR("Cannot find Road in object: " + name);
		}
	}
	else
	{
		LOG_ERROR("Cannot find object: " + name);
	}

	return nullptr;
}


void SceneLoader::loadMap(std::string name)
{
	_dirPath = GameDirectories::MAPS + name + "/";

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isDirectoryExists(_dirPath))
		_dirPath = ResourceManager::getInstance().getAlternativeResourcePath() + _dirPath;
#endif // DEVELOPMENT_RESOURCES

	std::string fullPath = _dirPath + MAP_FILE_NAME;

	XMLDocument doc;
	XMLError result = doc.LoadFile(fullPath.c_str());
	if (result != XML_SUCCESS)
	{
		LOG_ERROR("Cannot read xml file: " + fullPath + "! Result: " + Strings::toString((int)result));
	}

	// Search for main element - Scene
	XMLElement* scnElement = doc.FirstChildElement("Scene");
	if (scnElement == nullptr)
	{
		LOG_ERROR("Scene element not found!");
		return;
	}

	XMLElement* descriptionElement = scnElement->FirstChildElement("Description");
	ResourceDescriptionUtils::loadResourceDescription(descriptionElement, _sceneDescription);

	LOG_INFO("Load map: " + name);

	// todo: load description
	loadStartPosition(scnElement);
	if (!loadTerrain(scnElement))
		LOG_WARNING("Cannot find terrain element!");

	XMLElement* grassElement = scnElement->FirstChildElement("Grass");
	if (grassElement)
		loadGrass(grassElement);

	XMLElement* sunElement = scnElement->FirstChildElement("Light");
	if (sunElement)
		loadSunLight(sunElement);

	XMLElement* skyElement = scnElement->FirstChildElement("Sky");
	if (skyElement)
		loadSky(skyElement);

	XMLElement* objectsElement = scnElement->FirstChildElement("Objects");
	if (objectsElement)
		loadObjects(objectsElement);
	
	XMLElement* roadsElement = scnElement->FirstChildElement("Roads");
	if (roadsElement)
		loadRoads(roadsElement);
}


ResourceDescription& SceneLoader::getLoadedSceneDescription()
{
	return _sceneDescription;
}
