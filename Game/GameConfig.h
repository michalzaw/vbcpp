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
            isFullscreen(false), verticalSync(false), hdrQuality(16), msaaAntialiasing(false), msaaAntialiasingLevel(8), isBloomEnabled(true),
			isShadowmappingEnable(true), shadowmapSize(1024), staticShadowmapSize(1024),
			isGrassEnable(true), grassRenderingDistance(30.0f), isMirrorsEnabled(true), mirrorRenderingDistance(100.0f),
			textureCompression(false), anisotropicFiltering(false), anisotropySamples(4.0f),
			pbrSupport(false), developmentMode(false)
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
		bool verticalSync;
		int hdrQuality;
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
		bool textureCompression;
		bool anisotropicFiltering;
		float anisotropySamples;

		bool pbrSupport;

		bool developmentMode;

		std::string alternativeResourcesPath;

};


#endif // GAMECONFIG_H_INCLUDED
