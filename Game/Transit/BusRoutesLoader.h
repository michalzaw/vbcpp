#ifndef BUSROUTESLOADER_H_INCLUDED
#define BUSROUTESLOADER_H_INCLUDED


#include "BusRoutes.h"


namespace RoutesLoader
{
	BusRoutes* loadRoutes(const std::string& fileName);
}


#endif // BUSROUTESLOADER_H_INCLUDED
