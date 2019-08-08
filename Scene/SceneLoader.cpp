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

	std::string heightmapFullPath = _dirPath + terrainHeightmap;
	std::string materialFullPath = _dirPath + MaterialLoader::createMaterialFileName(terrainHeightmap);
	RStaticModel* terrModel = TerrainLoader::loadTerrainModel(heightmapFullPath.c_str(), materialFullPath, materialName, _dirPath, terrainMaxHeight);
	_sceneManager->addModelToRemoveAfterClearScene(terrModel);

	SceneObject* terrainObject = _sceneManager->addSceneObject("terrain");
	RenderObject* terrainObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(terrModel), terrainObject);
	terrainObj->setIsCastShadows(false);
	PhysicalBodyBvtTriangleMesh* terrainMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(terrModel, COL_TERRAIN, _terrainCollidesWith);
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

		RTextureCubeMap* skyboxTexture = ResourceManager::getInstance().loadTextureCubeMap(&skyboxFileNamesArray[0]);
		_sceneManager->addSky(skyboxTexture);
		GraphicsManager::getInstance().addGlobalEnvironmentCaptureComponent(skyboxTexture);

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

		std::string dirPath = GameDirectories::OBJECTS + name + "/";
		std::string fullPath = dirPath + OBJECT_FILE_NAME;
		RObject* rObject = ResourceManager::getInstance().loadRObject(fullPath);

		SceneObject* sceneObject = createObject(rObject, dirPath, objectName, position, rotation);

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


SceneObject* SceneLoader::createObject(RObject* objectDefinition, std::string objectDirPath, std::string name, glm::vec3 position, glm::vec3 rotation)
{
	SceneObject* sceneObject = _sceneManager->addSceneObject(name, objectDefinition);
	sceneObject->setPosition(position);
	sceneObject->setRotation(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z));

	RStaticModel* model = nullptr;

	std::vector<std::unordered_map<std::string, std::string>>& components = objectDefinition->getComponents();

	for (int i = 0; i < components.size(); ++i)
	{
		std::string componentType = components[i]["type"];

		if (componentType == "render")
		{
			std::string modelFile = components[i]["model"];

			std::string modelPath = objectDirPath + modelFile;

			model = ResourceManager::getInstance().loadModel(modelPath, objectDirPath);
			RenderObject* renderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(model), sceneObject);
		}
		else if (componentType == "physics")
		{
			std::string bodyType = components[i]["body"];

			PhysicsManager* physicsManager = _sceneManager->getPhysicsManager();
			PhysicalBody* physicalBody;
			if (bodyType == "box")
			{
				int collidesWith = COL_TERRAIN | COL_BUS | COL_ENV;
				float halfExtents = toFloat(components[i]["halfExtents"]);
				float mass = toFloat(components[i]["mass"]);

				physicalBody = physicsManager->createPhysicalBodyBox(btVector3(halfExtents, halfExtents, halfExtents), mass, COL_ENV, collidesWith);
				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "dynamic")
			{
				Logger::info("- Creating dynamic Convex Hull collision shape");

				float mass = toFloat(components[i]["mass"]);
				int collidesWith = COL_TERRAIN | COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyConvexHull* physicalBody = physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), mass,
					COL_ENV, collidesWith);

				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "static")
			{
				Logger::info("- Creating static Convex Hull collision shape");

				int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyConvexHull* physicalBody = physicsManager->createPhysicalBodyConvexHull(model->getCollisionMesh(), model->getCollisionMeshSize(), 0,
					COL_ENV, collidesWith);
				//terrainMesh->setRestitution(0.9f);
				//terrainMesh->getRigidBody()->setFriction(1.0f);
				sceneObject->addComponent(physicalBody);
			}
			else if (bodyType == "bvh")
			{
				Logger::info("- Creating BVH Triangle Mesh collision shape");

				int collidesWith = COL_WHEEL | COL_BUS | COL_DOOR | COL_ENV;

				PhysicalBodyBvtTriangleMesh* physicalBody = physicsManager->createPhysicalBodyBvtTriangleMesh(model, COL_ENV, collidesWith);
				//terrainMesh->setRestitution(0.9f);
				//terrainMesh->getRigidBody()->setFriction(1.0f);
				sceneObject->addComponent(physicalBody);
			}
		}
		else if (componentType == "tree")
		{
			TreeComponent* component = new TreeComponent;
			sceneObject->addComponent(component);
		}
		else if (componentType == "sound")
		{
			std::string soundFile = components[i]["file"];
			bool looping = toBool(components[i]["looping"]);
			float playDistance = toFloat(components[i]["playDistance"]);
			float volume = toFloat(components[i]["volume"]);
			glm::vec3 soundPosition = XMLstringToVec3(components[i]["position"].c_str());

			std::string soundPath = objectDirPath + soundFile;

			RSound * soundResource = ResourceManager::getInstance().loadSound(soundPath);
			SoundComponent * sound = new SoundComponent(soundResource, EST_AMBIENT, looping);
			_sceneManager->getSoundManager()->addSoundComponent(sound);

			sound->setPlayDistance(playDistance);
			sound->setGain(volume);
			sound->setPosition(soundPosition);

			sceneObject->addComponent(sound);
		}
		else if (componentType == "bus-stop")
		{
			BusStopComponent* component = BusStopSystem::getInstance().addBusStopComponent("Przystanek");
			sceneObject->addComponent(component);
		}
		else if (componentType == "environmentCapture")
		{
			std::string textures = components[i]["textures"];
			std::vector<std::string> t = split(textures, ',');

			RTextureCubeMap* cubeMap = ResourceManager::getInstance().loadTextureCubeMap(&t[0]);
			EnvironmentCaptureComponent* component = GraphicsManager::getInstance().addEnvironmentCaptureComponent(cubeMap);
			sceneObject->addComponent(component);
		}
	}

	return sceneObject;
}


