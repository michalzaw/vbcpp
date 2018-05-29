#include "Grass.h"


Grass::Grass(RModel* model, RTexture2D* terrainHeightmap, RTexture2D* grassDensityTexture)
    : RenderObject(model),
    _terrainHeightmap(terrainHeightmap), _grassDensityTexture(grassDensityTexture),
    _grassColor(1.0f, 1.0f, 1.0f, 1.0f)
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
