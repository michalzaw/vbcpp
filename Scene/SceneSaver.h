#ifndef SCENESAVER_H_INCLUDED
#define SCENESAVER_H_INCLUDED


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


class SceneSaver
{
	private:
		static constexpr char* MAP_FILE_NAME = "scene_new.xml";

		SceneManager* _sceneManager;

	public:
		SceneSaver(SceneManager* sceneManager);

		void saveMap(std::string name);

};


#endif // SCENESAVER_H_INCLUDED
