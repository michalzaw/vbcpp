#ifndef RROADPROFILE_H_INCLUDED
#define RROADPROFILE_H_INCLUDED


#include <vector>

#include "Roads.h"

#include "../Utils/Resource.h"


enum RoadProfileType
{
	RPT_ROAD,
	RPT_PAVEMENT,

	RPT_COUNT
};


const std::string roadProfileTypeStrings[] = { "road", "pavement" };
RoadProfileType getRoadProfileTypeFromString(const std::string& name);


enum AIPathIntersectionMode
{
	APIM_INNER,
	APIM_OUTER,
	APIM_IGNORE,

	APIM_COUNT
};


const std::string aIPathIntersectionModeStrings[] = { "inner", "outer", "ignore" };
AIPathIntersectionMode getAIPathIntersectionModeFromString(const std::string& name);


struct AIPath final
{
	std::string name;
	float x;
	float y;
	int direction;
	AIPathIntersectionMode intersectionMode;

};


class RRoadProfile : public Resource
{
	private:
		std::string _author;
		std::string _name;
		std::string _comment;

		RoadProfileType _type;

		std::vector<RoadLane> _roadLanes;

		std::vector<glm::vec2> _edges; // todo: remove (unused)

		Material* _intersectionMaterial;
		float _intersectionRoadY;

		std::vector<AIPath> _aiPaths;

		float _maxX;			bool _maxXIsCalculated;
		float _minX;			bool _minXIsCalculated;

	public:
		RRoadProfile(std::string path, std::string author, std::string name, std::string comment, Material* intersectionMaterial = nullptr, float intersectionRoadY = 0.0f, RoadProfileType type = RPT_ROAD);
		~RRoadProfile();

		RoadProfileType getType();

		std::string getAuthor();
		std::string getName();
		std::string getComment();

		std::vector<RoadLane>& getRoadLanes();
		std::vector<glm::vec2>& getEdges();
		Material* getIntersectionMaterial();
		float getIntersectionRoadY();
		std::vector<AIPath>& getAIPaths();

		float getMaxX();
		float getMinX();

};


#endif // RROADPROFILE_H_INCLUDED
