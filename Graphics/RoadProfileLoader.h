#ifndef ROADPROFILELOADER_H_INCLUDED
#define ROADPROFILELOADER_H_INCLUDED


#include "RRoadProfile.h"


#include <glm/glm.hpp>


namespace tinyxml2
{
	class XMLElement;
}


class RoadProfileLoader
{
	static constexpr char* PROFILE_FILE_NAME = "profile.xml";

	private:

	public:
		static RRoadProfile* loadRoadProfile(std::string dirPath);

};


#endif // ROADPROFILELOADER_H_INCLUDED
