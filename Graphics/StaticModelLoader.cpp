#include "StaticModelLoader.h"
#include <iostream>
#include <algorithm>


using namespace tinyxml2;


StaticModelLoader::StaticModelLoader(bool normalsSmoothing)
    : _normalsSmoothing(normalsSmoothing), _assimpScene(NULL)
{
    _materialLoader = new MaterialLoader;
}


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


void StaticModelLoader::loadAllMaterials()
{
    _materialsCount = _assimpScene->mNumMaterials;
	_materials = new Material[_materialsCount];
	for (int i = 0; i < _materialsCount; ++i)
    {
        aiString materialName;
        _assimpScene->mMaterials[i]->Get(AI_MATKEY_NAME, materialName);
        _materials[i] = _materialLoader->loadMaterial(materialName.C_Str(), _texturesPath);
    }
}


void StaticModelLoader::getTransformFromAssimpNode(aiNode* assimpNode, Transform& transform)
{
    aiVector3t<ai_real> position;
    aiVector3t<ai_real> rotation;
    aiVector3t<ai_real> scale;
    assimpNode->mTransformation.Decompose(scale, rotation, position);

    transform.setPosition(position.x, position.y, position.z);
    transform.setRotation(rotation.x, rotation.y, rotation.z);
    transform.setScale(scale.x, scale.y, scale.z);
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
        if (strcmp(materialName.C_Str(), COLLISION_MATERIAL_NAME) == 0)
        {
            return true;
        }
    }

    return false;
}


void StaticModelLoader::getMeshMenderVertexFromAssimpMesh(const aiMesh* assimpMesh, unsigned int vertexIndex, MeshMender::Vertex& outVertex)
{
    const aiVector3D zeroVector(0.0f, 0.0f, 0.0f);

    const aiVector3D* position = &(assimpMesh->mVertices[vertexIndex]);
    const aiVector3D* normal = &(assimpMesh->mNormals[vertexIndex]);
    const aiVector3D* texCoord = assimpMesh->HasTextureCoords(0) ? &(assimpMesh->mTextureCoords[0][vertexIndex]) : &zeroVector;
    const aiVector3D* tangent = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mTangents[vertexIndex]) : &zeroVector;
    const aiVector3D* bitangent = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mBitangents[vertexIndex]) : &zeroVector;

    outVertex.pos = Vector3(position->x, position->y, position->z);
    outVertex.normal = Vector3(normal->x, normal->y, normal->z);
    outVertex.s = texCoord->x;
    outVertex.t = texCoord->y;
    outVertex.tangent = Vector3(tangent->x, tangent->y, tangent->z);
    outVertex.binormal = Vector3(bitangent->x, bitangent->y, bitangent->z);
}


void StaticModelLoader::runMeshMender(std::vector<MeshMender::Vertex>& vertices, std::vector<unsigned int>& indices)
{
	if (_normalsSmoothing)
	{
		std::vector<unsigned int> temp;
		MeshMender mender;
		mender.Mend(vertices, indices, temp, 0.7f, 0.7f, 0.7f, 1.0f,
			MeshMender::CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);
	}
}


