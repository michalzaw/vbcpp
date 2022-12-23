#ifndef OBJECTSGENERATOR_H_INCLUDED
#define OBJECTSGENERATOR_H_INCLUDED


#include <vector>
#include <string>

#include <glm/glm.hpp>


class BezierCurve;
class SceneManager;


namespace ObjectsGenerator
{
	struct ObjectsAlongCurveGeneratorData
	{
		std::string objectsCollectionName;
		
		std::vector<std::string> objectsNames;

		float distance;
		glm::vec2 offsetFromCenter;
		glm::vec3 rotation;
		bool rightSide;
		bool leftSide;

		glm::vec3 positionOffset;
		glm::vec3 rotationOffset;

		ObjectsAlongCurveGeneratorData()
			: distance(10), offsetFromCenter(0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), rightSide(true), leftSide(false),
			positionOffset(0.0f, 0.0f, 0.0f), rotationOffset(0.0f, 0.0f, 0.0f)
		{}

	};

	void generateObjectsAlongCurve(BezierCurve* bezierCurve, ObjectsAlongCurveGeneratorData* generatorData, SceneManager* sceneManager);
}


#endif // OBJECTSGENERATOR_H_INCLUDED
