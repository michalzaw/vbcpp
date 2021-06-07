#include "BusRepaintLoader.h"

#include "../Game/Directories.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"
#include "../Utils/tinyxml2.h"


namespace BusRepaintLoader
{
	const std::string REPAINTS_DIRECTORY = "Repaints";
	const std::string CONFIG_FILENAME = "config.xml";

	void loadBusRepaint(const std::string& busName, const std::string& repaintName, std::vector<RMaterialsCollection*>& outMaterialsCollections)
	{
		std::string repaintPath = GameDirectories::BUSES + busName + "/" + REPAINTS_DIRECTORY + "/" + repaintName;
		std::string configFileName = repaintPath + "/" + CONFIG_FILENAME;

#ifdef DEVELOPMENT_RESOURCES
		if (!FilesHelper::isFileExists(configFileName))
			configFileName = ResourceManager::getInstance().getAlternativeResourcePath() + configFileName;
#endif // DEVELOPMENT_RESOURCES

		XMLDocument doc;
		XMLError result = doc.LoadFile(configFileName.c_str());
		if (result != XML_SUCCESS)
		{
			Logger::error("Cannot read xml file: " + configFileName + "! Result: " + toString(result));
			return;
		}

		XMLElement* rootElement = doc.FirstChildElement("Repaint");

		Logger::info("Bus repaint configurations XML DATA");

		outMaterialsCollections.clear();

		for (XMLElement* materialsElement = rootElement->FirstChildElement("Materials");
			materialsElement != nullptr;
			materialsElement = materialsElement->NextSiblingElement("Materials"))
		{
			const std::string fileName = XmlUtils::getAttributeString(materialsElement, "fileName");

			RMaterialsCollection* materialsCollection = ResourceManager::getInstance().loadMaterialsCollection(repaintPath + "/" + fileName);
			outMaterialsCollections.push_back(materialsCollection);
		}
	}
}