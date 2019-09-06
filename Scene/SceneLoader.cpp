#include "SceneLoader.h"

#include "SceneManager.h"

#include "../Game/Directories.h"

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

	Logger::info("*** TERRAIN DATA ***");
	Logger::info("Heightmap: " + terrainHeightmap);
	Logger::info("Material: " + materialName);

	SceneObject* terrainObject = _sceneManager->addSceneObject("terrain");
	Terrain* terrainObj = GraphicsManager::getInstance().addTerrain(terrainHeightmap, _dirPath, materialName, terrainMaxHeight, terrainObject);
	terrainObj->setIsCastShadows(false);
	PhysicalBodyBvtTriangleMesh* terrainMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(terrainObj->getModel(), COL_TERRAIN, _terrainCollidesWith);
	terrainMesh->setRestitution(0.9f);
	terrainMesh->getRigidBody()->setFriction(1.0f);
	terrainObject->addComponent(terrainMesh);

	return true;
}


void SceneLoader::loadGrass(XMLElement* sceneElement)
{
	XMLElement* grassElement = sceneElement->FirstChildElement("Grass");
	if (grassElement == nullptr)
	{
		Logger::warning("Grass element not found!");
	}
	else if (GameConfig::getInstance().isGrassEnable)
	{
		std::string grassModelFileName(grassElement->Attribute("model"));
		std::string terrainHeightmapForGrassFileName(grassElement->Attribute("terrain_heightmap"));
		std::string grassDensityTextureFileName(grassElement->Attribute("density_texture"));

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
		RTexture2D * heightmapTextureForGrass = ResourceManager::getInstance().loadTexture(_dirPath + terrainHeightmapForGrassFileName);
		RTexture2D * grassDensityTexture = ResourceManager::getInstance().loadTexture(_dirPath + grassDensityTextureFileName);
		heightmapTextureForGrass->setClampMode(TCM_CLAMP_TO_EDGE);
		heightmapTextureForGrass->setFiltering(TFM_LINEAR, TFM_LINEAR);

		SceneObject * grassObject = _sceneManager->addSceneObject("grass");
		Grass * grassComponent = GraphicsManager::getInstance().addGrassComponent(grassModel, heightmapTextureForGrass, grassDensityTexture);
		grassObject->addComponent(grassComponent);
	}
}


void SceneLoader::loadSunLight(XMLElement* sceneElement)
{
	XMLElement* lightElement = sceneElement->FirstChildElement("Light");
	if (lightElement == nullptr)
	{
		Logger::warning("Light element not found!");
	}
	else
	{
		glm::vec3 rotation = XmlUtils::getAttributeVec3(lightElement, "rotation");
		glm::vec3 color = XmlUtils::getAttributeVec3(lightElement, "color");
		float ambientIntensity = atof(lightElement->Attribute("ambientIntensity"));
		float diffuseIntensity = atof(lightElement->Attribute("diffuseIntensity"));

		SceneObject* dirLightObject = _sceneManager->addSceneObject("sun");
		Light* lightComponent = GraphicsManager::getInstance().addDirectionalLight(color, ambientIntensity, diffuseIntensity);
		dirLightObject->addComponent(lightComponent);
		dirLightObject->setRotation(rotation);
		dirLightObject->setPosition(glm::vec3(0, 0, 0));
		lightComponent->setShadowMapping(GameConfig::getInstance().isShadowmappingEnable);
	}
}


