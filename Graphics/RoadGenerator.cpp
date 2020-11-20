#include "RoadGenerator.h"

#include "../Game/GameConfig.h"


namespace RoadGenerator
{
	glm::vec2 calculateDirection(const std::vector<glm::vec3>& points, const std::vector<RoadConnectionPointData*>& roadConnectionPoints, int currentPointIndex)
	{
		// first point - if road is connected to crossroad
		if (currentPointIndex == 0 && roadConnectionPoints[0] != nullptr)
		{
			return glm::vec2(roadConnectionPoints[0]->direction.x, roadConnectionPoints[0]->direction.z);

		}
		// last point - if road is connected to crossroad
		else if (currentPointIndex == points.size() - 1 && roadConnectionPoints[1] != nullptr)
		{
			return -glm::vec2(roadConnectionPoints[1]->direction.x, roadConnectionPoints[1]->direction.z);
		}
		// standard case
		else if (currentPointIndex < points.size() - 1)
		{
			return glm::normalize(glm::vec2(points[currentPointIndex + 1].x - points[currentPointIndex].x, points[currentPointIndex + 1].z - points[currentPointIndex].z));
		}
		// last point
		else
		{
			return glm::normalize(glm::vec2(points[currentPointIndex].x - points[currentPointIndex - 1].x, points[currentPointIndex].z - points[currentPointIndex - 1].z));
		}
	}

	glm::vec3 calculateNormalForPointsConnectedToCrossroad(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3, RoadConnectionPointData* roadConnectionPointData)
	{
		glm::vec3 v1 = point1;
		glm::vec3 v2 = point2;
		glm::vec3 v3 = point3;

		v3.y = v2.y;

		return glm::normalize(glm::cross(v3 - v2, v1 - v2));
	}

	StaticModelMesh* createMeshes(std::vector<MeshMender::Vertex>* lanesVerticesArray, std::vector<unsigned int>* lanesIndicesArray, const std::vector<RoadLane>& roadLanes, unsigned int& indicesCountInAllMeshes, bool isGame, StaticModelMesh* oldMeshes)
	{
		StaticModelMesh* meshes = oldMeshes == nullptr ? new StaticModelMesh[roadLanes.size()] : oldMeshes;

		indicesCountInAllMeshes = 0;
		for (int i = 0; i < roadLanes.size(); ++i)
		{
			MeshMender mender;
			std::vector<unsigned int> temp;
			mender.Mend(lanesVerticesArray[i], lanesIndicesArray[i], temp, 0.0, 0.7, 0.7, 1.0, MeshMender::DONT_CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);

			unsigned int verticesCount = lanesVerticesArray[i].size();
			unsigned int indicesCount = lanesIndicesArray[i].size();
			indicesCountInAllMeshes += indicesCount;

			Vertex* vertices = new Vertex[verticesCount];
			unsigned int* indices = new unsigned int[indicesCount];

			for (int j = 0; j < verticesCount; ++j)
			{
				vertices[j].position = lanesVerticesArray[i][j].pos;
				vertices[j].texCoord = glm::vec2(lanesVerticesArray[i][j].s, lanesVerticesArray[i][j].t);
				vertices[j].normal = lanesVerticesArray[i][j].normal;
				vertices[j].tangent = lanesVerticesArray[i][j].tangent;
				vertices[j].bitangent = lanesVerticesArray[i][j].binormal;
			}
			for (int j = 0; j < indicesCount; ++j)
			{
				indices[j] = lanesIndicesArray[i][j];
			}

			if (oldMeshes == nullptr)
			{
				if (isGame)
				{
					meshes[i].setMeshData(vertices, verticesCount, indices, indicesCount, i, roadLanes[i].material.shader);
				}
				else // GM_EDITOR
				{
					// vbo for ~ 4600 vertices
					meshes[i].setMeshData(vertices, verticesCount, indices, indicesCount, i, roadLanes[i].material.shader, false, 1024 * 1024, 1024 * 1024, false);
				}
			}
			else
			{
				meshes[i].updateMeshData(vertices, verticesCount, indices, indicesCount);
			}
		}

		return meshes;
	}

