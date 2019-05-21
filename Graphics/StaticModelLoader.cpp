#include "StaticModelLoader.h"
#include <iostream>
#include <algorithm>


void StaticModelLoader::saveMaterialsDataToXml(std::string fileName)
{
    if (_assimpScene == NULL)
        return;

    XMLDocument doc;

    XMLDeclaration* dec = doc.NewDeclaration();
    doc.InsertFirstChild(dec);

    XMLNode* root = doc.NewElement(XML_MATERIAL_ROOT);

    for (int i = 0; i < _assimpScene->mNumMaterials; ++i)
    {
        const aiMaterial* material = _assimpScene->mMaterials[i];

        aiColor4D ambientColor;
        aiColor4D diffuseColor;
        aiColor4D specularColor;
        float opacity;
        float shinines;
        aiString diffuseTexturePath;
        aiString normalmapPath;
        aiString alphaTexturePath;

        material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        material->Get(AI_MATKEY_OPACITY, opacity);
        material->Get(AI_MATKEY_SHININESS, shinines);

        bool isDiffuseTextureExist = material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
        bool isNormalmapExist = material->GetTexture(aiTextureType_NORMALS, 0, &normalmapPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;
        bool isAlphaTextureExist = material->GetTexture(aiTextureType_OPACITY, 0, &alphaTexturePath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS;

        std::string type;
        if (isDiffuseTextureExist && isNormalmapExist)
            type = "normalmapping";
        else if (isDiffuseTextureExist && !isNormalmapExist)
            type = "solid";
        else
            type = "no_texture";

        XMLElement* matElement = doc.NewElement(XML_MATERIAL_ELEMENT);

        matElement->SetAttribute("type", type.c_str());

        aiString materialName;
        material->Get(AI_MATKEY_NAME, materialName);
        matElement->SetAttribute("name", materialName.C_Str());

        std::string ambientStr = toString(ambientColor.r) + "," + toString(ambientColor.g) + "," + toString(ambientColor.b) + "," + toString(ambientColor.a);
        matElement->SetAttribute("ambient", ambientStr.c_str());

        std::string diffuseStr = toString(diffuseColor.r) + "," + toString(diffuseColor.g) + "," + toString(diffuseColor.b) + "," + toString(diffuseColor.a);
        matElement->SetAttribute("diffuse", diffuseStr.c_str());

        std::string specularStr = toString(specularColor.r) + "," + toString(specularColor.g) + "," + toString(specularColor.b) + "," + toString(specularColor.a);
        matElement->SetAttribute("specular", specularStr.c_str());

        matElement->SetAttribute("diffuse_texture", diffuseTexturePath.C_Str());

        matElement->SetAttribute("normalmap_texture", normalmapPath.C_Str());

        matElement->SetAttribute("alpha_texture", alphaTexturePath.C_Str());

        std::string shininessStr = toString(shinines);
        matElement->SetAttribute("shininess", shininessStr.c_str());

        std::string transparencyStr = toString(opacity);
        std::cout << transparencyStr.c_str() << " " << opacity << std::endl;
        matElement->SetAttribute("transparency", transparencyStr.c_str());

        std::string offsetStr = "0,0";//toString(material->texture1_map.offset[0]) + "," + toString(material->texture1_map.offset[1]);
        matElement->SetAttribute("offset", offsetStr.c_str());

        std::string scaleStr = "1,1";//toString(material->texture1_map.scale[0]) + "," + toString(material->texture1_map.scale[1]);
        matElement->SetAttribute("scale", scaleStr.c_str());

        root->InsertEndChild(matElement);
    }

    doc.InsertEndChild(root);

    doc.SaveFile(fileName.c_str());
}


glm::mat4 calculateGlmMatrix(aiMatrix4x4& assimpMatrix)
{
    glm::mat4 matrix;

    matrix[0][0] = (GLfloat)assimpMatrix.a1; matrix[0][1] = (GLfloat)assimpMatrix.b1;  matrix[0][2] = (GLfloat)assimpMatrix.c1; matrix[0][3] = (GLfloat)assimpMatrix.d1;
    matrix[1][0] = (GLfloat)assimpMatrix.a2; matrix[1][1] = (GLfloat)assimpMatrix.b2;  matrix[1][2] = (GLfloat)assimpMatrix.c2; matrix[1][3] = (GLfloat)assimpMatrix.d2;
    matrix[2][0] = (GLfloat)assimpMatrix.a3; matrix[2][1] = (GLfloat)assimpMatrix.b3;  matrix[2][2] = (GLfloat)assimpMatrix.c3; matrix[2][3] = (GLfloat)assimpMatrix.d3;
    matrix[3][0] = (GLfloat)assimpMatrix.a4; matrix[3][1] = (GLfloat)assimpMatrix.b4;  matrix[3][2] = (GLfloat)assimpMatrix.c4; matrix[3][3] = (GLfloat)assimpMatrix.d4;

    return matrix;
}


void print(aiNode* assimpNode, int spaces)
{
    for (int i = 0; i < spaces; ++i)
        std::cout << " ";
    std::cout << assimpNode->mName.C_Str() << std::endl;

    aiMatrix4x4& assimpMatrix = assimpNode->mTransformation;

    std::cout << (GLfloat)assimpMatrix.a1 << " " << (GLfloat)assimpMatrix.b1 << " " << (GLfloat)assimpMatrix.c1 << " " << (GLfloat)assimpMatrix.d1 << std::endl;
    std::cout << (GLfloat)assimpMatrix.a2 << " " << (GLfloat)assimpMatrix.b2 << " " << (GLfloat)assimpMatrix.c2 << " " << (GLfloat)assimpMatrix.d2 << std::endl;
    std::cout << (GLfloat)assimpMatrix.a3 << " " << (GLfloat)assimpMatrix.b3 << " " << (GLfloat)assimpMatrix.c3 << " " << (GLfloat)assimpMatrix.d3 << std::endl;
    std::cout << (GLfloat)assimpMatrix.a4 << " " << (GLfloat)assimpMatrix.b4 << " " << (GLfloat)assimpMatrix.c4 << " " << (GLfloat)assimpMatrix.d4 << std::endl;

    system("pause");

    for (int i = 0; i < assimpNode->mNumChildren; ++i)
        print(assimpNode->mChildren[i], spaces + 2);
}


bool StaticModelLoader::isNodeContainsCollisionMesh(aiNode* assimpNode)
{
    unsigned int meshesCount = assimpNode->mNumMeshes;
    for (int i = 0; i < meshesCount; ++i)
    {
        const aiMesh* assimpMesh = _assimpScene->mMeshes[assimpNode->mMeshes[i]];
        unsigned int materialIndexInScene = assimpMesh->mMaterialIndex;

        aiString materialName;
        _assimpScene->mMaterials[materialIndexInScene]->Get(AI_MATKEY_NAME, materialName);
        if (strcmp(materialName.C_Str(), "Collision") == 0)
        {
            return true;
        }
    }

    return false;
}


StaticModelNode* StaticModelLoader::createModelNode(aiNode* assimpNode, glm::mat4 parentTransform, StaticModelNode* parent)
{
    glm::mat4 nodeTransform = calculateGlmMatrix(assimpNode->mTransformation);
    glm::mat4 globalNodeTransform = parentTransform * nodeTransform;

    std::string nodeName = std::string(assimpNode->mName.C_Str());
    if (std::find(_nodesToSkipNames.begin(), _nodesToSkipNames.end(), nodeName) != _nodesToSkipNames.end())
    {
        return NULL;
    }

    if (!_nodesToLoadNames.empty() && parent != NULL && std::find(_nodesToLoadNames.begin(), _nodesToLoadNames.end(), nodeName) == _nodesToLoadNames.end())
    {
        return NULL;
    }

    bool isCollisionMeshExist = isNodeContainsCollisionMesh(assimpNode);

    unsigned int meshesCount = isCollisionMeshExist ? assimpNode->mNumMeshes - 1 : assimpNode->mNumMeshes;
    std::vector<MeshMender::Vertex>* meshesVertices = new std::vector<MeshMender::Vertex>[meshesCount];
    std::vector<unsigned int>* meshesIndices = new std::vector<unsigned int>[meshesCount];
    unsigned int* meshesMaterialIndex = new unsigned int[meshesCount];

    StaticModelMesh* meshes = new StaticModelMesh[meshesCount];

    int meshesIndex = 0;

    for (int i = 0; i < assimpNode->mNumMeshes; ++i)
    {
        const aiMesh* assimpMesh = _assimpScene->mMeshes[assimpNode->mMeshes[i]];
        unsigned int materialIndexInScene = assimpMesh->mMaterialIndex;

        // ----------------------------------------
        aiString materialName;
        _assimpScene->mMaterials[materialIndexInScene]->Get(AI_MATKEY_NAME, materialName);
        if (strcmp(materialName.C_Str(), "Collision") == 0)
        {
            for (int j = 0; j < assimpMesh->mNumVertices; ++j)
            {
                const aiVector3D* position = &(assimpMesh->mVertices[j]);
                glm::vec4 v = globalNodeTransform * glm::vec4(position->x, position->y, position->z, 1.0f);
                _collisionMesh.push_back(glm::vec3(v.x, v.y, v.z));
            }

            isCollisionMeshExist = true;

            continue;
        }
        // ----------------------------------------

        // meshes vertices
        const aiVector3D zeroVector(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < assimpMesh->mNumVertices; ++j)
        {
            const aiVector3D* position = &(assimpMesh->mVertices[j]);
            const aiVector3D* normal = &(assimpMesh->mNormals[j]);
            const aiVector3D* texCoord = assimpMesh->HasTextureCoords(0) ? &(assimpMesh->mTextureCoords[0][j]) : &zeroVector;
            const aiVector3D* tangent = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mTangents[j]) : &zeroVector;
            const aiVector3D* bitangent = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mBitangents[j]) : &zeroVector;

            MeshMender::Vertex vertex;
            vertex.pos = Vector3(position->x, position->y, position->z);
            vertex.normal = Vector3(normal->x, normal->y, normal->z);
            vertex.s = texCoord->x;
            vertex.t = texCoord->y;
            vertex.tangent = Vector3(tangent->x, tangent->y, tangent->z);
            vertex.binormal = Vector3(bitangent->x, bitangent->y, bitangent->z);

            meshesVertices[meshesIndex].push_back(vertex);
        }

        // meshes indices
        for (int j = 0; j < assimpMesh->mNumFaces; ++j)
        {
            const aiFace& face = assimpMesh->mFaces[j];
            assert(face.mNumIndices == 3);
            meshesIndices[meshesIndex].push_back(face.mIndices[0]);
            meshesIndices[meshesIndex].push_back(face.mIndices[1]);
            meshesIndices[meshesIndex].push_back(face.mIndices[2]);
        }

        // meshes material index
        meshesMaterialIndex[meshesIndex] = materialIndexInScene;

        // Mesh mender pass
        std::vector<unsigned int> temp;
        MeshMender mender;
        mender.Mend(meshesVertices[meshesIndex], meshesIndices[meshesIndex], temp, 0.7f, 0.7f, 0.7f, 1.0f,
                    MeshMender::CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);

        // vertices and indices array
        Vertex* vertices = new Vertex[meshesVertices[meshesIndex].size()];
        for (unsigned int j = 0; j < meshesVertices[meshesIndex].size(); ++j)
        {
            vertices[j].position = meshesVertices[meshesIndex][j].pos;
            vertices[j].texCoord = glm::vec2(meshesVertices[meshesIndex][j].s, meshesVertices[meshesIndex][j].t);
            vertices[j].normal = meshesVertices[meshesIndex][j].normal;
            vertices[j].tangent = meshesVertices[meshesIndex][j].tangent;
            vertices[j].bitangent = meshesVertices[meshesIndex][j].binormal;
        }

        unsigned int* indices = new unsigned int[meshesIndices[meshesIndex].size()];
        for (unsigned int j = 0; j < meshesIndices[meshesIndex].size(); ++j)
        {
            indices[j] = meshesIndices[meshesIndex][j];
        }

        // create mesh
        meshes[meshesIndex].setMeshData(vertices, meshesVertices[meshesIndex].size(), indices, meshesIndices[meshesIndex].size(), materialIndexInScene, _materials[materialIndexInScene].shader);

        meshesIndex++;
    }

    delete[] meshesVertices;
    delete[] meshesIndices;

    // create model node
    StaticModelNode* modelNode = new StaticModelNode;
    modelNode->name = nodeName;
    modelNode->transformMatrix = nodeTransform;
    modelNode->meshes = meshes;
    modelNode->meshesCount = meshesCount;
    modelNode->parent = parent;

    // node children
    for (unsigned int i = 0; i < assimpNode->mNumChildren; ++i)
    {
        StaticModelNode* node = createModelNode(assimpNode->mChildren[i], globalNodeTransform, modelNode);
        if (node != NULL)
        {
            modelNode->children.push_back(node);
        }
    }

    return modelNode;
}