StaticModelNode* StaticModelLoader::createModelNode(aiNode* assimpNode, glm::mat4 parentTransform, StaticModelNode* parent)
{
    Transform nodeTransform;
    getTransformFromAssimpNode(assimpNode, nodeTransform);

    glm::mat4 globalNodeTransform = parentTransform * nodeTransform.getTransformMatrix();

    std::string nodeName = std::string(assimpNode->mName.C_Str());
    if (isVectorContains(_nodesToSkipNames, nodeName))
    {
        return NULL;
    }

    bool isLoadingSingleNode = false;
    if (!_nodeToLoadName.empty() && parent != NULL && _nodeToLoadName != nodeName)
    {
        return NULL;
    }
    else if (_nodeToLoadName == nodeName)
    {
        isLoadingSingleNode = true;
        _lastNodeTransform = nodeTransform;
    }

    bool isCollisionMeshExist = isNodeContainsCollisionMesh(assimpNode);

    unsigned int meshesCount = isCollisionMeshExist ? assimpNode->mNumMeshes - 1 : assimpNode->mNumMeshes;
    StaticModelMesh* meshes = new StaticModelMesh[meshesCount];
    int meshesIndex = 0;

    for (int i = 0; i < assimpNode->mNumMeshes; ++i)
    {
        std::vector<MeshMender::Vertex> meshVertices;
        std::vector<unsigned int> meshIndices;

        const aiMesh* assimpMesh = _assimpScene->mMeshes[assimpNode->mMeshes[i]];
        unsigned int materialIndexInScene = assimpMesh->mMaterialIndex;

        // ----------------------------------------
        aiString materialName;
        _assimpScene->mMaterials[materialIndexInScene]->Get(AI_MATKEY_NAME, materialName);
        if (strcmp(materialName.C_Str(), COLLISION_MATERIAL_NAME) == 0)
        {
            for (int j = 0; j < assimpMesh->mNumVertices; ++j)
            {
                const aiVector3D* position = &(assimpMesh->mVertices[j]);

                glm::vec4 v;
                if (!isLoadingSingleNode)
                    v = globalNodeTransform * glm::vec4(position->x, position->y, position->z, 1.0f);
                else
                    v = glm::vec4(position->x, position->y, position->z, 1.0f);

                _collisionMesh.push_back(glm::vec3(v.x, v.y, v.z));
            }

            continue;
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
        runMeshMender(meshVertices, meshIndices);

        // vertices and indices array
        Vertex* vertices = new Vertex[meshVertices.size()];
        for (unsigned int j = 0; j < meshVertices.size(); ++j)
        {
            vertices[j] = meshVertices[j];
        }

        unsigned int* indices = new unsigned int[meshIndices.size()];
        for (unsigned int j = 0; j < meshIndices.size(); ++j)
        {
            indices[j] = meshIndices[j];
        }

        // create mesh
        meshes[meshesIndex].setMeshData(vertices, meshVertices.size(), indices, meshIndices.size(), materialIndexInScene, _materials[materialIndexInScene].shader);

        meshesIndex++;
    }

    // create model node
    StaticModelNode* modelNode = new StaticModelNode;
    modelNode->name = nodeName;
    if (!isLoadingSingleNode)
        modelNode->transform = nodeTransform;
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


RStaticModel* StaticModelLoader::loadModelWithHierarchy(std::string fileName, std::string texturesPath)
{
    _texturesPath = texturesPath;

    if (_assimpScene == NULL)
    {
        _assimpScene = _assimpImporter.ReadFile(fileName.c_str(), IMPORT_FLAGS_FOR_LOADING_WITH_HIERARCHY);
    }
    if (_assimpScene == NULL)
    {
        std::cout << "Error parsing file: " << fileName << ": " << _assimpImporter.GetErrorString() << std::endl;
        return NULL;
    }

    std::string materialXmlFileName = MaterialLoader::createMaterialFileName(fileName);
    if (!FilesHelper::isFileExists(materialXmlFileName))
    {
        saveMaterialsDataToXml(materialXmlFileName);
    }

	_materialLoader->openFile(materialXmlFileName.c_str());
    loadAllMaterials();


    StaticModelNode* rootNode = createModelNode(_assimpScene->mRootNode);


    glm::vec3* colMesh = new glm::vec3[_collisionMesh.size()];
    for (int i = 0; i < _collisionMesh.size(); ++i)
    {
        colMesh[i] = _collisionMesh[i];
    }

    RStaticModel* model = new RStaticModel(fileName, rootNode, _materials, _materialsCount, GL_TRIANGLES, colMesh, _collisionMesh.size());

    _materialLoader->closeFile();
	_collisionMesh.clear();
    _nodesToSkipNames.clear();
    _nodeToLoadName.clear();

    return model;
}


RStaticModel* StaticModelLoader::loadModelWithHierarchy(std::string fileName, std::string texturesPath, std::vector<std::string> nodesToSkipNames)
{
    _nodesToSkipNames = nodesToSkipNames;

    return loadModelWithHierarchy(fileName, texturesPath);
}


RStaticModel* StaticModelLoader::loadModelWithHierarchyOnlyNode(std::string fileName, std::string texturesPath, std::string nodeToLoadName, Transform& loadedNodeTransformInModel)
{
    _nodeToLoadName = nodeToLoadName;

    RStaticModel* staticModel = loadModelWithHierarchy(fileName, texturesPath);
    loadedNodeTransformInModel = _lastNodeTransform;

    return staticModel;
}


RStaticModel* StaticModelLoader::loadModel(std::string fileName, std::string texturesPath)
{
    _texturesPath = texturesPath;

    _assimpScene = _assimpImporter.ReadFile(fileName.c_str(), IMPORT_FLAGS_FOR_LOADING_WITHOUT_HIERARCHY);
    if (_assimpScene == NULL)
    {
        std::cout << "Error parsing file: " << fileName << ": " << _assimpImporter.GetErrorString() << std::endl;
        return NULL;
    }

    std::string materialXmlFileName = MaterialLoader::createMaterialFileName(fileName);
    if (!FilesHelper::isFileExists(materialXmlFileName))
    {
        saveMaterialsDataToXml(materialXmlFileName);
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
        if (strcmp(materialName.C_Str(), COLLISION_MATERIAL_NAME) == 0)
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
            MeshMender::Vertex vertex;
            getMeshMenderVertexFromAssimpMesh(assimpMesh, j, vertex);

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
        runMeshMender(meshesVertices[i], meshesIndices[i]);
    }


    StaticModelMesh* meshes = new StaticModelMesh[!isCollisionMeshExist ? meshesCount : (meshesCount - 1)];
    Material* materials = new Material[!isCollisionMeshExist ? meshesCount : (meshesCount - 1)];

    MaterialLoader matLoader;
	matLoader.openFile(materialXmlFileName.c_str());

	unsigned int meshCounter = 0;
    for (int i = 0; i < meshesCount; ++i)
    {
        aiString materialName;
        _assimpScene->mMaterials[i]->Get(AI_MATKEY_NAME, materialName);
        // ----------------------------------------
        if (strcmp(materialName.C_Str(), COLLISION_MATERIAL_NAME) == 0)
        {
            continue;
        }
        // ----------------------------------------

        Vertex* vertices = new Vertex[meshesVertices[i].size()];
        for (unsigned int j = 0; j < meshesVertices[i].size(); ++j)
        {
            vertices[j] = meshesVertices[i][j];
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

    StaticModelNode* rootNode = new StaticModelNode;
    rootNode->name = "RootNode";
    rootNode->meshes = meshes;
    rootNode->meshesCount = !isCollisionMeshExist ? meshesCount : (meshesCount - 1);
    rootNode->parent = NULL;

    RStaticModel* model = new RStaticModel(fileName, rootNode, materials, rootNode->meshesCount, GL_TRIANGLES, colMesh, collisionMesh.size());

    return model;
}
