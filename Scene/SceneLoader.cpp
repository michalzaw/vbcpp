#include "SceneLoader.h"

#include "SceneManager.h"

#include "../Game/Directories.h"

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
		Logger::warning("Start point not found!");
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
		Logger::error("Terrain element not found!");
		return false;
	}

	std::string terrainHeightmap(terrElement->Attribute("heightmap"));
	std::string materialName(terrElement->Attribute("material"));
	float terrainMaxHeight = XmlUtils::getAttributeFloatOptional(terrElement, "maxHeight", 20.0f);
	bool is16bitTexture = XmlUtils::getAttributeBoolOptional(terrElement, "is16bitTexture");

	Logger::info("*** TERRAIN DATA ***");
	Logger::info("Heightmap: " + terrainHeightmap);
	Logger::info("Material: " + materialName);

	SceneObject* terrainObject = _sceneManager->addSceneObject("terrain");
	Terrain* terrainObj = GraphicsManager::getInstance().addTerrain(terrainHeightmap, _dirPath, materialName, terrainMaxHeight, is16bitTexture, terrainObject);
	terrainObj->setIsCastShadows(false);
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
		Logger::warning("Grass element not found!");
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

		Logger::info("*** GRASS DATA ***");
		Logger::info("Model: " + grassModelFileName);
		Logger::info("heightmap: " + terrainHeightmapForGrassFileName);
		Logger::info("density texture: " + grassDensityTextureFileName);

		RStaticModel* grassModel = ResourceManager::getInstance().loadModel(_dirPath + "grass/" + grassModelFileName, _dirPath + "grass/");

		RTexture2D * heightmapTextureForGrass = ResourceManager::getInstance().loadTexture(_dirPath + terrainHeightmapForGrassFileName, false);
		RTexture2D * grassDensityTexture = ResourceManager::getInstance().loadTexture(_dirPath + grassDensityTextureFileName, false);
		heightmapTextureForGrass->setClampMode(TCM_CLAMP_TO_EDGE);
		heightmapTextureForGrass->setFiltering(TFM_LINEAR, TFM_LINEAR);

		SceneObject * grassObject = _sceneManager->addSceneObject("grass");
		Grass * grassComponent = GraphicsManager::getInstance().addGrassComponent(grassModel, heightmapTextureForGrass, grassDensityTexture);
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
		Logger::warning("Grass element not found!");
}


void SceneLoader::loadSunLight(XMLElement* sunElement)
{
	//XMLElement* lightElement = sceneElement->FirstChildElement("Light");
	//if (lightElement == nullptr)
	//{
	//	Logger::warning("Light element not found!");
	//}
	//else
	//{
		glm::vec3 rotation = XmlUtils::getAttributeVec3(sunElement, "rotation");
		glm::vec3 color = XmlUtils::getAttributeVec3(sunElement, "color");
		float ambientIntensity = atof(sunElement->Attribute("ambientIntensity"));
		float diffuseIntensity = atof(sunElement->Attribute("diffuseIntensity"));

		SceneObject* dirLightObject = _sceneManager->addSceneObject("sun");
		Light* lightComponent = GraphicsManager::getInstance().addDirectionalLight(color, ambientIntensity, diffuseIntensity);
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
			Sky* skyComponent = GraphicsManager::getInstance().addSky(skyboxTexture, skySceneObject);

			GraphicsManager::getInstance().addGlobalEnvironmentCaptureComponent(skyboxTexture);

		}
		else
			Logger::error("Number of skybox files is invalid!");
	//}
	//else
	//{
	//	Logger::error("Sky element not found!");
	//	return false;
	//}
}