void SceneLoader::loadRoads(XMLElement* sceneElement)
{
	XMLElement* roads = sceneElement->FirstChildElement("Roads");

	std::map<std::string, std::vector<RoadLane>> profiles;

	XMLElement* roadElement = roads->FirstChildElement("Road");
	while (roadElement != nullptr)
	{
		std::string name = roadElement->Attribute("name");
		std::string profileName = roadElement->Attribute("profile");

		Logger::info("*** ROAD ***");
		Logger::info("profile name: " + profileName);


		if (profiles.find(profileName) == profiles.end())
		{
			loadRoadProfile(profileName, &profiles);
		}


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
		RStaticModel * roadModel = createRoadModel(profiles[profileName], profiles[profileName].size(), segments);
		_sceneManager->addModelToRemoveAfterClearScene(roadModel);
		//RModel* roadModel2 = new RModel("", roadModel);
		//RStaticModel* roadModel2 = new RStaticModel;
		SceneObject * roadSceneObject = _sceneManager->addSceneObject(name);
		RenderObject * roadRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(roadModel), roadSceneObject);
		roadRenderObject->setIsCastShadows(false);
		//roadSceneObject->addComponent(roadRenderObject);
		PhysicalBodyBvtTriangleMesh * roadMesh = _sceneManager->getPhysicsManager()->createPhysicalBodyBvtTriangleMesh(roadModel, COL_TERRAIN, _roadCollidesWith);
		roadMesh->setRestitution(0.9f);
		roadMesh->getRigidBody()->setFriction(1.0f);
		//terrainMesh->getRigidBody()->setFriction(1.5f);
		roadSceneObject->addComponent(roadMesh);

		roadElement = roadElement->NextSiblingElement("Road");
	}
}


void SceneLoader::loadRoadProfile(std::string name, std::map<std::string, std::vector<RoadLane>>* profiles)
{
	std::string dirPath = "RoadProfiles/" + name + "/";
	std::string fullPath = dirPath + "profile.xml";
	std::string materialFullPath = MaterialLoader::createMaterialFileName(fullPath);

	//std::cout << "Profile path: " << fullPath << std::endl;

	XMLDocument doc;
	XMLError result = doc.LoadFile(fullPath.c_str());
	std::cout << result << std::endl;

	// Search for main element - Object
	XMLElement* profileElement = doc.FirstChildElement("Profile");
	if (profileElement == nullptr)
	{
		std::cout << "Profile element not found!" << std::endl;
		return;
	}

	XMLElement* profileDesc = profileElement->FirstChildElement("Description");
	if (profileDesc == nullptr)
	{
		std::cout << "Description element not found" << std::endl;
		return;
	}

	// Load file description
	std::string author(profileDesc->Attribute("author"));
	std::string profName(profileDesc->Attribute("name"));
	std::string comment(profileDesc->Attribute("comment"));

	std::cout << "*** PROFILE DATA ***" << std::endl;
	std::cout << "Author: " << author << std::endl;
	std::cout << "Name: " << profName << std::endl;
	std::cout << "Comment: " << comment << std::endl;

	MaterialLoader matLoader;
	matLoader.openFile(materialFullPath.c_str());

	XMLElement* lanesElement = profileElement->FirstChildElement("Lanes");

	if (lanesElement == nullptr)
	{
		std::cout << "Lanes element not found" << std::endl;
		return;
	}
	else
	{
		profiles->insert(std::make_pair(name, std::vector<RoadLane>()));

		XMLElement* laneElement = lanesElement->FirstChildElement("Lane");
		while (laneElement != nullptr)
		{
			RoadLane lane;
			lane.material = matLoader.loadMaterial(laneElement->Attribute("material"), dirPath);
			lane.r1 = toFloat(laneElement->Attribute("x1"));
			lane.r2 = toFloat(laneElement->Attribute("x2"));
			lane.height1 = toFloat(laneElement->Attribute("y1"));
			lane.height2 = toFloat(laneElement->Attribute("y2"));

			(*profiles)[name].push_back(lane);

			laneElement = laneElement->NextSiblingElement("Lane");
		}
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
