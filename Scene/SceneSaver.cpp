#include "SceneSaver.h"

#include <algorithm>

#include "SceneManager.h"
#include "SceneLoader.h"

#include "../Game/AIAgent.h"
#include "../Game/BusStartPoint.h"
#include "../Game/Directories.h"
#include "../Game/PathComponent.h"

#include "../Graphics/BezierCurve.h"
#include "../Graphics/RenderObject.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceDescriptionUtils.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"


SceneSaver::SceneSaver(SceneManager* sceneManager)
	: _sceneManager(sceneManager)
{

}


std::string SceneSaver::createSkyTextureAttribute(std::string path)
{
	if (path[path.size() - 1] == ';')
		path = path.substr(0, path.size() - 1);

	std::vector<std::string> textures = split(path, ';');

	if (textures.size() != 6)
		LOG_ERROR("Scene saver: Invalid skybox textures path");

	std::string result = FilesHelper::getRelativePathToDir(textures[0], GameDirectories::SKYBOX) + "," + FilesHelper::getRelativePathToDir(textures[1], GameDirectories::SKYBOX) + "," +
						 FilesHelper::getRelativePathToDir(textures[2], GameDirectories::SKYBOX) + "," + FilesHelper::getRelativePathToDir(textures[3], GameDirectories::SKYBOX) + "," +
						 FilesHelper::getRelativePathToDir(textures[4], GameDirectories::SKYBOX) + "," + FilesHelper::getRelativePathToDir(textures[5], GameDirectories::SKYBOX);

	return result;
}


void SceneSaver::saveTerrain(XMLElement* terrainElement, XMLElement* grassElement, SceneObject* sceneObject)
{
	Terrain* terrainComponent = static_cast<Terrain*>(sceneObject->getComponent(CT_TERRAIN));

	if (terrainComponent)
	{
		terrainElement->SetAttribute("heightmap", terrainComponent->getHeightmapFileName().c_str());
		terrainElement->SetAttribute("material", terrainComponent->getMaterialName().c_str());
		terrainElement->SetAttribute("maxHeight", terrainComponent->getMaxHeight());

		grassElement->SetAttribute("terrain_heightmap", terrainComponent->getHeightmapFileName().c_str());
	}
}


void SceneSaver::saveGrass(XMLElement* grassElement, XMLDocument& doc, SceneObject* sceneObject)
{
	Grass* grassComponent = static_cast<Grass*>(sceneObject->getComponent(CT_GRASS));

	if (grassComponent)
	{
		grassElement->SetAttribute("model", FilesHelper::getRelativePathToDir(grassComponent->getModel()->getPath(), _dirPath + "grass/").c_str());
		grassElement->SetAttribute("density_texture", FilesHelper::getRelativePathToDir(grassComponent->getGrassDensityTexture()->getPath(), _dirPath).c_str());
		grassElement->SetAttribute("color", vec3ToString(glm::vec3(grassComponent->getGrassColor().x, grassComponent->getGrassColor().y, grassComponent->getGrassColor().z)).c_str());

		std::vector<RTexture2D*>& grassTextures = grassComponent->getAdditionalRandomGrassTextures();
		std::vector<float>& grassTexturesScale = grassComponent->getAdditionalRandomGrassTexturesScale();
		for (int i = 0; i < grassTextures.size(); ++i)
		{
			XMLElement* textureElement = doc.NewElement("Texture");

			textureElement->SetAttribute("path", FilesHelper::getRelativePathToDir(grassTextures[i]->getPath(), _dirPath + "grass/").c_str());
			textureElement->SetAttribute("scale", grassTexturesScale[i]);

			grassElement->InsertEndChild(textureElement);
		}
	}
}


void SceneSaver::saveSunLight(XMLElement* sunElement, SceneObject* sceneObject)
{
	Light* light = static_cast<Light*>(sceneObject->getComponent(CT_LIGHT));

	if (light)
	{
		sunElement->SetAttribute("rotation", vec3ToString(sceneObject->getRotation()).c_str());
		sunElement->SetAttribute("color", vec3ToString(light->getColor()).c_str());
		sunElement->SetAttribute("ambientIntensity", light->getAmbientIntensity());
		sunElement->SetAttribute("diffuseIntensity", light->getDiffiseIntenisty());
	}
}


