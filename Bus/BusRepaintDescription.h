#ifndef BUSREPAINTDESCRIPTION_H_INCLUDED
#define BUSREPAINTDESCRIPTION_H_INCLUDED


#include "../Utils/RTexture2D.h"


struct BusRepaintDescription final
{
	std::string repainDirectorytName;
	std::string name;
	std::string description;
	std::string author;
	RTexture2D* logo;
};


#endif // BUSREPAINTDESCRIPTION_H_INCLUDED
