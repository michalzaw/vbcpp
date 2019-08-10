#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED


#include "RenderObject.h"


class Terrain : public RenderObject
{
	private:
		std::string _heightmapFileName;
		std::string _materialName;
		float _maxHeight;

	public:
		Terrain(std::string heightmapFileName, std::string dirPath, std::string materialName, float maxHeight);
		virtual ~Terrain();

		std::string getHeightmapFileName();
		std::string getMaterialName();
		float getMaxHeight();

};


#endif // TERRAIN_H_INCLUDED
