#ifndef BUSREPAINTLOADER_H_INCLUDED
#define BUSREPAINTLOADER_H_INCLUDED


#include <string>
#include <vector>

#include "../Graphics/RMaterialsCollection.h"


namespace BusRepaintLoader
{
	void loadBusRepaint(const std::string& busName, const std::string& repaintName, std::vector<RMaterialsCollection*>& outMaterialsCollections);
}


#endif // BUSREPAINTLOADER_H_INCLUDED

