#ifndef XMLUTILS_H_INCLUDED
#define XMLUTILS_H_INCLUDED


#include "Helpers.hpp"


class XmlUtils
{
    public:
        static std::string getAttributeString(XMLElement* xmlElement, const char* attributeName)
        {
            return std::string(xmlElement->Attribute(attributeName));
        }

        static float getAttributeFloat(XMLElement* xmlElement, const char* attributeName)
        {
            return (float)atof(xmlElement->Attribute(attributeName));
        }

        static glm::vec2 getAttributeVec2(XMLElement* xmlElement, const char* attributeName)
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            return XMLstringToVec2(cValue);

        }
        static glm::vec3 getAttributeVec3(XMLElement* xmlElement, const char* attributeName)
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            return XMLstringToVec3(cValue);

        }
        static glm::vec4 getAttributeVec4(XMLElement* xmlElement, const char* attributeName)
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            return XMLstringToVec4(cValue);

        }
        static btVector3 getAttributeBtVector3(XMLElement* xmlElement, const char* attributeName)
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            return XMLstringToBtVec3(cValue);

        }

        static std::string getAttributeStringOptional(XMLElement* xmlElement, const char* attributeName, std::string defaultValue = "")
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            if (cValue != NULL)
                return std::string(cValue);
            else
                return defaultValue;
        }

        static float getAttributeFloatOptional(XMLElement* xmlElement, const char* attributeName, float defaultValue = 0.0f)
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            if (cValue != NULL)
                return (float)atof(cValue);
            else
                return defaultValue;
        }

        static glm::vec2 getAttributeVec2Optional(XMLElement* xmlElement, const char* attributeName, glm::vec2 defaultValue = glm::vec2(0, 0))
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            if (cValue != NULL)
                return XMLstringToVec2(cValue);
            else
                return defaultValue;
        }
        static glm::vec3 getAttributeVec3Optional(XMLElement* xmlElement, const char* attributeName, glm::vec3 defaultValue = glm::vec3(0, 0, 0))
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            if (cValue != NULL)
                return XMLstringToVec3(cValue);
            else
                return defaultValue;
        }
        static glm::vec4 getAttributeVec4Optional(XMLElement* xmlElement, const char* attributeName, glm::vec4 defaultValue = glm::vec4(0, 0, 0, 0))
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            if (cValue != NULL)
                return XMLstringToVec4(cValue);
            else
                return defaultValue;
        }
        static btVector3 getAttributeBtVector3Optional(XMLElement* xmlElement, const char* attributeName, btVector3 defaultValue = btVector3(0, 0, 0))
        {
            const char* cValue = xmlElement->Attribute(attributeName);
            if (cValue != NULL)
                return XMLstringToBtVec3(cValue);
            else
                return defaultValue;
        }

};


#endif // XMLUTILS_H_INCLUDED
