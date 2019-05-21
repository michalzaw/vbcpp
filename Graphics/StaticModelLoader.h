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

        std::vector<std::string> _nodesToSkipNames;
        std::vector<std::string> _nodesToLoadNames;

        Material* _materials;

        std::vector<glm::vec3> _collisionMesh;

        MaterialLoader* _materialLoader;
        std::string _texturesPath;

        void saveMaterialsDataToXml(std::string fileName);
        bool isNodeContainsCollisionMesh(aiNode* assimpNode);
        StaticModelNode* createModelNode(aiNode* node, glm::mat4 parentTransform = glm::mat4(1.0f), StaticModelNode* parent = NULL);

    public:
        StaticModelLoader()
        {
            _materialLoader = new MaterialLoader;
        }
        RStaticModel* loadModel(std::string fileName, std::string texturesPath);
        RStaticModel* loadModel(std::string fileName, std::string texturesPath, std::vector<std::string> nodesToSkipNames);
        RStaticModel* loadModelNodes(std::string fileName, std::string texturesPath, std::vector<std::string> nodesToLoadNames);
        //RStaticModel* loadModel2(std::string fileName, std::string texturesPath);

};


#endif // STATICMODELLOADER_H_INCLUDED
