#include "BusRepaintLoader.h"

#include "../Game/Directories.h"
#include "../Game/GameConfig.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger2.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"
#include "../Utils/tinyxml2.h"


namespace BusRepaintLoader
{
	const std::string REPAINTS_DIRECTORY = "Repaints";
	const std::string CONFIG_FILENAME = "config.xml";


	XMLElement* loadBusRepaintRootNodeRootNode(XMLDocument& doc, const std::string& repaintPath)
	{
		std::string configFileName = repaintPath + "/" + CONFIG_FILENAME;

#ifdef DEVELOPMENT_RESOURCES
		if (!FilesHelper::isFileExists(configFileName))
			configFileName = ResourceManager::getInstance().getAlternativeResourcePath() + configFileName;
#endif // DEVELOPMENT_RESOURCES

		XMLError result = doc.LoadFile(configFileName.c_str());
		if (result != XML_SUCCESS)
		{
			LOG_ERROR("Cannot read xml file: " + configFileName + "! Result: " + Strings::toString((int)result));
			return nullptr;
		}

		return doc.FirstChildElement("Repaint");
	}


	void loadBusRepaint(const std::string& busName, const std::string& repaintName, std::vector<RMaterialsCollection*>& outMaterialsCollections)
	{
		const std::string repaintPath = GameDirectories::BUSES + busName + "/" + REPAINTS_DIRECTORY + "/" + repaintName;

		XMLDocument doc;
		XMLElement* rootElement = loadBusRepaintRootNodeRootNode(doc, repaintPath);

		LOG_INFO("Bus repaint configurations XML DATA");

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


	BusRepaintDescription loadBusRepaintDescription(const std::string& busName, const std::string& repaintName)
	{
		const std::string repaintPath = GameDirectories::BUSES + busName + "/" + REPAINTS_DIRECTORY + "/" + repaintName;

		XMLDocument doc;
		XMLElement* rootElement = loadBusRepaintRootNodeRootNode(doc, repaintPath);

		LOG_INFO("Bus repaint configurations XML DATA");


		BusRepaintDescription description;
		description.repainDirectorytName = repaintName;
		description.name = XmlUtils::getAttributeString(rootElement, "name");

		XMLElement* descriptionElement = rootElement->FirstChildElement("Description");
		description.description = XmlUtils::getAttributeString(descriptionElement, "description");
		description.author = XmlUtils::getAttributeString(descriptionElement, "author");

		const std::string logoFileName = XmlUtils::getAttributeString(descriptionElement, "logo");
		if (!logoFileName.empty())
		{
			description.logo = ResourceManager::getInstance().loadTexture(repaintPath + "/" + logoFileName);
		}
		else
		{
			description.logo = ResourceManager::getInstance().loadDefaultWhiteTexture();
		}

		return description;
	}


	void loadAllRepaintsDescriptions(const std::string& busName, std::vector<BusRepaintDescription>& outRepaints)
	{
		LOG_INFO("Loading repaints list for bus: " + busName);

		// loading repaints names
		std::vector<std::string> repaintsNames;

		const std::string repaintsPath = GameDirectories::BUSES + busName + "/" + REPAINTS_DIRECTORY + "/";
		if (FilesHelper::isDirectoryExists(repaintsPath))
		{
			repaintsNames = FilesHelper::getDirectoriesList(repaintsPath);
		}
#ifdef DEVELOPMENT_RESOURCES
		else if (FilesHelper::isDirectoryExists(GameConfig::getInstance().alternativeResourcesPath + repaintsPath))
		{
			repaintsNames = FilesHelper::getDirectoriesList(GameConfig::getInstance().alternativeResourcesPath + repaintsPath);
		}
#endif // DEVELOPMENT_RESOURCES
		else
		{
			LOG_ERROR("Invalid repaints path: " + repaintsPath);
		}

		// loading descriptions
		LOG_INFO("Loading repaints descriptions for bus: " + busName);

		for (const std::string& repaintName : repaintsNames)
		{
			outRepaints.push_back(loadBusRepaintDescription(busName, repaintName));
		}
	}
}