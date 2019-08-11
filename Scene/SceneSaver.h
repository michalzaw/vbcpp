#ifndef SCENESAVER_H_INCLUDED
#define SCENESAVER_H_INCLUDED


#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "SceneLoader.h"


namespace tinyxml2
{
	class XMLElement;
	class XMLDocument;
}

class SceneManager;
struct RoadLane;
class RObject;
class SceneObject;
class SceneDescription;


class SceneSaver
{
	private:
		static constexpr char* MAP_FILE_NAME = "scene.xml";

		SceneManager* _sceneManager;
		std::string _dirPath;

		std::string getRelativePathToDir(std::string filePath, std::string dirPath);
		std::string createSkyTextureAttribute(std::string path);

		void saveDescription(tinyxml2::XMLElement* descriptionElement, const SceneDescription& sceneDescription);
		void saveStartPosition(tinyxml2::XMLElement* startPositionElement);
		void saveTerrain(tinyxml2::XMLElement* terrainElement, tinyxml2::XMLElement* grassElement, SceneObject* sceneObject);
		void saveGrass(tinyxml2::XMLElement* grassElement, SceneObject* sceneObject);
		void saveSunLight(tinyxml2::XMLElement* sunElement, SceneObject* sceneObject);
		void saveSky(tinyxml2::XMLElement* skyElement, SceneObject* sceneObject);
		void saveObject(tinyxml2::XMLElement* objectsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);
		void saveRoad(tinyxml2::XMLElement* roadsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);

	public:
		SceneSaver(SceneManager* sceneManager);

		void saveMap(std::string name, const SceneDescription& sceneDescription);

};


#endif // SCENESAVER_H_INCLUDED
