#include "GameConfig.h"

#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"


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
            busRepaint = XmlUtils::getAttributeStringOptional(child, "repaint");
            busConfiguration = XmlUtils::getAttributeStringOptional(child, "configuration");
        }
        else
        if (strcmp(ename,"Configuration") == 0)
        {
            for (XMLElement* configElement = child->FirstChildElement(); configElement != NULL; configElement = configElement->NextSiblingElement())
            {
                const char* ename = configElement->Name();

                if (strcmp(ename,"Fullscreen") == 0)
                {
                    fullscreenMode = toInt(configElement->GetText());
                }
				if (strcmp(ename, "VerticalSync") == 0)
				{
					verticalSync = toBool(configElement->GetText());
				}
				else if (strcmp(ename, "HdrQuality") == 0)
				{
					hdrQuality = toInt(configElement->GetText());
				}
                else if (strcmp(ename,"MsaaAntialiasing") == 0)
                {
                    msaaAntialiasing = toBool(configElement->GetText());
                }
                else if (strcmp(ename,"MsaaAntialiasingLevel") == 0)
                {
                    msaaAntialiasingLevel = toInt(configElement->GetText());
                }
                else if (strcmp(ename, "Fxaa") == 0)
                {
                    isFxaaEnabled = toBool(configElement->GetText());
                }
                else if (strcmp(ename, "FxaaQuality") == 0)
                {
                    fxaaQuality = toInt(configElement->GetText());
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
				else if (strcmp(ename, "GrassRenderingDistance") == 0)
				{
					grassRenderingDistance = toFloat(configElement->GetText());
				}
				else if (strcmp(ename, "Mirrors") == 0)
				{
					isMirrorsEnabled = toBool(configElement->GetText());
				}
                else if (strcmp(ename, "MirrorSize") == 0)
                {
                    mirrorSize = toFloat(configElement->GetText());
                }
				else if (strcmp(ename, "MirrorRenderingDistance") == 0)
				{
					mirrorRenderingDistance = toFloat(configElement->GetText());
				}
                else if (strcmp(ename, "MirrorRefreshDistance") == 0)
                {
                    mirrorRefreshDistance = toFloat(configElement->GetText());
                }
				else if (strcmp(ename, "TextureCompression") == 0)
				{
					textureCompression = toBool(configElement->GetText());
				}
				else if (strcmp(ename, "AnisotropicFiltering") == 0)
				{
					anisotropicFiltering = toBool(configElement->GetText());
				}
				else if (strcmp(ename, "AnisotropySamples") == 0)
				{
					anisotropySamples = toFloat(configElement->GetText());
				}
				else if (strcmp(ename, "PbrSupport") == 0)
				{
					pbrSupport = toBool(configElement->GetText());
				}
                else if (strcmp(ename, "OpenGlDebugContext") == 0)
                {
                    openGlDebugContext = toBool(configElement->GetText());
                }
                else if (strcmp(ename, "LoggerLevel") == 0)
                {
                    loggerLevel = std::string(configElement->GetText());
                }
                else if (strcmp(ename, "LoggerConsoleOutput") == 0)
                {
                    loggerConsoleOutput = toBool(configElement->GetText());
                }
                else if (strcmp(ename, "LoggerFileOutput") == 0)
                {
                    const char* text = configElement->GetText();
                    if (text != nullptr)
                    {
                        loggerFileOutput = std::string(text);
                    }
                }
                else if (strcmp(ename, "Language") == 0)
                {
                    language = std::string(configElement->GetText());
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
		else if (strcmp(ename, "developmentMode") == 0)
		{
			developmentMode = toBool(child->GetText());
		}
        else if (strcmp(ename, "firstScene") == 0)
        {
            firstScene = std::string(child->GetText());
        }
        else if (strcmp(ename, "useLoadingScreen") == 0)
        {
            useLoadingScreen = toBool(child->GetText());
        }
	}
}


GameConfig* GameConfig::instance = NULL;
