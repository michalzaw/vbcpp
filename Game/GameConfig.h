#ifndef GAMECONFIG_H_INCLUDED
#define GAMECONFIG_H_INCLUDED


#include <string>

#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;


class GameConfig
{
    private:
        static GameConfig* instance;

        GameConfig()
            : windowWidth(1024), windowHeight(768),
            isFullscreen(false), msaaAntialiasing(false), msaaAntialiasingLevel(8), isShadowmappingEnable(true), shadowmapSize(1024), staticShadowmapSize(1024), isBloomEnabled(true),
			isGrassEnable(true), grassRenderingDistance(30.0f), isMirrorsEnabled(true), mirrorRenderingDistance(100.0f),
			pbrSupport(false)
        {}
        GameConfig(const GameConfig&) {}

    public:
        static GameConfig& getInstance()
        {
            if (instance == NULL)
                instance = new GameConfig;

            return *instance;
        }

        void loadGameConfig(const char* filename);
		void loadDevelopmentConfig(const char* filename);

        int windowWidth;
        int windowHeight;
        std::string mapFile;
        std::string mapTexPath;
        std::string busModel;

        bool isFullscreen;
        bool msaaAntialiasing;
        int msaaAntialiasingLevel;
        bool isShadowmappingEnable;
        int shadowmapSize;
		int staticShadowmapSize;
        bool isBloomEnabled;
        bool isGrassEnable;
		float grassRenderingDistance;
		bool isMirrorsEnabled;
		float mirrorRenderingDistance;

		bool pbrSupport;

		std::string alternativeResourcesPath;

};


#endif // GAMECONFIG_H_INCLUDED
