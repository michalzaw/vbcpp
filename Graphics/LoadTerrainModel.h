#ifndef LOADTERRAINMODEL_H_INCLUDED
#define LOADTERRAINMODEL_H_INCLUDED


#include <fstream>
#include <iostream>
#include <conio.h>

#include <glm/glm.hpp>

//#include <SOIL/soil.h>

#include "Material.h"
#include "NVMeshMender/NVMeshMender.h"
#include "LoadMaterial.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/RStaticModel.h"


class TerrainLoader
{
    static constexpr char* TER_EXTENSION = ".ter";
    static constexpr char* HEIGHT_NORMAL_MAP_SUFFIX = "_gen.tga";

    private:
        static std::string createTerFileName(std::string heightmapFilename);

        static void saveTerFile(const char* fileName, RStaticModel* model);
        static RStaticModel* loadTerFile(const char* fileName, std::string materialFileName, std::string texturePath);

        static RStaticModel* loadTerrainFromHeightmap(const char* heightmapFilename, std::string materialFileName, std::string materialName, std::string texturePath, float maxHeight);

    public:
        static RStaticModel* loadTerrainModel(const char* heightmapFilename, std::string materialFileName, std::string materialName, std::string texturePath, float maxHeight);

        static std::string createTerrainHeightAndNormalMapFileName(std::string heightmapFilename);

};


#endif // LOADTERRAINMODEL_H_INCLUDED
