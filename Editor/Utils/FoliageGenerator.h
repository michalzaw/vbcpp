#ifndef FOLIAGESGENERATOR_H_INCLUDED
#define FOLIAGESGENERATOR_H_INCLUDED


#include <vector>
#include <string>

#include <glm/glm.hpp>


class SceneManager;
class SceneObject;


namespace FoliageGenerator
{
	struct FoliageGeneratorData final
	{
		std::string objectName;
		float minDistance;
	};

	void generateFoliage(const std::vector<glm::vec3>& polygonPoints, const FoliageGeneratorData& generatorData, SceneObject* parentObject, SceneManager* sceneManager);
}


#endif // FOLIAGESGENERATOR_H_INCLUDED
