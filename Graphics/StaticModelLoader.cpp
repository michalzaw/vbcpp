#include "StaticModelLoader.h"
#include <iostream>


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


RStaticModel* StaticModelLoader::loadModel(std::string fileName, std::string texturesPath)
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

        Material material = matLoader.loadMaterial(materialName.C_Str(), texturesPath);

        meshes[meshCounter].setMeshData(vertices, meshesVertices[i].size(), indices, meshesIndices[i].size(), material);

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


    RStaticModel* model = new RStaticModel(fileName, meshes, !isCollisionMeshExist ? meshesCount : (meshesCount - 1), GL_TRIANGLES, colMesh, collisionMesh.size());

    return model;
}
