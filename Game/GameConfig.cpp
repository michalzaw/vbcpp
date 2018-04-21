#include "GameConfig.h"


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
                    const char* value = configElement->GetText();
                    if (strcmp(value,"true") == 0)
                        isFullscreen = true;
                    else
                        isFullscreen = false;
                }
                else if (strcmp(ename,"Shadowmapping") == 0)
                {
                    const char* value = configElement->GetText();
                    if (strcmp(value,"true") == 0)
                        isShadowmappingEnable = true;
                    else
                        isShadowmappingEnable = false;
                }
                else
                if (strcmp(ename,"ShadowmapSize") == 0)
                {
                    shadowmapSize = (int)atoi(configElement->GetText());
                }
            }
        }
    }
}


GameConfig* GameConfig::instance = NULL;