RStaticModel* StaticModelLoader::loadModel(std::string fileName, std::string texturePath)
{
    _texturesPath = texturePath;

    unsigned int importFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace |
                               aiProcess_FlipUVs;
    _assimpScene = _assimpImporter.ReadFile(fileName.c_str(), importFlags);

    if (_assimpScene == NULL)
    {
        std::cout << "Error parsing file: " << fileName << ": " << _assimpImporter.GetErrorString() << std::endl;
        return NULL;
    }

    std::string xmlFileName = MaterialLoader::createMaterialFileName(fileName);

    if (!FilesHelper::isFileExists(xmlFileName))
    {
        saveMaterialsDataToXml(xmlFileName);
    }

	_materialLoader->openFile(xmlFileName.c_str());

	unsigned int materialsCount = _assimpScene->mNumMaterials;
	_materials = new Material[materialsCount];
	for (int i = 0; i < materialsCount; ++i)
    {
        aiString materialName;
        _assimpScene->mMaterials[i]->Get(AI_MATKEY_NAME, materialName);
        _materials[i] = _materialLoader->loadMaterial(materialName.C_Str(), _texturesPath);
    }

	_collisionMesh.clear();

    StaticModelNode* rootNode = createModelNode(_assimpScene->mRootNode);

    glm::vec3* colMesh = new glm::vec3[_collisionMesh.size()];
    for (int i = 0; i < _collisionMesh.size(); ++i)
    {
        colMesh[i] = _collisionMesh[i];
    }
    RStaticModel* model = new RStaticModel(fileName, rootNode, _materials, materialsCount, GL_TRIANGLES, colMesh, _collisionMesh.size());

    _materialLoader->closeFile();
    _nodesToLoadNames.clear();
    _nodesToSkipNames.clear();

    return model;
}