void SceneSaver::saveSky(XMLElement* skyElement, SceneObject* sceneObject)
{
	RenderObject* renderObject = static_cast<RenderObject*>(sceneObject->getComponent(CT_SKY));

	if (renderObject)
	{
		std::string skyboxTexturePaths = renderObject->getModel()->getMaterial(0)->diffuseTexture->getPath();
		skyElement->SetAttribute("texture", createSkyTextureAttribute(skyboxTexturePaths).c_str());
	}
}


void SceneSaver::savePrefabComponent(XMLElement* objectElement, XMLDocument& doc, Prefab* prefab)
{
	XMLElement* componentElement = doc.NewElement("Component");

	componentElement->SetAttribute("type", "prefab");
	componentElement->SetAttribute("prefabType", convertPrefabTypeToString(prefab->getPrefabType()).c_str());

	if (prefab->getPrefabType() == PrefabType::PLANE)
	{
		PlanePrefab* plane = static_cast<PlanePrefab*>(prefab);
		componentElement->SetAttribute("size", vec2ToString(plane->getSize()).c_str());
	}

	if (prefab->getPrefabType() == PrefabType::CUBE)
	{
		Cube* cube = static_cast<Cube*>(prefab);
		componentElement->SetAttribute("size", toString(cube->getSize()).c_str());
	}

	if (prefab->getPrefabType() == PrefabType::SPHERE)
	{
		SpherePrefab* sphere = static_cast<SpherePrefab*>(prefab);
		componentElement->SetAttribute("radius", toString(sphere->getRadius()).c_str());
		componentElement->SetAttribute("quality", toString(sphere->getQuality()).c_str());
	}

	if (prefab->getPrefabType() == PrefabType::CYLINDER)
	{
		CylinderPrefab* cylinder = static_cast<CylinderPrefab*>(prefab);
		componentElement->SetAttribute("radius", toString(cylinder->getRadius()).c_str());
		componentElement->SetAttribute("height", toString(cylinder->getHeight()).c_str());
		componentElement->SetAttribute("axis", toString(cylinder->getAxis()).c_str());
		componentElement->SetAttribute("quality", toString(cylinder->getQuality()).c_str());
	}

	objectElement->InsertEndChild(componentElement);
}


void SceneSaver::saveBezierCurveComponent(tinyxml2::XMLElement* objectElement, tinyxml2::XMLDocument& doc, BezierCurve* bezierCurve)
{
	XMLElement* componentElement = doc.NewElement("Component");

	componentElement->SetAttribute("type", "bezierCurve");
	componentElement->SetAttribute("offsetFromBaseCurve", vec2ToString(bezierCurve->getOffsetFromBaseCurve()).c_str());
	componentElement->SetAttribute("marginBegin", Strings::toString(bezierCurve->getMarginBegin()).c_str());
	componentElement->SetAttribute("marginEnd", Strings::toString(bezierCurve->getMarginEnd()).c_str());

	XMLElement* pointsElement = doc.NewElement("Points");
	for (const auto& point : bezierCurve->getPoints())
	{
		XMLElement* pointElement = doc.NewElement("Point");

		pointElement->SetText(vec3ToString(point).c_str());

		pointsElement->InsertEndChild(pointElement);
	}
	componentElement->InsertEndChild(pointsElement);

	XMLElement* segmentsElement = doc.NewElement("Segments");
	for (int i = 0; i < bezierCurve->getSegmentsCount(); ++i)
	{
		XMLElement* segmentElement = doc.NewElement("Segment");

		segmentElement->SetAttribute("points", bezierCurve->getSegmentPointsCount(i));

		segmentsElement->InsertEndChild(segmentElement);
	}
	componentElement->InsertEndChild(segmentsElement);

	objectElement->InsertEndChild(componentElement);
}


