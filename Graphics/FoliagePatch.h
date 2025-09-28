#ifndef FOLIAGEPATCH_H_INCLUDED
#define FOLIAGEPATCH_H_INCLUDED


#include <ctime>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class SceneManager;


struct FoliagePatchLayer final
{
	std::string objectName;
	float minDistance;
	unsigned int seed;

	FoliagePatchLayer(const std::string& objectName, float minDistance, unsigned int seed = (unsigned int)std::time(0))
		: objectName(std::move(objectName)), minDistance(minDistance), seed(seed)
	{

	}

};


class FoliagePatch : public Component
{
	VBCPP_COMPONENT(FoliagePatch, CT_FOLIAGE_PATCH)

	private:
		std::vector<FoliagePatchLayer> _layers;

		void generateFoliage(const std::vector<glm::vec3>& polygonPoints, const FoliagePatchLayer& foliageData, SceneObject* parentObject, SceneManager* sceneManager);

	public:
		FoliagePatch();
		~FoliagePatch();

		FoliagePatchLayer& addLayer(const std::string& objectName, float minDistance = 5.0f);
		FoliagePatchLayer& getLayer(unsigned int index);
		void removeLayer(unsigned int index);

		unsigned int getLayersCount();

		void generateFoliage();

};


#endif // FOLIAGEPATCH_H_INCLUDED