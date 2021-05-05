#include "BusConfigurationsLoader.h"

#include "../Game/Directories.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"
#include "../Utils/tinyxml2.h"


using namespace tinyxml2;


namespace BusConfigurationsLoader
{
	const std::string CONFIG_FILENAME = "predefinedConfigurations.xml";

	void loadVariables(XMLElement* element, std::unordered_map<std::string, std::string>& outVariables)
	{
		for (XMLElement* variableElement = element->FirstChildElement("Variable");
			variableElement != nullptr;
			variableElement = variableElement->NextSiblingElement("Variable"))
		{
			const std::string name = XmlUtils::getAttributeString(variableElement, "name");
			const std::string value = XmlUtils::getAttributeString(variableElement, "value");

			outVariables[name] = value;
		}
	}


	void loadBusPredefinedConfigurationByName(const std::string& busName, const std::string& configurationName, std::unordered_map<std::string, std::string>& outVariables)
	{
		std::string configFileName = GameDirectories::BUSES + busName + "/" + CONFIG_FILENAME;

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

		XMLElement* rootElement = doc.FirstChildElement("PredefinedConfigurations");

		Logger::info("Bus predefined configurations XML DATA");

		outVariables.clear();

		for (XMLElement* configurationElement = rootElement->FirstChildElement("Configuration");
			 configurationElement != nullptr;
			 configurationElement = configurationElement->NextSiblingElement("Configuration"))
		{
			const std::string name = XmlUtils::getAttributeString(configurationElement, "name");

			if (name == configurationName)
			{
				loadVariables(configurationElement, outVariables);
			}
		}
	}
}