	glm::vec3* generateCollistionMesh(std::vector<unsigned int>* lanesIndicesArray, StaticModelMesh* meshes, unsigned int totalIndicesCount, unsigned int lanesCount)
	{
		glm::vec3* collisionMesh = new glm::vec3[totalIndicesCount];
		int counter = 0;
		for (int i = 0; i < lanesCount; ++i)
		{
			unsigned int indicesCount = lanesIndicesArray[i].size();

			for (int j = 0; j < indicesCount; ++j)
			{
				collisionMesh[counter + j] = meshes[i].vertices[meshes[i].indices[j] - meshes[i].firstVertexInVbo].position;
			}

			counter += indicesCount;
		}

		return collisionMesh;
	}

	RStaticModel* generateModel(const std::vector<RoadLane>& roadLanes, StaticModelMesh* meshes, glm::vec3* collisionMesh, int collistionMeshSize)
	{
		int lanesCount = roadLanes.size();

		// materials
		Material* materials = new Material[lanesCount];
		for (int i = 0; i < lanesCount; ++i)
		{
			materials[i] = roadLanes[i].material;
		}

		StaticModelNode* modelNode = new StaticModelNode;
		modelNode->name = "road";
		modelNode->meshes = meshes;
		modelNode->meshesCount = lanesCount;
		modelNode->parent = NULL;

		return new RStaticModel("", modelNode, materials, lanesCount, GL_TRIANGLES, collisionMesh, collistionMeshSize);
	}

	RStaticModel* updateOldModel(RStaticModel* oldModel, glm::vec3* collisionMesh, int collistionMeshSize)
	{
		oldModel->setNewCollisionMesh(collisionMesh, collistionMeshSize);
		oldModel->recalculateAABB();

		return oldModel;
	}

