#include "Grass.h"

#include "../Utils/ResourceManager.h"


Grass::Grass(RStaticModel* model, RTexture2D* terrainHeightmap, RTexture2D* grassDensityTexture)
    : RenderObject(model),
    _terrainHeightmap(terrainHeightmap), _grassDensityTexture(grassDensityTexture),
    //_grassColor(0.388f, 0.78f, 0.0f, 1.0f),
	//_grassColor(0.5f, 1.0f, 0.0f, 1.0f),
	_grassColor(1.0f, 1.0f, 1.0f, 1.0f),
	_renderingDistance(30.0f)
{
    _type = CT_GRASS;
}


Grass::~Grass()
{

}


void Grass::setGrassColor(glm::vec4 grassColor)
{
    _grassColor = grassColor;
}


void Grass::setRenderingDistance(float renderingDistance)
{
	_renderingDistance = renderingDistance;
}


RTexture2D* Grass::getTerrainHeightmap()
{
    return _terrainHeightmap;
}


RTexture2D* Grass::getGrassDensityTexture()
{
    return _grassDensityTexture;
}


glm::vec4 Grass::getGrassColor()
{
    return _grassColor;
}


float Grass::getRenderingDistance()
{
	return _renderingDistance;
}


std::vector<RTexture2D*>& Grass::getAdditionalRandomGrassTextures()
{
	return _additionalRandomGrassTextures;
}


std::vector<float>& Grass::getAdditionalRandomGrassTexturesScale()
{
	return _additionalRandomGrassTexturesScale;
}
