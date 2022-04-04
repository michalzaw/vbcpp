#ifndef RESOURCEDESCRIPTIONUTILS_H_INCLUDED
#define RESOURCEDESCRIPTIONUTILS_H_INCLUDED


#include "ResourceDescription.h"


namespace tinyxml2
{
	class XMLElement;
}


namespace ResourceDescriptionUtils
{
	void loadResourceDescription(tinyxml2::XMLElement* descriptionElement, ResourceDescription& outDescription);
	void loadResourceDescription(const std::string& fileName, const std::string& rootElementName, ResourceDescription& outDescription);

	void saveResourceDescription(tinyxml2::XMLElement* descriptionElement, const ResourceDescription& sceneDescription);
}


#endif // RESOURCEDESCRIPTIONUTILS_H_INCLUDED
