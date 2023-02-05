#include "ModelGenerator.h"

#include "../Utils/RStaticModel.h"


namespace ModelGenerator
{
	glm::vec3* generateCollistionMesh(StaticModelMesh* meshes, unsigned int meshesCount, unsigned int totalIndicesCount)
	{
		glm::vec3* collisionMesh = new glm::vec3[totalIndicesCount];
		int counter = 0;

		for (int i = 0; i < meshesCount; ++i)
		{
			unsigned int indicesCount = meshes[i].indicesCount;

			for (int j = 0; j < indicesCount; ++j)
			{
				collisionMesh[counter + j] = VerticesUtils::getVertexPosition(meshes[i].vertices,
																			  meshes[i].vertexType,
																			  meshes[i].indices[j] - meshes[i].firstVertexInVbo);
			}

			counter += indicesCount;
		}

		return collisionMesh;
	}
}
