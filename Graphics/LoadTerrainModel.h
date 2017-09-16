#ifndef LOADTERRAINMODEL_H_INCLUDED
#define LOADTERRAINMODEL_H_INCLUDED


#include <fstream>
#include <iostream>
#include <conio.h>

#include <glm/glm.hpp>

#include <SOIL/soil.h>

#include "Model.h"
#include "Material.h"
#include "NVMeshMender/NVMeshMender.h"
#include "LoadMaterial.h"


class TerrainLoader
{
    static constexpr char* TER_EXTENSION = ".ter";

    private:
        static bool isTerFileExists(std::string fileName);
        static std::string createTerFileName(std::string heightmapFilename);

        static void saveTerFile(const char* fileName, Model* model);
        static Model* loadTerFile(const char* fileName, std::string materialFileName, std::string texturePath);

        static Model* loadTerrainFromHeightmap(const char* heightmapFilename, std::string materialFileName, std::string materialName, std::string texturePath, float maxHeight);

    public:
        static Model* loadTerrainModel(const char* heightmapFilename, std::string materialFileName, std::string materialName, std::string texturePath, float maxHeight);

};


#endif // LOADTERRAINMODEL_H_INCLUDED