void SceneSaver::savePathComponent(XMLElement* objectElement, XMLDocument& doc, PathComponent* pathComponent)
{
	XMLElement* componentElement = doc.NewElement("Component");

	componentElement->SetAttribute("type", "path");
	componentElement->SetAttribute("direction", Strings::toString(static_cast<int>(pathComponent->getDirection())).c_str());

	const auto& nextPaths = pathComponent->getNextPaths();
	if (nextPaths.size() > 0)
	{
		XMLElement* connectionsElement = doc.NewElement("Connections");
		for (const auto& connectedPath : nextPaths)
		{
			XMLElement* connectionElement = doc.NewElement("Connection");

			connectionElement->SetAttribute("pathName", connectedPath.path->getSceneObject()->getName().c_str());
			connectionElement->SetAttribute("index", connectedPath.index);

			connectionsElement->InsertEndChild(connectionElement);
		}
		componentElement->InsertEndChild(connectionsElement);
	}

	objectElement->InsertEndChild(componentElement);
}


void SceneSaver::saveAIAgentComponent(XMLElement* objectElement, XMLDocument& doc, AIAgent* aiAgent)
{
	if (aiAgent->getCurrentPath() != nullptr)
	{
		XMLElement* componentElement = doc.NewElement("Component");

		componentElement->SetAttribute("type", "aiAgent");
		//componentElement->SetAttribute("path", aiAgent->getCurrentPath()->getSceneObject()->getName().c_str());
		// path is now loaded from parent object
		// todo: save other agent params

		objectElement->InsertEndChild(componentElement);
	}
	else
	{
		LOG_DEBUG("Path is null. Skip saving component data to map file");
	}
}


void SceneSaver::saveBusStartPointComponent(tinyxml2::XMLElement* objectElement, tinyxml2::XMLDocument& doc, BusStartPoint* busStartPoint)
{
	XMLElement* componentElement = doc.NewElement("Component");

	componentElement->SetAttribute("type", "busStartPoint");
	componentElement->SetAttribute("name", busStartPoint->getName().c_str());

	objectElement->InsertEndChild(componentElement);
}


void SceneSaver::saveObject(XMLElement* objectsElement, XMLDocument& doc, SceneObject* sceneObject, RObject* objectDefinition)
{
	XMLElement* objectElement = doc.NewElement("Object");

	const std::string& objectDefinitionName = objectDefinition != nullptr ? objectDefinition->getOriginalName() : "";

	objectElement->SetAttribute("name", objectDefinitionName.c_str());

	if (objectDefinitionName != sceneObject->getName())
		objectElement->SetAttribute("id", sceneObject->getName().c_str());

	objectElement->SetAttribute("position", vec3ToString(sceneObject->getPosition()).c_str());
	glm::vec3 rotation = glm::vec3(radToDeg(sceneObject->getRotation().x),
								   radToDeg(sceneObject->getRotation().y),
								   radToDeg(sceneObject->getRotation().z));
	objectElement->SetAttribute("rotation", vec3ToString(rotation).c_str());

	BusStopComponent* busStopComponent = static_cast<BusStopComponent*>(sceneObject->getComponent(CT_BUS_STOP));
	if (busStopComponent)
	{
		XMLElement* componentElement = doc.NewElement("Component");

		componentElement->SetAttribute("type", "bus-stop");
		componentElement->SetAttribute("busStopName", busStopComponent->getName().c_str());
		componentElement->SetAttribute("id", busStopComponent->getId());
		componentElement->SetAttribute("announcement", busStopComponent->getAnnouncementFileName().c_str());

		objectElement->InsertEndChild(componentElement);
	}

	Prefab* prefab = static_cast<Prefab*>(sceneObject->getComponent(CT_PREFAB));
	if (prefab)
	{
		savePrefabComponent(objectElement, doc, prefab);
	}

	BezierCurve* bezierCurveComponent = static_cast<BezierCurve*>(sceneObject->getComponent(CT_BEZIER_CURVE));
	if (bezierCurveComponent)
	{
		saveBezierCurveComponent(objectElement, doc, bezierCurveComponent);
	}

	PathComponent* pathComponent = static_cast<PathComponent*>(sceneObject->getComponent(CT_PATH));
	if (pathComponent)
	{
		savePathComponent(objectElement, doc, pathComponent);
	}

	AIAgent* aiAgentComponent = static_cast<AIAgent*>(sceneObject->getComponent(CT_AI_AGENT));
	if (aiAgentComponent)
	{
		saveAIAgentComponent(objectElement, doc, aiAgentComponent);
	}

	BusStartPoint* busStartPointComponent = static_cast<BusStartPoint*>(sceneObject->getComponent(CT_BUS_START_POINT));
	if (busStartPointComponent)
	{
		saveBusStartPointComponent(objectElement, doc, busStartPointComponent);
	}

	objectsElement->InsertEndChild(objectElement);

	for (SceneObject* child : sceneObject->getChildren())
	{
		saveSceneObject(doc, objectElement, child, sceneObject);
	}
}