RStaticModel* StaticModelLoader::loadModel(std::string fileName, std::string texturesPath, std::vector<std::string> nodesToSkipNames)
{
    _nodesToSkipNames = nodesToSkipNames;

    return loadModel(fileName, texturesPath);
}


RStaticModel* StaticModelLoader::loadModelNodes(std::string fileName, std::string texturesPath, std::vector<std::string> nodesToLoadNames)
{
    _nodesToLoadNames = nodesToLoadNames;

    return loadModel(fileName, texturesPath);
}


/*RStaticModel* StaticModelLoader::loadModel(std::string fileName, std::string texturesPath)
{
    unsigned int importFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace |
                               aiProcess_FlipUVs | aiProcess_PreTransformVertices;
    _assimpScene = _assimpImporter.ReadFile(fileName.c_str(), importFlags);

    if (_assimpScene == NULL)
    {
        std::cout << "Error parsing file: " << fileName << ": " << _assimpImporter.GetErrorString() << std::endl;
        return NULL;
    }

    std::string xmlFileName = MaterialLoader::createMaterialFileName(fileName);

    if (!FilesHelper::isFileExists(xmlFileName))
    {
        saveMaterialsDataToXml(xmlFileName);
    }


    unsigned int meshesCount = _assimpScene->mNumMaterials;
    std::vector<MeshMender::Vertex>* meshesVertices = new std::vector<MeshMender::Vertex>[meshesCount];
    std::vector<unsigned int>* meshesIndices = new std::vector<unsigned int>[meshesCount];
    std::vector<glm::vec3> collisionMesh;

    bool isCollisionMeshExist = false;
    for (int i = 0; i < _assimpScene->mNumMeshes; ++i)
    {
        const aiMesh* assimpMesh = _assimpScene->mMeshes[i];
        unsigned int meshIndex = assimpMesh->mMaterialIndex;

        // ----------------------------------------
        aiString materialName;
        _assimpScene->mMaterials[meshIndex]->Get(AI_MATKEY_NAME, materialName);
        if (strcmp(materialName.C_Str(), "Collision") == 0)
        {
            for (int j = 0; j < assimpMesh->mNumVertices; ++j)
            {
                const aiVector3D* position = &(assimpMesh->mVertices[j]);
                collisionMesh.push_back(glm::vec3(position->x, position->y, position->z));
            }

            isCollisionMeshExist = true;

            continue;
        }
        // ----------------------------------------

        unsigned int firstVertex = meshesVertices[meshIndex].size();

        const aiVector3D zeroVector(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < assimpMesh->mNumVertices; ++j)
        {
            const aiVector3D* position = &(assimpMesh->mVertices[j]);
            const aiVector3D* normal = &(assimpMesh->mNormals[j]);
            const aiVector3D* texCoord = assimpMesh->HasTextureCoords(0) ? &(assimpMesh->mTextureCoords[0][j]) : &zeroVector;
            const aiVector3D* tangent = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mTangents[j]) : &zeroVector;
            const aiVector3D* bitangent = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mBitangents[j]) : &zeroVector;

            MeshMender::Vertex vertex;
            vertex.pos = Vector3(position->x, position->y, position->z);
            vertex.normal = Vector3(normal->x, normal->y, normal->z);
            vertex.s = texCoord->x;
            vertex.t = texCoord->y;
            vertex.tangent = Vector3(tangent->x, tangent->y, tangent->z);
            vertex.binormal = Vector3(bitangent->x, bitangent->y, bitangent->z);

            meshesVertices[meshIndex].push_back(vertex);
        }

        for (int j = 0; j < assimpMesh->mNumFaces; ++j)
        {
            const aiFace& face = assimpMesh->mFaces[j];
            assert(face.mNumIndices == 3);
            meshesIndices[meshIndex].push_back(face.mIndices[0] + firstVertex);
            meshesIndices[meshIndex].push_back(face.mIndices[1] + firstVertex);
            meshesIndices[meshIndex].push_back(face.mIndices[2] + firstVertex);
        }
    }


    for (int i = 0; i < meshesCount; ++i)
    {
        std::vector<unsigned int> temp;
        MeshMender mender;
        mender.Mend(meshesVertices[i], meshesIndices[i], temp, 0.7f, 0.7f, 0.7f, 1.0f,
                    MeshMender::CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);
    }


    StaticModelMesh* meshes = new StaticModelMesh[!isCollisionMeshExist ? meshesCount : (meshesCount - 1)];
    Material* materials = new Material[!isCollisionMeshExist ? meshesCount : (meshesCount - 1)];

    MaterialLoader matLoader;
	matLoader.openFile(xmlFileName.c_str());

	unsigned int meshCounter = 0;
    for (int i = 0; i < meshesCount; ++i)
    {
        aiString materialName;
        _assimpScene->mMaterials[i]->Get(AI_MATKEY_NAME, materialName);
        // ----------------------------------------
        if (strcmp(materialName.C_Str(), "Collision") == 0)
        {
            continue;
        }
        // ----------------------------------------

        Vertex* vertices = new Vertex[meshesVertices[i].size()];
        for (unsigned int j = 0; j < meshesVertices[i].size(); ++j)
        {
            vertices[j].position = meshesVertices[i][j].pos;
            vertices[j].texCoord = glm::vec2(meshesVertices[i][j].s, meshesVertices[i][j].t);
            vertices[j].normal = meshesVertices[i][j].normal;
            vertices[j].tangent = meshesVertices[i][j].tangent;
            vertices[j].bitangent = meshesVertices[i][j].binormal;
        }

        unsigned int* indices = new unsigned int[meshesIndices[i].size()];
        for (unsigned int j = 0; j < meshesIndices[i].size(); ++j)
        {
            indices[j] = meshesIndices[i][j];
        }

        materials[meshCounter] = matLoader.loadMaterial(materialName.C_Str(), texturesPath);

        meshes[meshCounter].setMeshData(vertices, meshesVertices[i].size(), indices, meshesIndices[i].size(), meshCounter, materials[meshCounter].shader);

        ++meshCounter;
    }

    matLoader.closeFile();


    // ----------------------------------------
    glm::vec3* colMesh = new glm::vec3[collisionMesh.size()];
    for (int i = 0; i < collisionMesh.size(); ++i)
    {
        colMesh[i] = collisionMesh[i];
    }
    // ----------------------------------------


    delete[] meshesVertices;
    delete[] meshesIndices;


    RStaticModel* model = new RStaticModel(fileName, meshes, !isCollisionMeshExist ? meshesCount : (meshesCount - 1), materials, GL_TRIANGLES, colMesh, collisionMesh.size());

    return model;
}*/