void SceneLoader::loadObjects(XMLElement* objectsElement)
{
	XMLElement* objectElement = objectsElement->FirstChildElement("Object");
	if (objectElement)
		loadObject(objectElement);

	//XMLElement* objects = sceneElement->FirstChildElement("Objects");

	//XMLElement* objectElement = objectsElement->FirstChildElement("Object");
	/*
	while (objectElement != nullptr)
	{
		std::string name(objectElement->Attribute("name"));
		std::string objectName = XmlUtils::getAttributeStringOptional(objectElement, "id", name);
		glm::vec3 position = XmlUtils::getAttributeVec3(objectElement, "position");
		glm::vec3 rotation = XmlUtils::getAttributeVec3(objectElement, "rotation");

		Logger::info("==> SCENE OBJECT");
		Logger::info("Name: " + name);
		Logger::info("Position: " + vec3ToString(position));
		Logger::info("Rotation: " + vec3ToString(rotation));

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

void SceneLoader::loadObject(XMLElement* objectElement)
{
	while (objectElement != nullptr)
	{
		std::string name(objectElement->Attribute("name"));
		std::string objectName = XmlUtils::getAttributeStringOptional(objectElement, "id", name);
		glm::vec3 position = XmlUtils::getAttributeVec3(objectElement, "position");
		glm::vec3 rotation = XmlUtils::getAttributeVec3(objectElement, "rotation");

		Logger::info("==> SCENE OBJECT");
		Logger::info("Name: " + name);
		Logger::info("Position: " + vec3ToString(position));
		Logger::info("Rotation: " + vec3ToString(rotation));

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

	/*
	while (roadElement != nullptr)
	{
		std::string name = roadElement->Attribute("name");
		std::string profileName = roadElement->Attribute("profile");

		Logger::info("*** ROAD ***");
		Logger::info("profile name: " + profileName);

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
		RenderObject * roadRenderObject = GraphicsManager::getInstance().addRoadObject(roadProfile, segments, roadSceneObject);
		roadRenderObject->setIsCastShadows(false);
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

		Logger::info("*** ROAD ***");
		Logger::info("profile name: " + profileName);

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
		RenderObject * roadRenderObject = GraphicsManager::getInstance().addRoadObject(RoadType::LINES_AND_ARC, roadProfile, points, segments, true, roadSceneObject);
		roadRenderObject->setIsCastShadows(false);
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

		Logger::info("*** ROAD ***");
		Logger::info("profile name: " + profileName);

		RRoadProfile* roadProfile = ResourceManager::getInstance().loadRoadProfile(profileName);


		std::vector<glm::vec3> points;
		std::vector<RoadSegment> segments;


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

			segmentElement = segmentElement->NextSiblingElement("Segment");
		}

		RoadType roadType = RoadType::LINES_AND_ARC;
		if (segments.size() > 0 && segments[0].type == RST_BEZIER_CURVE)
			roadType = RoadType::BEZIER_CURVES;


		SceneObject* roadSceneObject = _sceneManager->addSceneObject(name);
		RoadObject* roadRenderObject = GraphicsManager::getInstance().addRoadObject(roadType, roadProfile, points, segments, false, roadSceneObject);
		roadRenderObject->setIsCastShadows(false);


		XMLElement* connectionElement = roadElement->FirstChildElement("Connection");
		while (connectionElement != nullptr)
		{
			int type = XmlUtils::getAttributeInt(connectionElement, "type");
			std::string crossroadName = std::string(connectionElement->Attribute("crossroadName"));
			int index = XmlUtils::getAttributeInt(connectionElement, "index");

			Logger::info("Load road connection point. Type: " + toString(type) + ", crossroad: " + crossroadName + ", index: " + toString(index));

			CrossroadComponent* crossroadComponent = findCrossRoadComponentBySceneObjectName(crossroadName);
			roadRenderObject->setConnectionPoint(type, crossroadComponent, index);

			connectionElement = connectionElement->NextSiblingElement("Connection");
		}

		roadRenderObject->buildModel(false);

		PhysicalBodyBvtTriangleMesh* roadMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(roadRenderObject->getModel(), COL_TERRAIN, _roadCollidesWith);
		roadMesh->setRestitution(0.9f);
		roadMesh->getRigidBody()->setFriction(1.0f);
		//terrainMesh->getRigidBody()->setFriction(1.5f);
		roadSceneObject->addComponent(roadMesh);


		roadElement = roadElement->NextSiblingElement("Road");
	}
}


CrossroadComponent* SceneLoader::findCrossRoadComponentBySceneObjectName(std::string& name)
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
			Logger::error("Cannot find Crossroad component in object: " + name);
		}
	}
	else
	{
		Logger::error("Cannot find object: " + name);
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
		Logger::error("Cannot read xml file: " + fullPath + "! Result: " + toString(result));
	}

	// Search for main element - Scene
	XMLElement* scnElement = doc.FirstChildElement("Scene");
	if (scnElement == nullptr)
	{
		Logger::error("Scene element not found!");
		return;
	}

	XMLElement* descriptionElement = scnElement->FirstChildElement("Description");
	if (descriptionElement != nullptr)
	{
		_sceneDescription.author = descriptionElement->Attribute("author");
	}

	Logger::info("Load map: " + name);

	// todo: load description
	loadStartPosition(scnElement);
	if (!loadTerrain(scnElement))
		Logger::warning("Cannot find terrain element!");

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


SceneDescription& SceneLoader::getLoadedSceneDescription()
{
	return _sceneDescription;
}
