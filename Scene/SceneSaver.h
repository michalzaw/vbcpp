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

		std::string getRelativePathToDir(std::string filePath, std::string dirPath);
		std::string createSkyTextureAttribute(std::string path);

	public:
		SceneSaver(SceneManager* sceneManager);

		void saveMap(std::string name);

};


#endif // SCENESAVER_H_INCLUDED
