#include "SceneSaver.h"

#include <algorithm>

#include "SceneManager.h"
#include "SceneLoader.h"

#include "../Game/Directories.h"

#include "../Graphics/RenderObject.h"

#include "../Utils/Logger.h"
#include "../Utils/Strings.h"


SceneSaver::SceneSaver(SceneManager* sceneManager)
	: _sceneManager(sceneManager)
{

}


std::string SceneSaver::getRelativePathToDir(std::string filePath, std::string dirPath)
{
	std::string dirPathFromFilePath = filePath.substr(0, dirPath.size());
	if (dirPath != dirPathFromFilePath)
		Logger::error("Scene saver: Invalid dirPath");

	return filePath.substr(dirPath.size());
}


std::string SceneSaver::createSkyTextureAttribute(std::string path)
{
	if (path[path.size() - 1] == ';')
		path = path.substr(0, path.size() - 1);

	std::vector<std::string> textures = split(path, ';');

	if (textures.size() != 6)
		Logger::error("Scene saver: Invalid skybox textures path");

	std::string result = getRelativePathToDir(textures[0], GameDirectories::SKYBOX) + "," + getRelativePathToDir(textures[1], GameDirectories::SKYBOX) + "," +
						 getRelativePathToDir(textures[2], GameDirectories::SKYBOX) + "," + getRelativePathToDir(textures[3], GameDirectories::SKYBOX) + "," +
						 getRelativePathToDir(textures[4], GameDirectories::SKYBOX) + "," + getRelativePathToDir(textures[5], GameDirectories::SKYBOX);

	return result;
}


void SceneSaver::saveDescription(XMLElement* descriptionElement, const SceneDescription& sceneDescription)
{
	descriptionElement->SetAttribute("author", sceneDescription.author.c_str());
}


void SceneSaver::saveStartPosition(XMLElement* startPositionElement)
{
	startPositionElement->SetAttribute("position", vec3ToString(_sceneManager->getBusStart().position).c_str());
	startPositionElement->SetAttribute("rotation", vec3ToString(_sceneManager->getBusStart().rotation).c_str());
}


void SceneSaver::saveTerrain(XMLElement* terrainElement, XMLElement* grassElement, SceneObject* sceneObject)
{
	Terrain* terrainComponent = static_cast<Terrain*>(sceneObject->getComponent(CT_TERRAIN));

	terrainElement->SetAttribute("heightmap", terrainComponent->getHeightmapFileName().c_str());
	terrainElement->SetAttribute("material", terrainComponent->getMaterialName().c_str());
	terrainElement->SetAttribute("maxHeight", terrainComponent->getMaxHeight());

	grassElement->SetAttribute("terrain_heightmap", terrainComponent->getHeightmapFileName().c_str());
}


void SceneSaver::saveGrass(XMLElement* grassElement, SceneObject* sceneObject)
{
	Grass* grassComponent = static_cast<Grass*>(sceneObject->getComponent(CT_GRASS));

	grassElement->SetAttribute("model", getRelativePathToDir(grassComponent->getModel()->getPath(), _dirPath + "grass/").c_str());
	grassElement->SetAttribute("density_texture", getRelativePathToDir(grassComponent->getGrassDensityTexture()->getPath(), _dirPath).c_str());
}


void SceneSaver::saveSunLight(XMLElement* sunElement, SceneObject* sceneObject)
{
	Light* light = static_cast<Light*>(sceneObject->getComponent(CT_LIGHT));

	sunElement->SetAttribute("rotation", vec3ToString(sceneObject->getRotation()).c_str());
	sunElement->SetAttribute("color", vec3ToString(light->getColor()).c_str());
	sunElement->SetAttribute("ambientIntensity", light->getAmbientIntensity());
	sunElement->SetAttribute("diffuseIntensity", light->getDiffiseIntenisty());
}


void SceneSaver::saveSky(XMLElement* skyElement, SceneObject* sceneObject)
{
	RenderObject* renderObject = static_cast<RenderObject*>(sceneObject->getComponent(CT_RENDER_OBJECT));

	std::string skyboxTexturePaths = renderObject->getMaterial(0)->diffuseTexture->getPath();
	skyElement->SetAttribute("texture", createSkyTextureAttribute(skyboxTexturePaths).c_str());
}


