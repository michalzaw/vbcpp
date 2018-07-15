#ifndef STATICMODELLOADER_H_INCLUDED
#define STATICMODELLOADER_H_INCLUDED


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "LoadMaterial.h"
#include "NVMeshMender/NVMeshMender.h"

#include "../Utils/RStaticModel.h"
#include "../Utils/FilesHelper.h"
#include "../Utils/Strings.h"
#include "../Utils/Helpers.hpp"

#include "../Utils/tinyxml2.h"
using namespace tinyxml2;


class StaticModelLoader
{
    private:
        Assimp::Importer _assimpImporter;
        const aiScene* _assimpScene;

        void saveMaterialsDataToXml(std::string fileName);

    public:
        RStaticModel* loadModel(std::string fileName, std::string texturesPath);

};


#endif // STATICMODELLOADER_H_INCLUDED
