#include "LoadTerrainModel.h"

#include <stb_image.h>
#include <stb_image_write.h>


std::string TerrainLoader::createTerFileName(std::string heightmapFilename)
{
    std::string terFileName = heightmapFilename;
    terFileName.erase(terFileName.size() - 4, 4);
    terFileName += TER_EXTENSION;

    return terFileName;
}


std::string TerrainLoader::createTerrainHeightAndNormalMapFileName(std::string heightmapFilename)
{
    std::string fileName = heightmapFilename;
    fileName.erase(fileName.size() - 4, 4);
    fileName += HEIGHT_NORMAL_MAP_SUFFIX;

    return fileName;
}


void TerrainLoader::saveTerFile(const char* fileName, RStaticModel* model)
{
    StaticModelNode* modelRootNode = model->getRootNode();
    if (modelRootNode == NULL || modelRootNode->meshesCount != 1)
    {
        std::cout << "Terrain model cannot save to: " << fileName << ", meshes count: " << modelRootNode->getMeshesCount() << std::endl;
        return;
    }

    std::ofstream file(fileName, std::ios::binary);

    // Header
    file.write("TER001", 6 * sizeof(char));

    std::string materialName = model->getMaterial(0)->name;
    unsigned int materialNameLenght = materialName.length();
    file.write((char*)&materialNameLenght, sizeof(materialNameLenght));
    file.write(materialName.c_str(), materialNameLenght * sizeof(char));

    unsigned int verticesSize = modelRootNode->getMesh(0)->verticesCount;
    file.write((char*)&verticesSize, sizeof(verticesSize));

    unsigned int indicesSize = modelRootNode->getMesh(0)->indicesCount;
    file.write((char*)&indicesSize, sizeof(indicesSize));

    unsigned int collisionMeshSize = model-> getCollisionMeshSize();
    file.write((char*)&collisionMeshSize, sizeof(collisionMeshSize));

    // Vertices array
    file.write((char*)modelRootNode->getMesh(0)->vertices, sizeof(Vertex) * verticesSize);

    // Indices array
    file.write((char*)modelRootNode->getMesh(0)->indices, sizeof(unsigned int) * indicesSize);

    // Collision mesh vertices array
    file.write((char*)model->getCollisionMesh(), sizeof(glm::vec3) * collisionMeshSize);

    file.close();
}


RStaticModel* TerrainLoader::loadTerFile(const char* fileName, std::string materialFileName, std::string texturePath)
{
    std::ifstream file(fileName, std::ios::binary);

    // Header
    char fileId[7];
    file.read(fileId, 6);
    fileId[6] = 0;
    if (strcmp(fileId, "TER001") != 0)
    {
        file.close();

        return NULL;
    }

    unsigned int materialNameLength;
    file.read((char*)&materialNameLength, sizeof(unsigned int));

    char* materialName = new char[materialNameLength + 1];
    materialName[materialNameLength] = 0;
    file.read(materialName, materialNameLength * sizeof(char));

    unsigned int verticesSize;
    file.read((char*)&verticesSize, sizeof(verticesSize));

    unsigned int indicesSize;
    file.read((char*)&indicesSize, sizeof(indicesSize));

    unsigned int collisionMeshSize;
    file.read((char*)&collisionMeshSize, sizeof(collisionMeshSize));

    // Vertices array
    Vertex* vertices = new Vertex[verticesSize];
    file.read((char*)vertices, sizeof(Vertex) * verticesSize);

    // Indices array
    unsigned int* indices = new unsigned int[indicesSize];
    file.read((char*)indices, sizeof(unsigned int) * indicesSize);

    // Collision mesh vertices array
    glm::vec3* collisionMesh = new glm::vec3[collisionMeshSize];
    file.read((char*)collisionMesh, sizeof(glm::vec3) * collisionMeshSize);

    file.close();


    MaterialLoader matLoader;
    matLoader.openFile(materialFileName.c_str());

    Material* materials = new Material[1];
    materials[0] = matLoader.loadMaterial(materialName, texturePath);

    StaticModelMesh* meshes = new StaticModelMesh[1];
    meshes[0].setMeshData(vertices, verticesSize, indices, indicesSize, 0, materials[0].shader);

    StaticModelNode* modelNode = new StaticModelNode;
    modelNode->name = "terrain";
    modelNode->meshes = meshes;
    modelNode->meshesCount = 1;
    modelNode->parent = NULL;

    RStaticModel* model = new RStaticModel("", modelNode, materials, 1, GL_TRIANGLES, collisionMesh, collisionMeshSize);

    return model;
}


