#ifndef OBJECTSGENERATOR_H_INCLUDED
#define OBJECTSGENERATOR_H_INCLUDED


#include <vector>
#include <string>

#include <glm/glm.hpp>


class RoadObject;
class SceneManager;


namespace ObjectsGenerator
{
	struct ObjectsAlongRoadGeneratorData
	{
		std::string objectsCollectionName;
		
		std::vector<std::string> objectsNames;

		int distance;
		glm::vec2 offsetFromCenter;
		glm::vec3 rotation;
		bool rightSide;
		bool leftSide;

		glm::vec3 positionOffset;
		glm::vec3 rotationOffset;

		ObjectsAlongRoadGeneratorData()
			: distance(10), offsetFromCenter(0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), rightSide(true), leftSide(false),
			positionOffset(0.0f, 0.0f, 0.0f), rotationOffset(0.0f, 0.0f, 0.0f)
		{}

	};

	void generateObjectsAlongRoad(RoadObject* roadComponent, ObjectsAlongRoadGeneratorData* generatorData, SceneManager* sceneManager);
}


#endif // OBJECTSGENERATOR_H_INCLUDED
