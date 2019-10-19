#include "GameConfig.h"

#include "../Utils/Strings.h"


void GameConfig::loadGameConfig(const char* filename)
{
    XMLDocument doc;
    doc.LoadFile(filename);

    XMLElement* objElement = doc.FirstChildElement("Game");

    for (XMLElement* child = objElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        const char* ename = child->Name();

        if (strcmp(ename,"Resolution") == 0)
        {
            const char* cWidth = child->Attribute("x");
            const char* cHeight = child->Attribute("y");

            windowWidth = (int)atoi(cWidth);
            windowHeight = (int)atoi(cHeight);
        }
        else
        if (strcmp(ename,"Map") == 0)
        {
            mapFile = std::string(child->Attribute("name"));
        }
        else
        if (strcmp(ename,"Bus") == 0)
        {
            busModel = std::string(child->Attribute("model"));
        }
        else
        if (strcmp(ename,"Configuration") == 0)
        {
            for (XMLElement* configElement = child->FirstChildElement(); configElement != NULL; configElement = configElement->NextSiblingElement())
            {
                const char* ename = configElement->Name();

                if (strcmp(ename,"Fullscreen") == 0)
                {
                    isFullscreen = toBool(configElement->GetText());
                }
                else if (strcmp(ename,"MsaaAntialiasing") == 0)
                {
                    msaaAntialiasing = toBool(configElement->GetText());
                }
                else if (strcmp(ename,"MsaaAntialiasingLevel") == 0)
                {
                    msaaAntialiasingLevel = toInt(configElement->GetText());
                }
                else if (strcmp(ename,"Shadowmapping") == 0)
                {
                    isShadowmappingEnable = toBool(configElement->GetText());
                }
                else if (strcmp(ename,"ShadowmapSize") == 0)
                {
                    shadowmapSize = toInt(configElement->GetText());
                }
                else if (strcmp(ename,"Bloom") == 0)
                {
                    isBloomEnabled = toBool(configElement->GetText());
                }
                else if (strcmp(ename,"Grass") == 0)
                {
                    isGrassEnable = toBool(configElement->GetText());
                }
				else if (strcmp(ename, "PbrSupport") == 0)
				{
					pbrSupport = toBool(configElement->GetText());
				}
            }
        }
    }
}


void GameConfig::loadDevelopmentConfig(const char* filename)
{
	XMLDocument doc;
	doc.LoadFile(filename);

	XMLElement* objElement = doc.FirstChildElement("DevSettings");

	for (XMLElement* child = objElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
	{
		const char* ename = child->Name();

		if (strcmp(ename, "alternativeResourcesPath") == 0)
		{
			alternativeResourcesPath = std::string(child->GetText());
		}
	}
}


GameConfig* GameConfig::instance = NULL;
