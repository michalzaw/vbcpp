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

	public:
		RRoadProfile(std::string path, std::string author, std::string name, std::string comment);

		std::string getAuthor();
		std::string getName();
		std::string getComment();

		std::vector<RoadLane>& getRoadLanes();

};


#endif // RROADPROFILE_H_INCLUDED