void SceneSaver::saveObject(XMLElement* objectsElement, XMLDocument& doc, SceneObject* sceneObject, RObject* objectDefinition)
{
	XMLElement* objectElement = doc.NewElement("Object");

	objectElement->SetAttribute("name", objectDefinition->getName().c_str());
	objectElement->SetAttribute("position", vec3ToString(sceneObject->getPosition()).c_str());
	glm::vec3 rotation = glm::vec3(radToDeg(sceneObject->getRotation().x),
		radToDeg(sceneObject->getRotation().y),
		radToDeg(sceneObject->getRotation().z));
	objectElement->SetAttribute("rotation", vec3ToString(rotation).c_str());

	BusStopComponent* busStopComponent = static_cast<BusStopComponent*>(sceneObject->getComponent(CT_BUS_STOP));
	if (busStopComponent != nullptr)
	{
		XMLElement* componentElement = doc.NewElement("Component");

		componentElement->SetAttribute("type", "bus-stop");
		componentElement->SetAttribute("busStopName", busStopComponent->getName().c_str());
		componentElement->SetAttribute("id", busStopComponent->getId());
		componentElement->SetAttribute("announcement", busStopComponent->getAnnouncementFileName().c_str());

		objectElement->InsertEndChild(componentElement);
	}

	objectsElement->InsertEndChild(objectElement);
}


void SceneSaver::saveRoad(XMLElement* roadsElement, XMLDocument& doc, SceneObject* sceneObject)
{
	XMLElement* roadElement = doc.NewElement("Road");

	RoadObject* roadObject = static_cast<RoadObject*>(sceneObject->getComponent(CT_ROAD_OBJECT));
	RRoadProfile* profile = roadObject->getRoadProfile();

	roadElement->SetAttribute("name", sceneObject->getName().c_str());
	roadElement->SetAttribute("profile", profile->getName().c_str());

	for (const RoadSegment& segment : roadObject->getSegments())
	{
		XMLElement* segmentElement = doc.NewElement("Segment");

		segmentElement->SetAttribute("type", segment.type == RST_LINE ? "line" : "arc");
		segmentElement->SetAttribute("radius", segment.r);
		segmentElement->SetAttribute("beginPoint", vec3ToString(segment.begin).c_str());
		segmentElement->SetAttribute("endPoint", vec3ToString(segment.end).c_str());
		segmentElement->SetAttribute("points", segment.pointsCount);
		segmentElement->SetAttribute("interpolation", segment.interpolation == RI_LIN ? "lin" : "cos");

		roadElement->InsertEndChild(segmentElement);
	}

	roadsElement->InsertEndChild(roadElement);
}


void SceneSaver::saveMap(std::string name, const SceneDescription& sceneDescription)
{
	_dirPath = GameDirectories::MAPS + name + "/";
	std::string fullPath = _dirPath + MAP_FILE_NAME;

	XMLDocument doc;

	XMLDeclaration* declaration = doc.NewDeclaration();
	doc.InsertFirstChild(declaration);

	XMLNode* rootNode = doc.NewElement("Scene");
	doc.InsertEndChild(rootNode);

	XMLElement* descriptionElement = doc.NewElement("Description");
	rootNode->InsertEndChild(descriptionElement);

	XMLElement* terrainElement = doc.NewElement("Terrain");
	rootNode->InsertEndChild(terrainElement);

	XMLElement* startPositionElement = doc.NewElement("Start");
	rootNode->InsertEndChild(startPositionElement);

	XMLElement* grassElement = doc.NewElement("Grass");
	rootNode->InsertEndChild(grassElement);

	XMLElement* sunElement = doc.NewElement("Light");
	rootNode->InsertEndChild(sunElement);

	XMLElement* skyElement = doc.NewElement("Sky");
	rootNode->InsertEndChild(skyElement);

	XMLElement* objectsElement = doc.NewElement("Objects");
	rootNode->InsertEndChild(objectsElement);

	XMLElement* roadsElement = doc.NewElement("Roads");
	rootNode->InsertEndChild(roadsElement);
	
	saveDescription(descriptionElement, sceneDescription);
	saveStartPosition(startPositionElement);

	for (SceneObject* sceneObject : _sceneManager->getSceneObjects())
	{
		RObject* objectDefinition = sceneObject->getObjectDefinition();
		if (objectDefinition != nullptr)
		{
			saveObject(objectsElement, doc, sceneObject, objectDefinition);
		}
		else if (sceneObject->getComponent(CT_ROAD_OBJECT) != nullptr)
		{
			saveRoad(roadsElement, doc, sceneObject);
		}
		else if (sceneObject->getComponent(CT_GRASS) != nullptr)
		{
			saveGrass(grassElement, sceneObject);
		}
		else if (sceneObject->getComponent(CT_TERRAIN) != nullptr)
		{
			saveTerrain(terrainElement, grassElement, sceneObject);
		}
		else if (sceneObject->getName() == "sky")
		{
			saveSky(skyElement, sceneObject);
		}
		else if (sceneObject->getName() == "sun")
		{
			saveSunLight(sunElement, sceneObject);
		}
	}

	doc.SaveFile(fullPath.c_str());
}
