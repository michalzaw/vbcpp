#ifndef SCENELOADER_H_INCLUDED
#define SCENELOADER_H_INCLUDED


#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>


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


struct SceneDescription
{
	std::string author;

	SceneDescription() {}
	SceneDescription(const std::string& author)
	{
		this->author = author;
	}
};


class SceneLoader
{
	private:
		static constexpr char* MAP_FILE_NAME = "scene.xml";

		int _terrainCollidesWith;
		int _roadCollidesWith;

		SceneManager* _sceneManager;

		std::string _dirPath;

		SceneDescription _sceneDescription;

		void loadStartPosition(tinyxml2::XMLElement* );
		bool loadTerrain(tinyxml2::XMLElement* );
		void loadGrass(tinyxml2::XMLElement* );
		void loadSunLight(tinyxml2::XMLElement* );
		void loadSky(tinyxml2::XMLElement* );
		void loadObjects(tinyxml2::XMLElement* );
		void loadObject(tinyxml2::XMLElement* );
		void loadRoads(tinyxml2::XMLElement* );
		void loadRoad(tinyxml2::XMLElement*);
		void loadRoadV2(tinyxml2::XMLElement*);
		void loadRoadIntersection(tinyxml2::XMLElement*);

		CrossroadComponent* findCrossRoadComponentBySceneObjectName(const std::string& name);
		RoadObject* findRoadObjectBySceneObjectName(const std::string& name);

	public:
		SceneLoader(SceneManager* sceneManager);

		void loadMap(std::string name);

		SceneDescription& getLoadedSceneDescription();

};


#endif // SCENELOADER_H_INCLUDED
