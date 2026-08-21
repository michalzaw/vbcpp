#ifndef BUSROUTESLOADER_H_INCLUDED
#define BUSROUTESLOADER_H_INCLUDED


#include "BusRoutes.h"


namespace BusRoutesLoader
{
	BusRoutes* loadRoutes(const std::string& dirPath, const std::string& fileName);
}


#endif // BUSROUTESLOADER_H_INCLUDED
