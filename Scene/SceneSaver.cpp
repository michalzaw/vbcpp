#include "SceneSaver.h"

#include <algorithm>

#include "SceneManager.h"
#include "SceneLoader.h"

#include "../Game/Directories.h"

#include "../Graphics/RenderObject.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger.h"
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
		Logger::error("Scene saver: Invalid skybox textures path");

	std::string result = FilesHelper::getRelativePathToDir(textures[0], GameDirectories::SKYBOX) + "," + FilesHelper::getRelativePathToDir(textures[1], GameDirectories::SKYBOX) + "," +
						 FilesHelper::getRelativePathToDir(textures[2], GameDirectories::SKYBOX) + "," + FilesHelper::getRelativePathToDir(textures[3], GameDirectories::SKYBOX) + "," +
						 FilesHelper::getRelativePathToDir(textures[4], GameDirectories::SKYBOX) + "," + FilesHelper::getRelativePathToDir(textures[5], GameDirectories::SKYBOX);

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


void SceneSaver::saveObject(XMLElement* objectsElement, XMLDocument& doc, SceneObject* sceneObject, RObject* objectDefinition)
{
	XMLElement* objectElement = doc.NewElement("Object");

	if (objectElement)
	{
		objectElement->SetAttribute("name", objectDefinition->getName().c_str());

		if (objectDefinition->getName() != sceneObject->getName())
			objectElement->SetAttribute("id", sceneObject->getName().c_str());

		objectElement->SetAttribute("position", vec3ToString(sceneObject->getPosition()).c_str());
		glm::vec3 rotation = glm::vec3(radToDeg(sceneObject->getRotation().x),
			radToDeg(sceneObject->getRotation().y),
			radToDeg(sceneObject->getRotation().z));
		objectElement->SetAttribute("rotation", vec3ToString(rotation).c_str());
	}

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

	objectsElement->InsertEndChild(objectElement);
}


void SceneSaver::saveRoad(XMLElement* roadsElement, XMLDocument& doc, SceneObject* sceneObject)
{
	XMLElement* roadElement = doc.NewElement("Road");

	RoadObject* roadObject = static_cast<RoadObject*>(sceneObject->getComponent(CT_ROAD_OBJECT));
	
	if (roadObject)
	{
		RRoadProfile* profile = roadObject->getRoadProfile();

		roadElement->SetAttribute("name", sceneObject->getName().c_str());
		roadElement->SetAttribute("profile", profile->getName().c_str());

		for (const glm::vec3& point : roadObject->getPoints())
		{
			XMLElement* pointElement = doc.NewElement("Point");

			pointElement->SetText(vec3ToString(point).c_str());

			roadElement->InsertEndChild(pointElement);
		}

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


void SceneSaver::saveMap(std::string name, const SceneDescription& sceneDescription)
{
	_dirPath = GameDirectories::MAPS + name + "/";
	std::string fullPath = _dirPath + MAP_FILE_NAME;

	LOG_INFO("Map path: " + fullPath);

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

	XMLElement* sunElement = doc.NewElement("Light");
	rootNode->InsertEndChild(sunElement);

	XMLElement* skyElement = doc.NewElement("Sky");
	rootNode->InsertEndChild(skyElement);

	XMLElement* objectsElement = doc.NewElement("Objects");
	rootNode->InsertEndChild(objectsElement);

	XMLElement* roadsElement = doc.NewElement("Roads");
	roadsElement->SetAttribute("version", "2");
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
			if (grassElement)
			{
				rootNode->InsertAfterChild(startPositionElement, grassElement);
				saveGrass(grassElement, doc, sceneObject);
			}
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

	XMLError errorCode = doc.SaveFile(fullPath.c_str());

	LOG_INFO(LOG_VARIABLE((int)errorCode));
}