	RStaticModel* createRoadModel(const std::vector<RoadLane>& roadLanes, const std::vector<glm::vec3>& points,
								  const std::vector<RoadConnectionPointData*>& roadConnectionPoints, RStaticModel* oldModel)
	{
		int lanesCount = roadLanes.size();

		std::vector<MeshMender::Vertex>* lanesVerticesArray = new std::vector<MeshMender::Vertex>[lanesCount];
		std::vector<unsigned int>* lanesIndicesArray = new std::vector<unsigned int>[lanesCount];

		for (int i = 0; i < lanesCount; ++i)
		{
			float distanceFromLaneBegin = 0;
			glm::vec3 lastPoint;

			for (int j = 0; j < points.size(); ++j)
			{
				glm::vec2 currentCurvePoint = glm::vec2(points[j].x, points[j].z);
				glm::vec2 direction = calculateDirection(points, roadConnectionPoints, j);
				glm::vec2 rightVector = glm::vec2(-direction.y, direction.x);

				glm::vec2 point1 = currentCurvePoint + roadLanes[i].r1 * rightVector;
				glm::vec2 point2 = currentCurvePoint + roadLanes[i].r2 * rightVector;

				float y = points[j].y;

				MeshMender::Vertex vertex1;
				vertex1.pos = glm::vec3(point1.x, y + roadLanes[i].height1, point1.y);
				vertex1.normal = glm::vec3(0.0f, 0.0f, 0.0f);

				MeshMender::Vertex vertex2;
				vertex2.pos = glm::vec3(point2.x, y + roadLanes[i].height2, point2.y);
				vertex2.normal = glm::vec3(0.0f, 0.0f, 0.0f);

				glm::vec3 centerPoint = glm::vec3((vertex1.pos + vertex2.pos) / 2.0f);
				if (j > 0)
				{
					distanceFromLaneBegin += glm::length(lastPoint - centerPoint);
				}
				lastPoint = centerPoint;

				vertex1.s = 1.0f * roadLanes[i].material.scale.x + roadLanes[i].material.offset.x;
				vertex1.t = distanceFromLaneBegin * roadLanes[i].material.scale.y + roadLanes[i].material.offset.y;
				vertex2.s = 0.0f * roadLanes[i].material.scale.x + roadLanes[i].material.offset.x;
				vertex2.t = distanceFromLaneBegin * roadLanes[i].material.scale.y + roadLanes[i].material.offset.y;

				lanesVerticesArray[i].push_back(vertex1);
				lanesVerticesArray[i].push_back(vertex2);
			}

			for (int j = 0; j < (points.size() - 1) * 2; j += 2)
			{
				int trianglesIndices[2][3] = {
					{ j, j + 1, j + 3 },
					{ j, j + 3, j + 2 }
				};

				for (int k = 0; k < 2; ++k)
				{
					unsigned int i1 = trianglesIndices[k][0];
					unsigned int i2 = trianglesIndices[k][1];
					unsigned int i3 = trianglesIndices[k][2];

					// indices
					lanesIndicesArray[i].push_back(i1);
					lanesIndicesArray[i].push_back(i2);
					lanesIndicesArray[i].push_back(i3);

					// normals
					glm::vec3 normal = glm::cross(glm::vec3(lanesVerticesArray[i][i3].pos) - glm::vec3(lanesVerticesArray[i][i2].pos),
												  glm::vec3(lanesVerticesArray[i][i1].pos) - glm::vec3(lanesVerticesArray[i][i2].pos));
					normal = glm::normalize(normal);

					lanesVerticesArray[i][i1].normal += normal;   lanesVerticesArray[i][i1].normal /= 2.0f;
					lanesVerticesArray[i][i2].normal += normal;   lanesVerticesArray[i][i2].normal /= 2.0f;
					lanesVerticesArray[i][i3].normal += normal;   lanesVerticesArray[i][i3].normal /= 2.0f;

					lanesVerticesArray[i][i1].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i1].normal));
					lanesVerticesArray[i][i2].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i2].normal));
					lanesVerticesArray[i][i3].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i3].normal));
				}
			}

			// Calculate normal for first and last point in lane - if road is connected to crossroad
			int firstVertexForLastConnectionPoint = lanesVerticesArray[i].size() - 4;
			int trianglesIndices[2][3] = {
					{ 0, 1, 3 },
					{ firstVertexForLastConnectionPoint + 0, firstVertexForLastConnectionPoint + 3, firstVertexForLastConnectionPoint + 2 }
			};

			for (int j = 0; j < 2; ++j)
			{
				if (roadConnectionPoints[j] != nullptr)
				{
					unsigned int i1 = trianglesIndices[j][0];
					unsigned int i2 = trianglesIndices[j][1];
					unsigned int i3 = trianglesIndices[j][2];

					glm::vec3 normal = calculateNormalForPointsConnectedToCrossroad(lanesVerticesArray[i][i1].pos, lanesVerticesArray[i][i2].pos, lanesVerticesArray[i][i3].pos, roadConnectionPoints[0]);

					lanesVerticesArray[i][i1].normal = normal;
					lanesVerticesArray[i][i2].normal = normal;
				}
			}
		}

		unsigned int totalIndicesCount = 0;
		bool isGame = GameConfig::getInstance().mode == GM_GAME;
		StaticModelMesh* meshes = createMeshes(lanesVerticesArray, lanesIndicesArray, roadLanes, totalIndicesCount, 
											   isGame, oldModel == nullptr ? nullptr : oldModel->getRootNode()->meshes);

		glm::vec3* collisionMesh = generateCollistionMesh(lanesIndicesArray, meshes, totalIndicesCount, lanesCount);

		RStaticModel* model;
		if (oldModel == nullptr)
		{
			model = generateModel(roadLanes, meshes, collisionMesh, totalIndicesCount);
		}
		else
		{
			model = updateOldModel(oldModel, collisionMesh, totalIndicesCount);
		}

		delete[] lanesVerticesArray;
		delete[] lanesIndicesArray;

		return model;
	}
}