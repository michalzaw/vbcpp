#include "AnimatedModelLoader.h"

#include "MaterialSaver.h"

#include "../Utils/AssimpGlmConverter.h"


AnimatedModelLoader::AnimatedModelLoader()
	: StaticModelLoader(false),
    _boneCounter(0)
{

}


AnimatedModelLoader::~AnimatedModelLoader()
{

}


void AnimatedModelLoader::setBoneDataInVertex(AnimatedVertex& vertex, int boneId, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.boneIds[i] < 0)
        {
            vertex.boneIds[i] = boneId;
            vertex.weights[i] = weight;

            break;
        }
    }
}


void AnimatedModelLoader::extractBoneWeights(AnimatedVertex* vertices, unsigned int verticesCount, const aiMesh* assimpMesh)
{
    for (int i = 0; i < assimpMesh->mNumBones; ++i)
    {
        int boneId = -1;
        std::string boneName = assimpMesh->mBones[i]->mName.C_Str();

        if (_boneInfos.find(boneName) == _boneInfos.end())
        {
            boneId = _boneCounter++;

            BoneInfo* boneInfo = new BoneInfo;
            boneInfo->id = boneId;
            boneInfo->offset = AssimpGlmConverter::toMat4(assimpMesh->mBones[i]->mOffsetMatrix);

            _boneInfos[boneName] = boneInfo;
        }
        else
        {
            boneId = _boneInfos[boneName]->id;
        }

        auto weights = assimpMesh->mBones[i]->mWeights;
        for (int j = 0; j < assimpMesh->mBones[i]->mNumWeights; ++j)
        {
            int vertexId = weights[j].mVertexId;
            float weight = weights[j].mWeight;

            if (vertexId < verticesCount)
            {
                setBoneDataInVertex(vertices[vertexId], boneId, weight);
            }
            else
            {
                LOG_ERROR("Invalid vertexId: " + Strings::toString(vertexId) + ", verticesCount: " + Strings::toString(verticesCount));
            }
        }
    }
}


void AnimatedModelLoader::loadNode(const aiNode* assimpNode, AnimationNodeData& outNode)
{
    outNode.name = std::string(assimpNode->mName.C_Str());
    outNode.transformation = AssimpGlmConverter::toMat4(assimpNode->mTransformation);

    for (unsigned int i = 0; i < assimpNode->mNumChildren; ++i)
    {
        AnimationNodeData child;
        loadNode(assimpNode->mChildren[i], child);
        outNode.children.push_back(child);
    }
}


void AnimatedModelLoader::getTransformFromAssimpNode(aiNode* assimpNode, Transform& transform)
{
    // w animacji nie uwzgledniamy transformacji z nodow modelu - transformacja bedzie pochodzic z kosci
    transform.setPosition(0.0f, 0.0f, 0.0f);
    transform.setRotation(0.0f, 0.0f, 0.0f);
    transform.setScale(1.0f, 1.0f, 1.0f);
}


bool AnimatedModelLoader::loadMeshFromNode(const aiMesh* assimpMesh, StaticModelMesh& mesh, bool isLoadingSingleNode, const glm::mat4& globalNodeTransform)
{
    std::vector<MeshMender::Vertex> meshVertices;
    std::vector<unsigned int> meshIndices;

    unsigned int materialIndexInScene = assimpMesh->mMaterialIndex;

    // ----------------------------------------
    aiString materialName;
    _assimpScene->mMaterials[materialIndexInScene]->Get(AI_MATKEY_NAME, materialName);
    if (strcmp(materialName.C_Str(), COLLISION_MATERIAL_NAME) == 0)
    {
        for (int j = 0; j < assimpMesh->mNumVertices; ++j)
        {
            const aiVector3D& position = assimpMesh->mVertices[j];

            glm::vec4 v;
            if (!isLoadingSingleNode)
                v = globalNodeTransform * AssimpGlmConverter::toVec4(position);
            else
                v = AssimpGlmConverter::toVec4(position);

            _collisionMesh.push_back(glm::vec3(v.x, v.y, v.z));
        }

        return false;
    }
    // ----------------------------------------

    // meshes vertices
    for (int j = 0; j < assimpMesh->mNumVertices; ++j)
    {
        MeshMender::Vertex vertex;
        getMeshMenderVertexFromAssimpMesh(assimpMesh, j, vertex);

        meshVertices.push_back(vertex);
    }

    // meshes indices
    for (int j = 0; j < assimpMesh->mNumFaces; ++j)
    {
        const aiFace& face = assimpMesh->mFaces[j];
        assert(face.mNumIndices == 3);
        meshIndices.push_back(face.mIndices[0]);
        meshIndices.push_back(face.mIndices[1]);
        meshIndices.push_back(face.mIndices[2]);
    }

    // Mesh mender pass
    //runMeshMender(meshVertices, meshIndices);

    // vertices and indices array
    AnimatedVertex* vertices = new AnimatedVertex[meshVertices.size()];
    for (unsigned int j = 0; j < meshVertices.size(); ++j)
    {
        vertices[j] = meshVertices[j];
    }

    unsigned int* indices = new unsigned int[meshIndices.size()];
    for (unsigned int j = 0; j < meshIndices.size(); ++j)
    {
        indices[j] = meshIndices[j];
    }

    extractBoneWeights(vertices, meshVertices.size(), assimpMesh);

    // create mesh
    mesh.setMeshData(vertices, meshVertices.size(), indices, meshIndices.size(), materialIndexInScene, _materials[materialIndexInScene]->shader);

    return true;
}


RAnimatedModel* AnimatedModelLoader::loadAnimatedModelWithHierarchy(const std::string& fileName, const std::string& texturesPath)
{
    LOG_INFO("Load animated model: " + fileName);

    _texturesPath = texturesPath;

    if (_assimpScene == nullptr)
    {
        _assimpScene = _assimpImporter.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
    }
    if (_assimpScene == nullptr)
    {
        LOG_ERROR("Error parsing file: " + fileName + ": " + _assimpImporter.GetErrorString());
        return nullptr;
    }

    std::string materialXmlFileName = MaterialLoader::createMaterialFileName(fileName);
    if (!FilesHelper::isFileExists(materialXmlFileName))
    {
        MaterialSaver::saveMaterialsFromAssimpModel(materialXmlFileName, _assimpScene);
    }

    _materialLoader->openFile(materialXmlFileName.c_str());
    loadAllMaterials();


    StaticModelNode* rootNode = createModelNode(_assimpScene->mRootNode);


    glm::vec3* colMesh = new glm::vec3[_collisionMesh.size()];
    for (int i = 0; i < _collisionMesh.size(); ++i)
    {
        colMesh[i] = _collisionMesh[i];
    }

    RAnimatedModel* model = new RAnimatedModel(fileName, rootNode, _materials, _boneInfos, GL_TRIANGLES, colMesh, _collisionMesh.size());

    loadNode(_assimpScene->mRootNode, model->_bonesRootNode);

    _materialLoader->closeFile();
    _collisionMesh.clear();
    _nodesToSkipNames.clear();
    _nodeToLoadName.clear();

    _boneInfos.clear();
    _boneCounter = 0;

    return model;
}
