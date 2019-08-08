#ifndef ROBJECTLOADER_H_INCLUDED
#define ROBJECTLOADER_H_INCLUDED


#include "RObject.h"


namespace tinyxml2
{
	class XMLElement;
}


class RObjectLoader
{
	private:
		static void loadComponents(tinyxml2::XMLElement* objectElement, RObject* object);
		static void loadRenderComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadPhysicsComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadTreeComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadSoundComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadBusStopComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
		static void loadEnvironmentCaptureComponent(tinyxml2::XMLElement* componentElement, RObject* object, int componentIndex);
	
	public:
		static RObject* loadObject(std::string fullPath);
};


#endif // ROBJECTLOADER_H_INCLUDED
