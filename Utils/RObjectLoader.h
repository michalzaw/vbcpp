#ifndef ROBJECTLOADER_H_INCLUDED
#define ROBJECTLOADER_H_INCLUDED


#include "RObject.h"


#include <glm/glm.hpp>


namespace tinyxml2
{
	class XMLElement;
}

class SceneManager;
class SceneObject;
class RStaticModel;


class RObjectLoader
{
	static constexpr char* OBJECT_FILE_NAME = "object.xml";

	private:
		static void loadComponents(tinyxml2::XMLElement* objectElement, RObject* object);
		static void loadRenderComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadPhysicsComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadTreeComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadSoundComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadBusStopComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadEnvironmentCaptureComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadCrossroadComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadSkeletalAnimation(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadAiAgent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);

		static RStaticModel* loadModel(const std::string& modelPath, const std::string& objectDirPath, bool isAnimated, bool normalSmoothing, RStaticModel* hightPollyModel = nullptr);
	
	public:
		static RObject* loadObject(const std::string& dirPath, const std::string& originalName);

		static SceneObject* createSceneObjectFromRObject(RObject* objectDefinition, const std::string& name,
														 const glm::vec3& position, const glm::vec3& rotation, SceneManager* sceneManager);
};


#endif // ROBJECTLOADER_H_INCLUDED