// Mesh mender
//Model* loadTerrainModel(const char* heightmapFilename, Material& material, float maxHeight)
RStaticModel* TerrainLoader::loadTerrainFromHeightmap(const char* heightmapFilename, std::string materialFileName, std::string materialName, std::string texturePath, float maxHeight, bool is16bit)
{
    MaterialLoader matLoader;
    matLoader.openFile(materialFileName.c_str());
    Material material = matLoader.loadMaterial(materialName, texturePath);


    const float cellSize = 1.0f;

    int width, height, chanels;
	unsigned short* heightmapData16 = NULL;
	unsigned char* heightmapData = NULL;
	if (is16bit)
	{
		heightmapData16 = stbi_load_16(heightmapFilename, &width, &height, &chanels, STBI_default);
	}
	else
	{
		heightmapData = stbi_load(heightmapFilename, &width, &height, &chanels, STBI_default);
	}

    glm::vec3 startPosition(static_cast<float>(width - 1) / -2.0f, 0.0f, static_cast<float>(height - 1) / 2.0f);
    glm::vec3 vertexPosition = startPosition;


    int terrainVerticesSize = width * height;
    Vertex* terrainVertices = new Vertex[terrainVerticesSize];

    int min = maxHeight;
    int max = 0;
    for (int z = 0; z < height; ++z)
    {
        vertexPosition.x = startPosition.x;

        for (int x = 0; x < width; ++x)
        {
			int color;
			if (is16bit)
			{
				color = static_cast<int>(heightmapData16[((height - z - 1) * width + x) * chanels]);
				vertexPosition.y = static_cast<float>(color) / 65025.0f * maxHeight;
			}
			else
			{
				color = static_cast<int>(heightmapData[((height - z - 1) * width + x) * chanels]);
				vertexPosition.y = static_cast<float>(color) / 255.0f * maxHeight;
			}


            if (vertexPosition.y < min)
                min = vertexPosition.y;
            if (vertexPosition.y > max)
                max = vertexPosition.y;


            terrainVertices[z * width + x].position = vertexPosition;
            terrainVertices[z * width + x].texCoord = glm::vec2(static_cast<float>(x) / width * material.scale.x + material.offset.x,
                                                                static_cast<float>(z) / height * material.scale.y + material.offset.y);
            terrainVertices[z * width + x].normal = glm::vec3(0.0f, 0.0f, 0.0f);
            terrainVertices[z * width + x].tangent = glm::vec3(0.0f, 0.0f, 0.0f);
            terrainVertices[z * width + x].bitangent = glm::vec3(0.0f, 0.0f, 0.0f);


            vertexPosition.x += cellSize;
        }

        vertexPosition.z -= cellSize;
    }

    int indicesSize = (width - 1) * (height - 1) * 6;
    unsigned int* indices = new unsigned int[indicesSize];

    glm::vec3* collisionMesh = new glm::vec3[indicesSize];

    int i = 0;
    for (int z = 0; z < height - 1; ++z)
    {
        for (int x = 0; x < width - 1; ++x)
        {
            // First triangle
            unsigned int i1 = z * width + x;
            unsigned int i2 = z * width + x + 1;
            unsigned int i3 = z * width + x + width;

            indices[i++] = i1;
            indices[i++] = i2;
            indices[i++] = i3;

            collisionMesh[i - 3] = terrainVertices[i1].position;
            collisionMesh[i - 2] = terrainVertices[i2].position;
            collisionMesh[i - 1] = terrainVertices[i3].position;


            glm::vec3 normal = glm::cross(terrainVertices[i3].position - terrainVertices[i2].position,
										  terrainVertices[i1].position - terrainVertices[i2].position);
			normal = glm::normalize(normal);

			terrainVertices[i1].normal += normal;   terrainVertices[i1].normal /= 2.0f;
			terrainVertices[i2].normal += normal;   terrainVertices[i2].normal /= 2.0f;
			terrainVertices[i3].normal += normal;   terrainVertices[i3].normal /= 2.0f;

			terrainVertices[i1].normal = glm::normalize(terrainVertices[i1].normal);
			terrainVertices[i2].normal = glm::normalize(terrainVertices[i2].normal);
			terrainVertices[i3].normal = glm::normalize(terrainVertices[i3].normal);


			// Second triangle
            i1 = z * width + x + width;
            i2 = z * width + x + 1;
            i3 = z * width + x + width + 1;

            indices[i++] = i1;
            indices[i++] = i2;
            indices[i++] = i3;

            collisionMesh[i - 3] = terrainVertices[i1].position;
            collisionMesh[i - 2] = terrainVertices[i2].position;
            collisionMesh[i - 1] = terrainVertices[i3].position;


            normal = glm::cross(terrainVertices[i3].position - terrainVertices[i2].position,
                                terrainVertices[i1].position - terrainVertices[i2].position);
			normal = glm::normalize(normal);

			terrainVertices[i1].normal += normal;   terrainVertices[i1].normal /= 2.0f;
			terrainVertices[i2].normal += normal;   terrainVertices[i2].normal /= 2.0f;
			terrainVertices[i3].normal += normal;   terrainVertices[i3].normal /= 2.0f;

			terrainVertices[i1].normal = glm::normalize(terrainVertices[i1].normal);
			terrainVertices[i2].normal = glm::normalize(terrainVertices[i2].normal);
			terrainVertices[i3].normal = glm::normalize(terrainVertices[i3].normal);
        }
    }


    // Save height and normals into new tga file (r - height; g, b, a - normal)
	unsigned char* heightmapDataWithNormals = new unsigned char[width * height * 4];
    for (int z = 0; z < height; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
			if (is16bit)
			{
				heightmapDataWithNormals[((height - z - 1) * width + x) * 4] = static_cast<unsigned char>(heightmapData16[((height - z - 1) * width + x) * chanels] / 65025.0f * 255);
			}
			else
			{
				heightmapDataWithNormals[((height - z - 1) * width + x) * 4] = heightmapData[((height - z - 1) * width + x) * chanels];
			}
			heightmapDataWithNormals[((height - z - 1) * width + x) * 4 + 1] = static_cast<unsigned char>((terrainVertices[z * width + x].normal.x * 0.5f + 0.5f) * 255);
			heightmapDataWithNormals[((height - z - 1) * width + x) * 4 + 2] = static_cast<unsigned char>((terrainVertices[z * width + x].normal.y * 0.5f + 0.5f) * 255);
			heightmapDataWithNormals[((height - z - 1) * width + x) * 4 + 3] = static_cast<unsigned char>((terrainVertices[z * width + x].normal.z * 0.5f + 0.5f) * 255);
        }
    }

    stbi_flip_vertically_on_write(0);
    stbi_write_tga(createTerrainHeightAndNormalMapFileName(heightmapFilename).c_str(), width, height, STBI_rgb_alpha, heightmapDataWithNormals);


    std::vector<MeshMender::Vertex> v;
    std::vector<unsigned int> in;

    for (int i = 0; i < terrainVerticesSize; ++i)
    {
        MeshMender::Vertex vertex;
        vertex.pos = terrainVertices[i].position;
        vertex.s = terrainVertices[i].texCoord.x;
        vertex.t = terrainVertices[i].texCoord.y;
        vertex.normal = terrainVertices[i].normal;
        vertex.tangent = terrainVertices[i].tangent;
        vertex.binormal = terrainVertices[i].bitangent;

        v.push_back(vertex);
    }

    for (int i = 0; i < indicesSize; ++i)
    {
        in.push_back(indices[i]);
    }

    MeshMender mender;
	std::vector<unsigned int> temp;
	mender.Mend(v, in, temp, 0.0, 0.7, 0.7, 1.0, MeshMender::CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);



	Vertex* vert = new Vertex[v.size()];
	unsigned int* ind = new unsigned int[in.size()];

	for (int i = 0; i < v.size(); ++i)
    {
        vert[i].position = v[i].pos;
        vert[i].texCoord = glm::vec2(v[i].s, v[i].t);
        vert[i].normal = v[i].normal;
        vert[i].tangent = v[i].tangent;
        vert[i].bitangent = v[i].binormal;
    }

    for (int i = 0; i < in.size(); ++i)
    {
        ind[i] = in[i];
    }


    StaticModelMesh* meshes = new StaticModelMesh[1];
    Material* materials = new Material[1];
    materials[0] = material;
    meshes[0].setMeshData(vert, v.size(), ind, in.size(), 0, materials[0].shader);

    StaticModelNode* modelNode = new StaticModelNode;
    modelNode->name = "terrain";
    modelNode->meshes = meshes;
    modelNode->meshesCount = 1;
    modelNode->parent = NULL;

    RStaticModel* model = new RStaticModel("", modelNode, materials, 1, GL_TRIANGLES, collisionMesh, indicesSize);


	if (heightmapData != NULL)
	{
		stbi_image_free(heightmapData);
	}
	if (heightmapData16 != NULL)
	{
		stbi_image_free(heightmapData16);
	}


    return model;
}


RStaticModel* TerrainLoader::loadTerrainModel(const char* heightmapFilename, std::string materialFileName, std::string materialName, std::string texturePath, float maxHeight, bool is16bit)
{
    std::string terFileName = createTerFileName(heightmapFilename);

    if (!FilesHelper::isFileExists(terFileName))
    {
        RStaticModel* model = loadTerrainFromHeightmap(heightmapFilename, materialFileName, materialName, texturePath, maxHeight, is16bit);

        saveTerFile(terFileName.c_str(), model);

        return model;
    }
    else
    {
        return loadTerFile(terFileName.c_str(), materialFileName, texturePath);
    }
}
