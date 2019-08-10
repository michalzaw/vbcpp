#include "SceneSaver.h"

#include <algorithm>

#include "SceneManager.h"

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


void SceneSaver::saveMap(std::string name)
{
	std::string dirPath = GameDirectories::MAPS + name + "/";
	std::string fullPath = dirPath + MAP_FILE_NAME;

	XMLDocument doc;

	XMLDeclaration* declaration = doc.NewDeclaration();
	doc.InsertFirstChild(declaration);

	XMLNode* rootNode = doc.NewElement("Scene");
	doc.InsertEndChild(rootNode);

	XMLElement* descriptionElement = doc.NewElement("Description");
	descriptionElement->SetAttribute("author", "");
	rootNode->InsertEndChild(descriptionElement);

	XMLElement* startPositionElement = doc.NewElement("Start");
	startPositionElement->SetAttribute("position", vec3ToString(_sceneManager->getBusStart().position).c_str());
	startPositionElement->SetAttribute("rotation", vec3ToString(_sceneManager->getBusStart().rotation).c_str());
	rootNode->InsertEndChild(startPositionElement);

	XMLElement* objectsElement = doc.NewElement("Objects");
	rootNode->InsertEndChild(objectsElement);

	XMLElement* roadsElement = doc.NewElement("Roads");
	rootNode->InsertEndChild(roadsElement);

	for (SceneObject* sceneObject : _sceneManager->getSceneObjects())
	{
		RObject* objectDefinition = sceneObject->getObjectDefinition();
		if (objectDefinition != nullptr)
		{
			XMLElement* objectElement = doc.NewElement("Object");

			//objectElement->SetAttribute("name", objectDefinition->getName().c_str());
			objectElement->SetAttribute("name", sceneObject->getName().c_str());
			objectElement->SetAttribute("position", vec3ToString(sceneObject->getPosition()).c_str());
			glm::vec3 rotation = glm::vec3(radToDeg(sceneObject->getRotation().x),
										   radToDeg(sceneObject->getRotation().y),
										   radToDeg(sceneObject->getRotation().z));
			objectElement->SetAttribute("rotation", vec3ToString(rotation).c_str());

			objectsElement->InsertEndChild(objectElement);
		}
		else if (sceneObject->getComponent(CT_GRASS) != nullptr)
		{
			XMLElement* grassElement = doc.NewElement("Grass");

			Grass* grassComponent = static_cast<Grass*>(sceneObject->getComponent(CT_GRASS));

			grassElement->SetAttribute("model", getRelativePathToDir(grassComponent->getModel()->getPath(), dirPath + "grass/").c_str());
			grassElement->SetAttribute("terrain_heightmap", getRelativePathToDir(grassComponent->getTerrainHeightmap()->getPath(), dirPath).c_str());
			grassElement->SetAttribute("density_texture", getRelativePathToDir(grassComponent->getGrassDensityTexture()->getPath(), dirPath).c_str());

			rootNode->InsertFirstChild(grassElement);
		}
		else if (sceneObject->getComponent(CT_ROAD_OBJECT) != nullptr)
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
		else if (sceneObject->getComponent(CT_TERRAIN) != nullptr)
		{
			XMLElement* terrainElement = doc.NewElement("Terrain");

			Terrain* terrainComponent = static_cast<Terrain*>(sceneObject->getComponent(CT_TERRAIN));

			terrainElement->SetAttribute("heightmap", terrainComponent->getHeightmapFileName().c_str());
			terrainElement->SetAttribute("material", terrainComponent->getMaterialName().c_str());
			terrainElement->SetAttribute("maxHeight", terrainComponent->getMaxHeight());

			rootNode->InsertFirstChild(terrainElement);
		}
		else if (sceneObject->getName() == "sky")
		{
			XMLElement* skyElement = doc.NewElement("Sky");

			RenderObject* renderObject = static_cast<RenderObject*>(sceneObject->getComponent(CT_RENDER_OBJECT));
			std::string skyboxTexturePaths = renderObject->getMaterial(0)->diffuseTexture->getPath();
			skyElement->SetAttribute("texture", createSkyTextureAttribute(skyboxTexturePaths).c_str());

			rootNode->InsertFirstChild(skyElement);
		}
		else if (sceneObject->getName() == "sun")
		{
			XMLElement* sunElement = doc.NewElement("Light");

			Light* light = static_cast<Light*>(sceneObject->getComponent(CT_LIGHT));
			sunElement->SetAttribute("rotation", vec3ToString(sceneObject->getRotation()).c_str());
			sunElement->SetAttribute("color", vec3ToString(light->getColor()).c_str());
			sunElement->SetAttribute("ambientIntensity", light->getAmbientIntensity());
			sunElement->SetAttribute("diffuseIntensity", light->getDiffiseIntenisty());

			rootNode->InsertFirstChild(sunElement);
		}
	}

	doc.SaveFile(fullPath.c_str());
}
