#ifndef BUSREPAINTLOADER_H_INCLUDED
#define BUSREPAINTLOADER_H_INCLUDED


#include <string>
#include <vector>

#include "BusRepaintDescription.h"

#include "../Graphics/RMaterialsCollection.h"

#include "../Utils/RTexture2D.h"


namespace BusRepaintLoader
{
	void loadBusRepaint(const std::string& busName, const std::string& repaintName, std::vector<RMaterialsCollection*>& outMaterialsCollections);

	void loadAllRepaintsDescriptions(const std::string& busName, std::vector<BusRepaintDescription>& outRepaints);
}


#endif // BUSREPAINTLOADER_H_INCLUDED
