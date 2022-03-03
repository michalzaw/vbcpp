#include "ResourceDescriptionUtils.h"

#include "Logger.h"
#include "XmlUtils.h"

#include "../Game/Directories.h"

#include "../Utils/tinyxml2.h"


namespace ResourceDescriptionUtils
{
	void loadResourceDescription(tinyxml2::XMLElement* descriptionElement, ResourceDescription& outDescription)
	{
		if (descriptionElement != nullptr)
		{
			outDescription.author = XmlUtils::getAttributeStringOptional(descriptionElement, "author");
			outDescription.name = XmlUtils::getAttributeStringOptional(descriptionElement, "name");
			outDescription.comment = XmlUtils::getAttributeStringOptional(descriptionElement, "comment");
		}
		else
		{
			outDescription.author = "";
			outDescription.name = "";
			outDescription.comment = "";
		}
	}


	void loadResourceDescription(const std::string& fileName, const std::string& rootElementName, ResourceDescription& outDescription)
	{
		XMLDocument doc;
		XMLError result = doc.LoadFile(fileName.c_str());
		if (result != XML_SUCCESS)
		{
			LOG_ERROR("Cannot read xml file: " + fileName + "! Result: " + Strings::toString((int)result));
		}

		// Search for main element
		XMLElement* scnElement = doc.FirstChildElement(rootElementName.c_str());
		if (scnElement == nullptr)
		{
			LOG_ERROR("Root element not found!");
			return;
		}

		XMLElement* descriptionElement = scnElement->FirstChildElement("Description");

		loadResourceDescription(descriptionElement, outDescription);
	}


	void saveResourceDescription(tinyxml2::XMLElement* descriptionElement, const ResourceDescription& sceneDescription)
	{
		if (!sceneDescription.author.empty())
		{
			descriptionElement->SetAttribute("author", sceneDescription.author.c_str());
		}
		if (!sceneDescription.name.empty())
		{
			descriptionElement->SetAttribute("name", sceneDescription.name.c_str());
		}
		if (!sceneDescription.comment.empty())
		{
			descriptionElement->SetAttribute("comment", sceneDescription.comment.c_str());
		}
	}
}
