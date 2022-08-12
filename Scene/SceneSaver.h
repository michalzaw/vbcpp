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
	class XMLNode;
}

class SceneManager;
struct RoadLane;
class RObject;
class SceneObject;
struct ResourceDescription;


class SceneSaver
{
	private:
		static constexpr char* MAP_FILE_NAME = "scene.xml";

		SceneManager* _sceneManager;
		std::string _dirPath;

		tinyxml2::XMLNode* _rootNode;
		tinyxml2::XMLElement* _terrainElement;
		tinyxml2::XMLElement* _startPositionElement;
		tinyxml2::XMLElement* _grassElement;
		tinyxml2::XMLElement* _sunElement;
		tinyxml2::XMLElement* _skyElement;
		tinyxml2::XMLElement* _objectsElement;
		tinyxml2::XMLElement* _roadsElement;

		std::string createSkyTextureAttribute(std::string path);

		void saveStartPosition(tinyxml2::XMLElement* startPositionElement);
		void saveTerrain(tinyxml2::XMLElement* terrainElement, tinyxml2::XMLElement* grassElement, SceneObject* sceneObject);
		void saveGrass(tinyxml2::XMLElement* grassElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);
		void saveSunLight(tinyxml2::XMLElement* sunElement, SceneObject* sceneObject);
		void saveSky(tinyxml2::XMLElement* skyElement, SceneObject* sceneObject);
		void saveObject(tinyxml2::XMLElement* objectsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject, RObject* objectDefinition);
		void saveRoad(tinyxml2::XMLElement* roadsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);
		void saveRoadIntersection(tinyxml2::XMLElement* roadsElement, tinyxml2::XMLDocument& doc, SceneObject* sceneObject);

		void saveSceneObject(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parentElement, SceneObject* sceneObject, SceneObject* parentObject = nullptr);

	public:
		SceneSaver(SceneManager* sceneManager);

		void saveMap(std::string name, const ResourceDescription& sceneDescription);

};


#endif // SCENESAVER_H_INCLUDED
