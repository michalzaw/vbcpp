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


class SceneLoader
{
	private:
		static constexpr char* MAP_FILE_NAME = "scene.xml";

		int _terrainCollidesWith;
		int _roadCollidesWith;

		SceneManager* _sceneManager;

		std::string _dirPath;

		void loadStartPosition(tinyxml2::XMLElement* sceneElement);
		bool loadTerrain(tinyxml2::XMLElement* sceneElement);
		void loadGrass(tinyxml2::XMLElement* sceneElement);
		void loadSunLight(tinyxml2::XMLElement* sceneElement);
		bool loadSky(tinyxml2::XMLElement* sceneElement);
		void loadObjects(tinyxml2::XMLElement* sceneElement);
		void loadRoads(tinyxml2::XMLElement* sceneElement);

	public:
		SceneLoader(SceneManager* sceneManager);

		void loadMap(std::string name);

};


#endif // SCENELOADER_H_INCLUDED
