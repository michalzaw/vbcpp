#ifndef XMLUTILS_H_INCLUDED
#define XMLUTILS_H_INCLUDED


#include "Helpers.hpp"

#include "tinyxml2.h"
using namespace tinyxml2;


namespace XmlUtils
{
std::string getAttributeString(XMLElement* xmlElement, const char* attributeName);


float getAttributeFloat(XMLElement* xmlElement, const char* attributeName);


int getAttributeInt(XMLElement* xmlElement, const char* attributeName);


bool getAttributeBool(XMLElement* xmlElement, const char* attributeName);
 

glm::vec2 getAttributeVec2(XMLElement* xmlElement, const char* attributeName);

glm::vec3 getAttributeVec3(XMLElement* xmlElement, const char* attributeName);

glm::vec4 getAttributeVec4(XMLElement* xmlElement, const char* attributeName);

btVector3 getAttributeBtVector3(XMLElement* xmlElement, const char* attributeName);


std::string getAttributeStringOptional(XMLElement* xmlElement, const char* attributeName, std::string defaultValue = "");


float getAttributeFloatOptional(XMLElement* xmlElement, const char* attributeName, float defaultValue = 0.0f);


int getAttributeIntOptional(XMLElement* xmlElement, const char* attributeName, int defaultValue = 0);


bool getAttributeBoolOptional(XMLElement* xmlElement, const char* attributeName, bool defaultValue = false);


glm::vec2 getAttributeVec2Optional(XMLElement* xmlElement, const char* attributeName, glm::vec2 defaultValue = glm::vec2(0, 0));

glm::vec3 getAttributeVec3Optional(XMLElement* xmlElement, const char* attributeName, glm::vec3 defaultValue = glm::vec3(0, 0, 0));

glm::vec4 getAttributeVec4Optional(XMLElement* xmlElement, const char* attributeName, glm::vec4 defaultValue = glm::vec4(0, 0, 0, 0));

btVector3 getAttributeBtVector3Optional(XMLElement* xmlElement, const char* attributeName, btVector3 defaultValue = btVector3(0, 0, 0));
} // namespace


#endif // XMLUTILS_H_INCLUDED
