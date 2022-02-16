#ifndef RROADPROFILE_H_INCLUDED
#define RROADPROFILE_H_INCLUDED


#include <vector>

#include "Roads.h"

#include "../Utils/Resource.h"


class RRoadProfile : public Resource
{
	private:
		std::string _author;
		std::string _name;
		std::string _comment;

		std::vector<RoadLane> _roadLanes;

		std::vector<glm::vec2> _edges;

		Material* _intersectionMaterial;
		float _intersectionRoadY;

		float _maxX;			bool _maxXIsCalculated;
		float _minX;			bool _minXIsCalculated;

	public:
		RRoadProfile(std::string path, std::string author, std::string name, std::string comment, Material* intersectionMaterial = nullptr, float intersectionRoadY = 0.0f);
		~RRoadProfile();

		std::string getAuthor();
		std::string getName();
		std::string getComment();

		std::vector<RoadLane>& getRoadLanes();
		std::vector<glm::vec2>& getEdges();
		Material* getIntersectionMaterial();
		float getIntersectionRoadY();

		float getMaxX();
		float getMinX();

};


#endif // RROADPROFILE_H_INCLUDED
