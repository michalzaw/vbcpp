#ifndef LOADTERRAINMODEL_H_INCLUDED
#define LOADTERRAINMODEL_H_INCLUDED


#include <glm/glm.hpp>

#include <SOIL/soil.h>

#include "Model.h"
#include "Material.h"
#include "NVMeshMender/NVMeshMender.h"


Model* loadTerrainModel(const char* heightmapFilename, Material& material, float maxHeight)
{
    const float cellSize = 1.0f;

    int width, height, chanels;
    unsigned char* heightmapData = SOIL_load_image(heightmapFilename, &width, &height, &chanels, SOIL_LOAD_L);

    glm::vec3 startPosition(static_cast<float>(width) / -2.0f, 0.0f, static_cast<float>(height) / 2.0f);
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
            int color = static_cast<int>(heightmapData[(height - z - 1) * width + x]);
            vertexPosition.y = static_cast<float>(color) / 255.0f * maxHeight;


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


    Mesh* meshes = new Mesh[1];
    meshes[0].material = material;
    meshes[0].firstVertex = 0;
    meshes[0].quantumOfVertice = indicesSize;

    Model* model = new Model(terrainVertices, terrainVerticesSize, indices, indicesSize, meshes, 1, collisionMesh, indicesSize);


    return model;
}


// Mesh mender
/*Model* loadTerrainModel(const char* heightmapFilename, Material& material, float maxHeight)
{
    const float cellSize = 1.0f;

    int width, height, chanels;
    unsigned char* heightmapData = SOIL_load_image(heightmapFilename, &width, &height, &chanels, SOIL_LOAD_L);

    glm::vec3 startPosition(static_cast<float>(width) / -2.0f, 0.0f, static_cast<float>(height) / 2.0f);
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
            int color = static_cast<int>(heightmapData[(height - z - 1) * width + x]);
            vertexPosition.y = static_cast<float>(color) / 255.0f * maxHeight;


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


    Mesh* meshes = new Mesh[1];
    meshes[0].material = material;
    meshes[0].firstVertex = 0;
    meshes[0].quantumOfVertice = in.size();

    Model* model = new Model(vert, v.size(), ind, in.size(), meshes, 1, collisionMesh, indicesSize);


    return model;
}*/


#endif // LOADTERRAINMODEL_H_INCLUDED