void SceneSaver::saveRoad(XMLElement* roadsElement, XMLDocument& doc, SceneObject* sceneObject)
{
	XMLElement* roadElement = doc.NewElement("Road");

	RoadObject* roadObject = static_cast<RoadObject*>(sceneObject->getComponent(CT_ROAD_OBJECT));
	BezierCurve* bezierCurve = dynamic_cast<BezierCurve*>(sceneObject->getComponent(CT_BEZIER_CURVE));
	
	if (roadObject)
	{
		RRoadProfile* profile = roadObject->getRoadProfile();

		roadElement->SetAttribute("name", sceneObject->getName().c_str());
		roadElement->SetAttribute("profile", profile->getName().c_str());

		const auto& points = bezierCurve != nullptr ? bezierCurve->getPoints() : roadObject->getPoints();
		for (const glm::vec3& point : points)
		{
			XMLElement* pointElement = doc.NewElement("Point");

			pointElement->SetText(vec3ToString(point).c_str());

			roadElement->InsertEndChild(pointElement);
		}

		if (bezierCurve != nullptr)
		{
			for (int i = 0; i < bezierCurve->getSegmentsCount(); ++i)
			{
				XMLElement* segmentElement = doc.NewElement("Segment");

				segmentElement->SetAttribute("type", "bezier");
				segmentElement->SetAttribute("points", bezierCurve->getSegmentPointsCount(i));

				roadElement->InsertEndChild(segmentElement);
			}
		}
		else
		{
			for (const RoadSegment& segment : roadObject->getSegments())
			{
				XMLElement* segmentElement = doc.NewElement("Segment");

				std::string type;
				if (segment.type == RST_LINE)
					type = "line";
				else if (segment.type == RST_ARC)
					type = "arc";
				if (segment.type == RST_BEZIER_CURVE)
					type = "bezier";

				segmentElement->SetAttribute("type", type.c_str());
				segmentElement->SetAttribute("points", segment.pointsCount);

				if (segment.type != RST_BEZIER_CURVE)
				{
					segmentElement->SetAttribute("radius", segment.r);
					segmentElement->SetAttribute("interpolation", segment.interpolation == RI_LIN ? "lin" : "cos");
				}

				roadElement->InsertEndChild(segmentElement);
			}
		}

		for (int i = 0; i < 2; ++i)
		{
			RoadConnectionPoint& connectionPoint = roadObject->getConnectionPoint(i);
			if (connectionPoint.crossroadComponent != nullptr)
			{
				XMLElement* connectionElement = doc.NewElement("Connection");
				
				connectionElement->SetAttribute("type", i);
				connectionElement->SetAttribute("crossroadName", connectionPoint.crossroadComponent->getSceneObject()->getName().c_str());
				connectionElement->SetAttribute("index", connectionPoint.index);

				roadElement->InsertEndChild(connectionElement);
			}
		}

		roadsElement->InsertEndChild(roadElement);
	}
}


void SceneSaver::saveRoadIntersection(tinyxml2::XMLElement* roadsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject)
{
	XMLElement* objectElement = doc.NewElement("RoadIntersection");

	objectElement->SetAttribute("name", sceneObject->getName().c_str());
	objectElement->SetAttribute("position", vec3ToString(sceneObject->getPosition()).c_str());

	RoadIntersectionComponent* roadIntersectionComponent = static_cast<RoadIntersectionComponent*>(sceneObject->getComponent(CT_ROAD_INTERSECTION));
	const auto& connectedRoads = roadIntersectionComponent->getConnectedRoads();

	objectElement->SetAttribute("quality", roadIntersectionComponent->getQuality());
	objectElement->SetAttribute("profile", roadIntersectionComponent->getEdgeRoadProfile()->getName().c_str());

	for (int i = 0; i < connectedRoads.size(); ++i)
	{
		const auto& road = connectedRoads[i];

		XMLElement* roadElement = doc.NewElement("ConnectedRoad");

		roadElement->SetAttribute("name", road.road->getSceneObject()->getName().c_str());
		roadElement->SetAttribute("index", road.connectionPointInRoadIndex);
		roadElement->SetAttribute("length", roadIntersectionComponent->getLength(i));
		roadElement->SetAttribute("width", roadIntersectionComponent->getWidth(i));
		roadElement->SetAttribute("arc", roadIntersectionComponent->getArc(i));

		objectElement->InsertEndChild(roadElement);
	}

	roadsElement->InsertEndChild(objectElement);
}


