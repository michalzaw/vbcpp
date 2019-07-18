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


class StaticModelLoader
{
    private:
        static constexpr const char* COLLISION_MATERIAL_NAME = "Collision";

        static const unsigned int IMPORT_FLAGS_FOR_LOADING_WITH_HIERARCHY = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                                                                            aiProcess_CalcTangentSpace | aiProcess_FlipUVs;

        static const unsigned int IMPORT_FLAGS_FOR_LOADING_WITHOUT_HIERARCHY = IMPORT_FLAGS_FOR_LOADING_WITH_HIERARCHY | aiProcess_PreTransformVertices;

        Assimp::Importer _assimpImporter;
        const aiScene* _assimpScene;

        std::vector<std::string> _nodesToSkipNames;
        std::string _nodeToLoadName;

        unsigned int _materialsCount;
        Material* _materials;

        std::vector<glm::vec3> _collisionMesh;

        MaterialLoader* _materialLoader;
        std::string _texturesPath;

        Transform _lastNodeTransform;


        void saveMaterialsDataToXml(std::string fileName);
        void loadAllMaterials();

        void getTransformFromAssimpNode(aiNode* assimpNode, Transform& transform);
        bool isNodeContainsCollisionMesh(aiNode* assimpNode);

        void getMeshMenderVertexFromAssimpMesh(const aiMesh* assimpMesh, unsigned int vertexIndex, MeshMender::Vertex& outVertex);
        void runMeshMender(std::vector<MeshMender::Vertex>& vertices, std::vector<unsigned int>& indices);

        StaticModelNode* createModelNode(aiNode* node, glm::mat4 parentTransform = glm::mat4(1.0f), StaticModelNode* parent = NULL);

    public:
        StaticModelLoader();
		~StaticModelLoader()
		{
			delete _materialLoader;
		}

        RStaticModel* loadModelWithHierarchy(std::string fileName, std::string texturesPath);
        RStaticModel* loadModelWithHierarchy(std::string fileName, std::string texturesPath, std::vector<std::string> nodesToSkipNames);
        RStaticModel* loadModelWithHierarchyOnlyNode(std::string fileName, std::string texturesPath, std::string nodeToLoadName, Transform& loadedNodeTransformInModel);

        RStaticModel* loadModel(std::string fileName, std::string texturesPath);

};


#endif // STATICMODELLOADER_H_INCLUDED
