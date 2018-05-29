#ifndef GRASS_H_INCLUDED
#define GRASS_H_INCLUDED


#include "RenderObject.h"


class Grass : public RenderObject
{
    private:
        RTexture2D* _terrainHeightmap;
        RTexture2D* _grassDensityTexture;
        glm::vec4   _grassColor;

    public:
        Grass(RModel* model, RTexture2D* terrainHeightmap, RTexture2D* grassDensityTexture);
        ~Grass();

        void setGrassColor(glm::vec4 grassColor);

        RTexture2D* getTerrainHeightmap();
        RTexture2D* getGrassDensityTexture();
        glm::vec4   getGrassColor();

};


#endif // GRASS_H_INCLUDED
