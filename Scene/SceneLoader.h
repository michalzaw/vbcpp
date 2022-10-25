#ifndef SCENELOADER_H_INCLUDED
#define SCENELOADER_H_INCLUDED


#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "../Utils/ResourceDescriptionUtils.h"


namespace tinyxml2
{
	class XMLElement;
}

class SceneManager;
struct RoadLane;
class RObject;
class SceneObject;
struct CrossroadComponent;
class RoadObject;


class SceneLoader
{
	private:
		static constexpr char* MAP_FILE_NAME = "scene.xml";

		int _terrainCollidesWith;
		int _roadCollidesWith;

		SceneManager* _sceneManager;

		std::string _dirPath;

		ResourceDescription _sceneDescription;

		void loadStartPosition(tinyxml2::XMLElement* );
		bool loadTerrain(tinyxml2::XMLElement* );
		void loadGrass(tinyxml2::XMLElement* );
		void loadSunLight(tinyxml2::XMLElement* );
		void loadSky(tinyxml2::XMLElement* );
		void loadObjects(tinyxml2::XMLElement* , SceneObject* parent = nullptr);
		void loadObject(tinyxml2::XMLElement* , SceneObject* parent = nullptr);
		void loadRoads(tinyxml2::XMLElement* );
		void loadRoad(tinyxml2::XMLElement*);
		void loadRoadV2(tinyxml2::XMLElement*);
		void loadRoadIntersection(tinyxml2::XMLElement*);

		CrossroadComponent* findCrossRoadComponentBySceneObjectName(const std::string& name);
		RoadObject* findRoadObjectBySceneObjectName(const std::string& name);

		void loadPrefabComponent(tinyxml2::XMLElement* componentElement, SceneObject* sceneObject);
		void loadPathComponent(tinyxml2::XMLElement* componentElement, SceneObject* sceneObject);

	public:
		SceneLoader(SceneManager* sceneManager);

		void loadMap(std::string name);

		ResourceDescription& getLoadedSceneDescription();

};


#endif // SCENELOADER_H_INCLUDED