void SceneSaver::saveSceneObject(XMLDocument& doc, XMLElement* parentElement, SceneObject* sceneObject, SceneObject* parentObject/* = nullptr*/)
{
	if (sceneObject->getFlags() & SOF_NOT_SERIALIZABLE)
	{
		LOG_DEBUG("Skip object: " + sceneObject->getName());
		return;
	}

	RObject* objectDefinition = sceneObject->getObjectDefinition();
	if (objectDefinition == nullptr)
	{
		if (sceneObject->getComponent(CT_ROAD_OBJECT) != nullptr)
		{
			saveRoad(_roadsElement, doc, sceneObject);
		}
		else if (sceneObject->getComponent(CT_ROAD_INTERSECTION) != nullptr)
		{
			saveRoadIntersection(_roadsElement, doc, sceneObject);
		}
		else if (sceneObject->getComponent(CT_GRASS) != nullptr)
		{
			if (_grassElement)
			{
				_rootNode->InsertAfterChild(_terrainElement, _grassElement);
				saveGrass(_grassElement, doc, sceneObject);
			}
		}
		else if (sceneObject->getComponent(CT_TERRAIN) != nullptr)
		{
			saveTerrain(_terrainElement, _grassElement, sceneObject);
		}
		else if (sceneObject->getName() == "sky")
		{
			saveSky(_skyElement, sceneObject);
		}
		else if (sceneObject->getName() == "sun")
		{
			saveSunLight(_sunElement, sceneObject);
		}
		else
		{
			saveObject(parentElement, doc, sceneObject, nullptr);
		}
	}
	else if (sceneObject->getParent() == parentObject)
	{
		saveObject(parentElement, doc, sceneObject, objectDefinition);
	}
}


void SceneSaver::saveMap(std::string name, const ResourceDescription& sceneDescription)
{
	_dirPath = GameDirectories::MAPS + name + "/";

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isDirectoryExists(_dirPath))
		_dirPath = ResourceManager::getInstance().getAlternativeResourcePath() + _dirPath;
#endif // DEVELOPMENT_RESOURCES

	std::string fullPath = _dirPath + MAP_FILE_NAME;

	LOG_INFO("Map path: " + fullPath);

	XMLDocument doc;

	XMLDeclaration* declaration = doc.NewDeclaration();
	doc.InsertFirstChild(declaration);

	XMLNode* rootNode = doc.NewElement("Scene");
	doc.InsertEndChild(rootNode);

	XMLElement* descriptionElement = doc.NewElement("Description");
	rootNode->InsertEndChild(descriptionElement);

	_terrainElement = doc.NewElement("Terrain");
	rootNode->InsertEndChild(_terrainElement);

	_grassElement = doc.NewElement("Grass");

	_sunElement = doc.NewElement("Light");
	rootNode->InsertEndChild(_sunElement);

	_skyElement = doc.NewElement("Sky");
	rootNode->InsertEndChild(_skyElement);

	_objectsElement = doc.NewElement("Objects");
	rootNode->InsertEndChild(_objectsElement);

	_roadsElement = doc.NewElement("Roads");
	_roadsElement->SetAttribute("version", "2");
	rootNode->InsertEndChild(_roadsElement);
	
	ResourceDescriptionUtils::saveResourceDescription(descriptionElement, sceneDescription);

	for (SceneObject* sceneObject : _sceneManager->getSceneObjects())
	{
		if (sceneObject->getParent() == nullptr)
		{
			saveSceneObject(doc, _objectsElement, sceneObject);
		}
	}

	XMLError errorCode = doc.SaveFile(fullPath.c_str());

	LOG_INFO(LOG_VARIABLE((int)errorCode));
}
