#include "XmlUtils.h"

namespace XmlUtils
{

	std::string getAttributeString(XMLElement* xmlElement, const char* attributeName)
	{
		return std::string(xmlElement->Attribute(attributeName));
	}

	float getAttributeFloat(XMLElement* xmlElement, const char* attributeName)
	{
		return (float)atof(xmlElement->Attribute(attributeName));
	}

	int getAttributeInt(XMLElement* xmlElement, const char* attributeName)
	{
		return (int)atoi(xmlElement->Attribute(attributeName));
	}

	bool getAttributeBool(XMLElement* xmlElement, const char* attributeName)
	{
		const char* value = xmlElement->Attribute(attributeName);
		return strcmp(value, "true") == 0;
	}

	glm::vec2 getAttributeVec2(XMLElement* xmlElement, const char* attributeName)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		return XMLstringToVec2(cValue);

	}
	glm::vec3 getAttributeVec3(XMLElement* xmlElement, const char* attributeName)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		return XMLstringToVec3(cValue);

	}
	glm::vec4 getAttributeVec4(XMLElement* xmlElement, const char* attributeName)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		return XMLstringToVec4(cValue);

	}
	btVector3 getAttributeBtVector3(XMLElement* xmlElement, const char* attributeName)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		return XMLstringToBtVec3(cValue);

	}

	std::string getAttributeStringOptional(XMLElement* xmlElement, const char* attributeName, std::string defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return std::string(cValue);
		else
			return defaultValue;
	}

	float getAttributeFloatOptional(XMLElement* xmlElement, const char* attributeName, float defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return (float)atof(cValue);
		else
			return defaultValue;
	}

	int getAttributeIntOptional(XMLElement* xmlElement, const char* attributeName, int defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return (int)atoi(cValue);
		else
			return defaultValue;
	}

	bool getAttributeBoolOptional(XMLElement* xmlElement, const char* attributeName, bool defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return strcmp(cValue, "true") == 0;
		else
			return defaultValue;
	}

	glm::vec2 getAttributeVec2Optional(XMLElement* xmlElement, const char* attributeName, glm::vec2 defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return XMLstringToVec2(cValue);
		else
			return defaultValue;
	}
	glm::vec3 getAttributeVec3Optional(XMLElement* xmlElement, const char* attributeName, glm::vec3 defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return XMLstringToVec3(cValue);
		else
			return defaultValue;
	}
	glm::vec4 getAttributeVec4Optional(XMLElement* xmlElement, const char* attributeName, glm::vec4 defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return XMLstringToVec4(cValue);
		else
			return defaultValue;
	}
	btVector3 getAttributeBtVector3Optional(XMLElement* xmlElement, const char* attributeName, btVector3 defaultValue)
	{
		const char* cValue = xmlElement->Attribute(attributeName);
		if (cValue != NULL)
			return XMLstringToBtVec3(cValue);
		else
			return defaultValue;
	}

} // namespace