bool SceneLoader::loadSky(XMLElement* sceneElement)
{
	XMLElement* sklyElement = sceneElement->FirstChildElement("Sky");
	if (sklyElement == nullptr)
	{
		Logger::error("Sky element not found!");
		return false;
	}

	std::string skyboxFileNames = std::string(sklyElement->Attribute("texture"));
	std::vector<std::string> skyboxFileNamesArray = split(skyboxFileNames, ',');
	if (skyboxFileNamesArray.size() == 6)
	{
		for (int i = 0; i < skyboxFileNamesArray.size(); ++i)
		{
			skyboxFileNamesArray[i] = GameDirectories::SKYBOX + skyboxFileNamesArray[i];
		}

		SceneObject* skySceneObject = _sceneManager->addSceneObject("sky");

		RTextureCubeMap* skyboxTexture = ResourceManager::getInstance().loadTextureCubeMap(&skyboxFileNamesArray[0]);
		Sky* skyComponent = GraphicsManager::getInstance().addSky(skyboxTexture, skySceneObject);

		/*std::string names[] = {
			GameDirectories::SKYBOX + "output_iem_posx.hdr",
			GameDirectories::SKYBOX + "output_iem_negx.hdr",
			GameDirectories::SKYBOX + "output_iem_posy.hdr",
			GameDirectories::SKYBOX + "output_iem_negy.hdr",
			GameDirectories::SKYBOX + "output_iem_posz.hdr",
			GameDirectories::SKYBOX + "output_iem_negz.hdr"
		};*/
		std::string names[] = {
			GameDirectories::SKYBOX + "irradiance/output_skybox_posx.hdr",
			GameDirectories::SKYBOX + "irradiance/output_skybox_negx.hdr",
			GameDirectories::SKYBOX + "irradiance/output_skybox_posy.hdr",
			GameDirectories::SKYBOX + "irradiance/output_skybox_negy.hdr",
			GameDirectories::SKYBOX + "irradiance/output_skybox_posz.hdr",
			GameDirectories::SKYBOX + "irradiance/output_skybox_negz.hdr",
		};
		/*std::string specularIrradianecNames[] = {
			GameDirectories::SKYBOX + "specular/specularIbl_posx.hdr",
			GameDirectories::SKYBOX + "specular/specularIbl_negx.hdr",
			GameDirectories::SKYBOX + "specular/specularIbl_posy.hdr",
			GameDirectories::SKYBOX + "specular/specularIbl_negy.hdr",
			GameDirectories::SKYBOX + "specular/specularIbl_posz.hdr",
			GameDirectories::SKYBOX + "specular/specularIbl_negz.hdr",
		};*/
		std::string specularIrradianecNames[] = {
			GameDirectories::SKYBOX + "specularFromCMFT/output_pmrem_posx.hdr",
			GameDirectories::SKYBOX + "specularFromCMFT/output_pmrem_negx.hdr",
			GameDirectories::SKYBOX + "specularFromCMFT/output_pmrem_posy.hdr",
			GameDirectories::SKYBOX + "specularFromCMFT/output_pmrem_negy.hdr",
			GameDirectories::SKYBOX + "specularFromCMFT/output_pmrem_posz.hdr",
			GameDirectories::SKYBOX + "specularFromCMFT/output_pmrem_negz.hdr",
		};
		RTextureCubeMap* irradianceMap = ResourceManager::getInstance().loadTextureCubeMap(names);
		RTextureCubeMap* specularIrradianceMap = ResourceManager::getInstance().loadTextureCubeMap(specularIrradianecNames);

		GraphicsManager::getInstance().addGlobalEnvironmentCaptureComponent(skyboxTexture, irradianceMap, specularIrradianceMap);

		return true;
	}
	else
	{
		Logger::error("Number of skybox files is invalid!");
		return false;
	}
}


void SceneLoader::loadObjects(XMLElement* sceneElement)
{
	XMLElement* objects = sceneElement->FirstChildElement("Objects");

	XMLElement* objectElement = objects->FirstChildElement("Object");
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


void SceneLoader::loadRoads(XMLElement* sceneElement)
{
	XMLElement* roads = sceneElement->FirstChildElement("Roads");

	XMLElement* roadElement = roads->FirstChildElement("Road");
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
}


void SceneLoader::loadMap(std::string name)
{
	_dirPath = GameDirectories::MAPS + name + "/";
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
		return;
	loadGrass(scnElement);
	loadSunLight(scnElement);
	if (!loadSky(scnElement))
		return;
	loadObjects(scnElement);
	loadRoads(scnElement);
}


SceneDescription& SceneLoader::getLoadedSceneDescription()
{
	return _sceneDescription;
}
