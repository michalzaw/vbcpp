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
struct ResourceDescription;
class Prefab;


class SceneSaver
{
	private:
		static constexpr char* MAP_FILE_NAME = "scene.xml";

		SceneManager* _sceneManager;
		std::string _dirPath;

		std::string createSkyTextureAttribute(std::string path);

		void saveStartPosition(tinyxml2::XMLElement* startPositionElement);
		void saveTerrain(tinyxml2::XMLElement* terrainElement, tinyxml2::XMLElement* grassElement, SceneObject* sceneObject);
		void saveGrass(tinyxml2::XMLElement* grassElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);
		void saveSunLight(tinyxml2::XMLElement* sunElement, SceneObject* sceneObject);
		void saveSky(tinyxml2::XMLElement* skyElement, SceneObject* sceneObject);
		void saveObject(tinyxml2::XMLElement* objectsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject, RObject* objectDefinition);
		void saveRoad(tinyxml2::XMLElement* roadsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);
		void saveRoadIntersection(tinyxml2::XMLElement* roadsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);

		void savePrefabComponent(tinyxml2::XMLElement* objectElement, tinyxml2::XMLDocument& doc, Prefab* prefab);

	public:
		SceneSaver(SceneManager* sceneManager);

		void saveMap(std::string name, const ResourceDescription& sceneDescription);

};


#endif // SCENESAVER_H_INCLUDED
