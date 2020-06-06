#include "Terrain.h"

#include "LoadTerrainModel.h"


Terrain::Terrain(std::string heightmapFileName, std::string dirPath, std::string materialName, float maxHeight, bool is16bitTexture)
	: _heightmapFileName(heightmapFileName), _materialName(materialName), _maxHeight(maxHeight)
{
	_type = CT_TERRAIN;

	std::string heightmapFullPath = dirPath + _heightmapFileName;
	std::string materialFullPath = dirPath + MaterialLoader::createMaterialFileName(_heightmapFileName);

	setModel(TerrainLoader::loadTerrainModel(heightmapFullPath.c_str(), materialFullPath, _materialName, dirPath, _maxHeight, is16bitTexture));
}


Terrain::~Terrain()
{
	if (_model != nullptr)
	{
		delete _model;
	}
}


std::string Terrain::getHeightmapFileName()
{
	return _heightmapFileName;
}


std::string Terrain::getMaterialName()
{
	return _materialName;
}


float Terrain::getMaxHeight()
{
	return _maxHeight;
}
