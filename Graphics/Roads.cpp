#include "Roads.h"

#include "ModelGenerator.h"

#include "../Game/GameConfig.h"

#include "../Utils/BezierCurvesUtils.h"


// Current stable
RStaticModel* createRoadModel(std::vector<RoadLane>& roadLanes, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, std::vector<RoadConnectionPointData*>& roadConnectionPoints, RStaticModel* oldModel)
{
	int lanesCount = roadLanes.size();

	std::vector<MeshMender::Vertex>* lanesVerticesArray = new std::vector<MeshMender::Vertex>[lanesCount];
	std::vector<unsigned int>* lanesIndicesArray = new std::vector<unsigned int>[lanesCount];
	float* t = new float[lanesCount];       // wsp. do obliczania texCoordow
	glm::vec3* lastPoints = new glm::vec3[lanesCount];

	for (int k = 0; k < segments.size(); ++k)
	{
		// Calculate circle center
		glm::vec3 segmentBegin = points[k];
		glm::vec3 segmentEnd = points[k + 1];
		if (segments[k].type == RST_BEZIER_CURVE)
		{
			segmentBegin = points[3 * k];
			segmentEnd = points[3 * k + 3];
		}

		float x1 = segmentBegin.x;
		float x2 = segmentEnd.x;
		float z1 = segmentBegin.z;
		float z2 = segmentEnd.z;
		float q = sqrt((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1));
		float x3 = (x1 + x2) / 2;
		float z3 = (z1 + z2) / 2;

		glm::vec2 center;
		if (segments[k].r > 0)
		{
			center.x = x3 + sqrt(segments[k].r * segments[k].r - (q / 2) * (q / 2)) * (z1 - z2) / q;
			center.y = z3 + sqrt(segments[k].r * segments[k].r - (q / 2) * (q / 2)) * (x2 - x1) / q;
		}
		else if (segments[k].r < 0)
		{
			center.x = x3 - sqrt(segments[k].r * segments[k].r - (q / 2) * (q / 2)) * (z1 - z2) / q;
			center.y = z3 - sqrt(segments[k].r * segments[k].r - (q / 2) * (q / 2)) * (x2 - x1) / q;
		}

		float angle1 = atan2(segmentBegin.z - center.y, segmentBegin.x - center.x);
		float angle2 = atan2(segmentEnd.z - center.y, segmentEnd.x - center.x);


		int pointsCount = segments[k].pointsCount;
		//float deltaAngle = fabs(angle2 - angle1) / (pointsCount - 1);
		//float deltaAngle = (angle2 - angle1) / (pointsCount - 1);
		float deltaAngle = asin(sqrt((segmentBegin.x - segmentEnd.x) * (segmentBegin.x - segmentEnd.x) +
			(segmentBegin.z - segmentEnd.z) * (segmentBegin.z - segmentEnd.z)) / 2.0f /
			fabs(segments[k].r)) * 2.0f / (pointsCount - 1);

		if (segments[k].r < 0)
		{
			deltaAngle = -deltaAngle;
		}

		float originalR = segments[k].r;
		bool is = segments[k].r < 0;
		segments[k].r = fabs(segments[k].r);


		int indicesCountInLane = (pointsCount - 1) * 6;


		std::vector<glm::vec3> bezierCurvePoints;
		int bezierCurveBeginSegmentPoint = k * 3;
		if (segments[k].type == RST_BEZIER_CURVE)
		{
			BezierCurvesUtils::generateBezierCurvePoints(points[bezierCurveBeginSegmentPoint], points[bezierCurveBeginSegmentPoint + 1], points[bezierCurveBeginSegmentPoint + 2], points[bezierCurveBeginSegmentPoint + 3], pointsCount, bezierCurvePoints);
		}


		for (int i = 0; i < lanesCount; ++i)
		{
			float angle = angle1;

			int verticesCountInLane = lanesVerticesArray[i].size();

			int j = 0;
			if (k > 0)
			{
				j = 2;
				verticesCountInLane -= 2;
				angle += deltaAngle;
			}
			//lanesVerticesArray[i].push_back(MeshMender::Vertex());   //first and second vertex
			//lanesVerticesArray[i].push_back(MeshMender::Vertex());

			for (; j < pointsCount * 2; j += 2)
			{
				//glm::vec2 point1(center.x + segments[k].r * cosf(angle),
				//                 center.y + segments[k].r * sinf(angle));

				//angle += deltaAngle;

				glm::vec2 point;
				glm::vec2 v;
				if (segments[k].type == RST_ARC)
				{
					point = glm::vec2(center.x + segments[k].r * cosf(angle),
						center.y + segments[k].r * sinf(angle));

					v = glm::normalize(center - point);

					if (is)
						v = -v;
				}
				else if (segments[k].type == RST_LINE)
				{
					point.x = segmentBegin.x + (segmentEnd.x - segmentBegin.x) * (j / static_cast<float>(pointsCount * 2 - 2));
					point.y = segmentBegin.z + (segmentEnd.z - segmentBegin.z) * (j / static_cast<float>(pointsCount * 2 - 2));

					glm::vec2 d = glm::normalize(glm::vec2(segmentEnd.x - segmentBegin.x, segmentEnd.z - segmentBegin.z));
					v = glm::vec2(-d.y, d.x);
				}
				else if (segments[k].type == RST_BEZIER_CURVE)
				{
					point.x = bezierCurvePoints[j / 2].x;
					point.y = bezierCurvePoints[j / 2].z;

					if (j < pointsCount * 2 - 2)
					{
						glm::vec2 d = glm::normalize(glm::vec2(bezierCurvePoints[j / 2 + 1].x - bezierCurvePoints[j / 2].x, bezierCurvePoints[j / 2 + 1].z - bezierCurvePoints[j / 2].z));
						v = glm::vec2(-d.y, d.x);
					}
					else
					{
						glm::vec2 d = glm::normalize(glm::vec2(bezierCurvePoints[j / 2].x - bezierCurvePoints[j / 2 - 1].x, bezierCurvePoints[j / 2].z - bezierCurvePoints[j / 2 - 1].z));
						v = glm::vec2(-d.y, d.x);
					}
				}

				// first point in first vertex - if road is connected to crossroad
				if (k == 0 && j == 0 && roadConnectionPoints[0] != nullptr)
				{
					glm::vec2 crossroadDirection = glm::vec2(roadConnectionPoints[0]->direction.x, roadConnectionPoints[0]->direction.z);

					v = glm::vec2(-crossroadDirection.y, crossroadDirection.x);
				}
				// last point in last segment - if road is connected to crossroad
				if (k == segments.size() - 1 && j == pointsCount * 2 - 2 && roadConnectionPoints[1] != nullptr)
				{
					glm::vec2 crossroadDirection = -glm::vec2(roadConnectionPoints[1]->direction.x, roadConnectionPoints[1]->direction.z);

					v = glm::vec2(-crossroadDirection.y, crossroadDirection.x);
				}


				//glm::vec2 a = point2 - point1;
				//glm::vec2 v = glm::normalize(glm::vec2(a.y, a.x));


				glm::vec2 p1 = point + roadLanes[i].r1 * v;
				glm::vec2 p2 = point + roadLanes[i].r2 * v;

				float y;
				if (segments[k].interpolation == RI_LIN)
					y = segmentBegin.y + (segmentEnd.y - segmentBegin.y) * (j / static_cast<float>(pointsCount * 2 - 2));
				else if (segments[k].interpolation == RI_COS)
					y = segmentBegin.y + (segmentEnd.y - segmentBegin.y) * ((-cosf((j / static_cast<float>(pointsCount * 2 - 2)) * PI) + 1) / 2.0f);

				MeshMender::Vertex vertex1;
				vertex1.pos = glm::vec3(p1.x, y + roadLanes[i].height1, p1.y);
				//vertex1.s = p1.x; //p1.x; //0.0f * roadLanes[i].material.scale.x; //p1.x * roadLanes[i].material.scale.x; // + roadLanes[i].material.offset.x;//
				//vertex1.t = p1.y; //p1.y; //0.0f * roadLanes[i].material.scale.y; //p1.y * roadLanes[i].material.scale.y; //j / 2.0f * roadLanes[i].material.scale.y + roadLanes[i].material.offset.y;//

				vertex1.normal = glm::vec3(0.0f, 0.0f, 0.0f);

				MeshMender::Vertex vertex2;
				vertex2.pos = glm::vec3(p2.x, y + roadLanes[i].height2, p2.y);
				//vertex2.s = p2.x; //1.0f * roadLanes[i].material.scale.x; //p2.x * roadLanes[i].material.scale.x;// + roadLanes[i].material.offset.x;//
				//vertex2.t = p2.y; //1.0f * roadLanes[i].material.scale.y; //p2.y * roadLanes[i].material.scale.y;// + roadLanes[i].material.offset.y;//

				vertex2.normal = glm::vec3(0.0f, 0.0f, 0.0f);

				glm::vec3 centerPoint = glm::vec3((vertex1.pos + vertex2.pos) / 2.0f);
				if (j == 0)
				{
					lastPoints[i] = centerPoint;
					t[i] = 0.0f;
				}
				t[i] += glm::length(lastPoints[i] - centerPoint);
				lastPoints[i] = centerPoint;

				vertex1.s = 1.0f * roadLanes[i].material->scale.x + roadLanes[i].material->offset.x;//
				vertex1.t = t[i] * roadLanes[i].material->scale.y + roadLanes[i].material->offset.y;//
				vertex2.s = 0.0f * roadLanes[i].material->scale.x + roadLanes[i].material->offset.x;//
				vertex2.t = t[i] * roadLanes[i].material->scale.y + roadLanes[i].material->offset.y;//

				lanesVerticesArray[i].push_back(vertex1);   //lanesVerticesArray[i][j];
				lanesVerticesArray[i].push_back(vertex2);   //lanesVerticesArray[i][j];

				angle += deltaAngle;
			}


			int c = 0;
			for (int j = 0; j < (pointsCount - 1) * 2; j += 2)
			{
				// First triangle
				unsigned int i1 = verticesCountInLane + j;
				unsigned int i2 = verticesCountInLane + j + 1;
				unsigned int i3 = verticesCountInLane + j + 3;

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

				// Second triangle
				i1 = verticesCountInLane + j;
				i2 = verticesCountInLane + j + 3;
				i3 = verticesCountInLane + j + 2;

				// indices
				lanesIndicesArray[i].push_back(i1);
				lanesIndicesArray[i].push_back(i2);
				lanesIndicesArray[i].push_back(i3);

				// normals
				normal = glm::cross(glm::vec3(lanesVerticesArray[i][i3].pos) - glm::vec3(lanesVerticesArray[i][i2].pos),
					glm::vec3(lanesVerticesArray[i][i1].pos) - glm::vec3(lanesVerticesArray[i][i2].pos));
				normal = glm::normalize(normal);

				lanesVerticesArray[i][i1].normal += normal;   lanesVerticesArray[i][i1].normal /= 2.0f;
				lanesVerticesArray[i][i2].normal += normal;   lanesVerticesArray[i][i2].normal /= 2.0f;
				lanesVerticesArray[i][i3].normal += normal;   lanesVerticesArray[i][i3].normal /= 2.0f;

				lanesVerticesArray[i][i1].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i1].normal));
				lanesVerticesArray[i][i2].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i2].normal));
				lanesVerticesArray[i][i3].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i3].normal));
			}

			// Calculate normal for first and last point in lane - if road is connected to crossroad
			if (k == 0 && roadConnectionPoints[0] != nullptr)
			{
				unsigned int i1 = verticesCountInLane + 0;
				unsigned int i2 = verticesCountInLane + 0 + 1;
				unsigned int i3 = verticesCountInLane + 0 + 3;

				glm::vec3 v1 = lanesVerticesArray[i][i1].pos;
				glm::vec3 v2 = lanesVerticesArray[i][i2].pos;
				glm::vec3 v3 = lanesVerticesArray[i][i3].pos;

				v3.y = v2.y;

				glm::vec3 normal = glm::cross(v3 - v2, v1 - v2);
				normal = glm::normalize(normal);

				lanesVerticesArray[i][i1].normal = normal;
				lanesVerticesArray[i][i2].normal = normal;

				lanesVerticesArray[i][i1].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i1].normal));
				lanesVerticesArray[i][i2].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i2].normal));
			}
			if (k == segments.size() - 1 && roadConnectionPoints[1] != nullptr)
			{
				// last quad
				unsigned int firstVertex = lanesVerticesArray[i].size() - 4;
				unsigned int i1 = firstVertex + 0;
				unsigned int i2 = firstVertex + 0 + 3;
				unsigned int i3 = firstVertex + 0 + 2;

				glm::vec3 v1 = lanesVerticesArray[i][i1].pos;
				glm::vec3 v2 = lanesVerticesArray[i][i2].pos;
				glm::vec3 v3 = lanesVerticesArray[i][i3].pos;

				v1.y = v3.y;

				glm::vec3 normal = glm::cross(v3 - v2, v1 - v2);
				normal = glm::normalize(normal);

				lanesVerticesArray[i][i2].normal = normal;
				lanesVerticesArray[i][i3].normal = normal;

				lanesVerticesArray[i][i2].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i2].normal));
				lanesVerticesArray[i][i3].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i3].normal));
			}
		}

		segments[k].r = originalR;
	}


	// meshes
	StaticModelMesh* meshes = oldModel == NULL ? new StaticModelMesh[lanesCount] : oldModel->getRootNode()->meshes;

	unsigned int indicesCountInAllMeshes = 0;
	for (int i = 0; i < lanesCount; ++i)
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

		// vbo for ~ 4600 vertices in GM_EDITOR or shared vbo in GM_GAME
		meshes[i].setMeshData(vertices, verticesCount, indices, indicesCount, i, roadLanes[i].material->shader, GameConfig::getInstance().mode == GM_GAME, 1024 * 1024, 1024 * 1024, false);
	}


	// collistion mesh
	glm::vec3* collisionMesh = ModelGenerator::generateCollistionMesh(meshes, lanesCount, indicesCountInAllMeshes);


	RStaticModel* model;
	if (oldModel == NULL)
	{
		// materials
		std::vector<Material*> materials;
		for (int i = 0; i < lanesCount; ++i)
		{
			materials.push_back(new Material(*roadLanes[i].material));
		}

		StaticModelNode* modelNode = new StaticModelNode;
		modelNode->name = "road";
		modelNode->meshes = meshes;
		modelNode->meshesCount = lanesCount;
		modelNode->parent = NULL;

		model = new RStaticModel("", modelNode, materials, GL_TRIANGLES, collisionMesh, indicesCountInAllMeshes);
	}
	else
	{
		model = oldModel;
		model->setNewCollisionMesh(collisionMesh, indicesCountInAllMeshes);
		model->recalculateAABB();
	}


	delete[] lanesVerticesArray;
	delete[] lanesIndicesArray;
	delete[] t;
	delete[] lastPoints;


    return model;
}


/*Model* createRoadModel(RoadLane* roadLanes, int lanesCount, RoadSegment& segment)
{
    int pointsCount = 60;
    float deltaAngle = fabs(segment.angle2 - segment.angle1) / (pointsCount - 1);

    //int verticesCount = 2 * lanesCount * pointsCount;
    //Vertex* vertices = new Vertex[verticesCount];

    int indicesCountInLane = (pointsCount - 1) * 6;
    //int indicesCount = lanesCount * indicesCountInLane;
    //unsigned int* indices = new unsigned int[indicesCount];

    std::vector<MeshMender::Vertex>* lanesVerticesArray = new std::vector<MeshMender::Vertex>[lanesCount];
    std::vector<unsigned int>* lanesIndicesArray = new std::vector<unsigned int>[lanesCount];
    Mesh* meshes = new Mesh[lanesCount];

    int verticesCount = 0;
    int indicesCount = 0;

    for (int i = 0; i < lanesCount; ++i)
    {
        float angle = segment.angle1;


        for (int j = 0; j < pointsCount * 2; j += 2)
        {
            glm::vec2 point(segment.center.x + segment.r * cosf(angle),
                            segment.center.y + segment.r * sinf(angle));

            glm::vec2 v = glm::normalize(segment.center - point);

            glm::vec2 p1 = point + roadLanes[i].r1 * v;
            glm::vec2 p2 = point + roadLanes[i].r2 * v;

            MeshMender::Vertex vertex1;
            vertex1.pos = glm::vec3(p1.x, roadLanes[i].height1, p1.y);
            //vertex1.s = p1.x; //p1.x; //0.0f * roadLanes[i].material.scale.x; //p1.x * roadLanes[i].material.scale.x; // + roadLanes[i].material.offset.x;//
            //vertex1.t = p1.y; //p1.y; //0.0f * roadLanes[i].material.scale.y; //p1.y * roadLanes[i].material.scale.y; //j / 2.0f * roadLanes[i].material.scale.y + roadLanes[i].material.offset.y;//
            vertex1.s = 1.0f * roadLanes[i].material.scale.x + roadLanes[i].material.offset.x;//
            vertex1.t = j / 2.0f * roadLanes[i].material.scale.y + roadLanes[i].material.offset.y;//
            vertex1.normal = glm::vec3(0.0f, 0.0f, 0.0f);

            MeshMender::Vertex vertex2;
            vertex2.pos = glm::vec3(p2.x, roadLanes[i].height2, p2.y);
            //vertex2.s = p2.x; //1.0f * roadLanes[i].material.scale.x; //p2.x * roadLanes[i].material.scale.x;// + roadLanes[i].material.offset.x;//
            //vertex2.t = p2.y; //1.0f * roadLanes[i].material.scale.y; //p2.y * roadLanes[i].material.scale.y;// + roadLanes[i].material.offset.y;//
            vertex2.s = 0.0f * roadLanes[i].material.scale.x + roadLanes[i].material.offset.x;//
            vertex2.t = j / 2.0f * roadLanes[i].material.scale.y + roadLanes[i].material.offset.y;//
            vertex2.normal = glm::vec3(0.0f, 0.0f, 0.0f);

            lanesVerticesArray[i].push_back(vertex1);   //lanesVerticesArray[i][j];
            lanesVerticesArray[i].push_back(vertex2);   //lanesVerticesArray[i][j];

            angle += deltaAngle;
        }


        int c = 0;
        for (int j = 0; j < (pointsCount - 1) * 2; j += 2)
        {
            // First triangle
            unsigned int i1 = j;
            unsigned int i2 = j + 1;
            unsigned int i3 = j + 3;

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

            // Second triangle
            i1 = j;
            i2 = j + 3;
            i3 = j + 2;

            // indices
            lanesIndicesArray[i].push_back(i1);
            lanesIndicesArray[i].push_back(i2);
            lanesIndicesArray[i].push_back(i3);

            // normals
            normal = glm::cross(glm::vec3(lanesVerticesArray[i][i3].pos) - glm::vec3(lanesVerticesArray[i][i2].pos),
                                glm::vec3(lanesVerticesArray[i][i1].pos) - glm::vec3(lanesVerticesArray[i][i2].pos));
			normal = glm::normalize(normal);

			lanesVerticesArray[i][i1].normal += normal;   lanesVerticesArray[i][i1].normal /= 2.0f;
			lanesVerticesArray[i][i2].normal += normal;   lanesVerticesArray[i][i2].normal /= 2.0f;
			lanesVerticesArray[i][i3].normal += normal;   lanesVerticesArray[i][i3].normal /= 2.0f;

			lanesVerticesArray[i][i1].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i1].normal));
			lanesVerticesArray[i][i2].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i2].normal));
			lanesVerticesArray[i][i3].normal = glm::normalize(glm::vec3(lanesVerticesArray[i][i3].normal));
        }

        MeshMender mender;
        std::vector<unsigned int> temp;
        mender.Mend(lanesVerticesArray[i], lanesIndicesArray[i], temp, 0.0, 0.7, 0.7, 1.0, MeshMender::CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);


        meshes[i].material = roadLanes[i].material;
        meshes[i].firstVertex = indicesCount;
        meshes[i].quantumOfVertice = lanesIndicesArray[i].size();


        verticesCount += lanesVerticesArray[i].size();
        indicesCount += lanesIndicesArray[i].size();
    }


    Vertex* vertices = new Vertex[verticesCount];
    unsigned int* indices = new unsigned int[indicesCount];
    glm::vec3* collisionMesh = new glm::vec3[indicesCount];

    int vertCount = 0;
    int indCount = 0;
    for (int i = 0; i < lanesCount; ++i)
    {
        //memcpy(&vertices[vertCount], &lanesVerticesArray[i][0], lanesVerticesArray[i].size() * sizeof(lanesVerticesArray[i][0]));
        //memcpy(&indices[indCount], &lanesIndicesArray[i][0], lanesIndicesArray[i].size() * sizeof(lanesIndicesArray[i][0]));

        for (int j = 0; j < lanesVerticesArray[i].size(); ++j)
        {
            vertices[vertCount + j].position = lanesVerticesArray[i][j].pos;
            vertices[vertCount + j].texCoord = glm::vec2(lanesVerticesArray[i][j].s, lanesVerticesArray[i][j].t);
            vertices[vertCount + j].normal = lanesVerticesArray[i][j].normal;
            vertices[vertCount + j].tangent = lanesVerticesArray[i][j].tangent;
            vertices[vertCount + j].bitangent = lanesVerticesArray[i][j].binormal;
        }
        for (int j = 0; j < lanesIndicesArray[i].size(); ++j)
        {
            indices[indCount + j] = lanesIndicesArray[i][j] + vertCount;

            collisionMesh[indCount + j] = vertices[indices[indCount + j]].position;
        }

        vertCount += lanesVerticesArray[i].size();
        indCount += lanesIndicesArray[i].size();
    }


    delete[] lanesVerticesArray;
    delete[] lanesIndicesArray;


    Model* model = new Model(vertices, vertCount, indices, indCount, meshes, lanesCount, collisionMesh, indCount);

    return model;
}*/

/*================================================================================*/
/*Model* createRoadModel(RoadLane* roadLanes, int lanesCount, RoadSegment& segment)
{
    int pointsCount = 20;
    float deltaAngle = fabs(segment.angle2 - segment.angle1) / (pointsCount - 1);

    int verticesCount = 2 * lanesCount * pointsCount;
    Vertex* vertices = new Vertex[verticesCount];

    int indicesCountInLane = (pointsCount - 1) * 6;
    int indicesCount = lanesCount * indicesCountInLane;
    unsigned int* indices = new unsigned int[indicesCount];

    for (int i = 0; i < lanesCount; ++i)
    {
        float angle = segment.angle1;

        for (int j = 0; j < pointsCount * 2; j += 2)
        {
            glm::vec2 point(segment.center.x + segment.r * cosf(angle),
                            segment.center.y + segment.r * sinf(angle));

            glm::vec2 v = glm::normalize(segment.center - point);

            glm::vec2 p1 = point + roadLanes[i].r1 * v;
            glm::vec2 p2 = point + roadLanes[i].r2 * v;

            Vertex vertex1;
            vertex1.position = glm::vec3(p1.x, roadLanes[i].height1, p1.y);
            vertex1.texCoord = p1;
            vertex1.normal = glm::vec3(0.0f, 0.0f, 0.0f);

            Vertex vertex2;
            vertex2.position = glm::vec3(p2.x, roadLanes[i].height2, p2.y);
            vertex2.texCoord = p2;
            vertex2.normal = glm::vec3(0.0f, 0.0f, 0.0f);

            vertices[i * 2 * pointsCount + j] = vertex1;
            vertices[i * 2 * pointsCount + j + 1] = vertex2;

            angle += deltaAngle;
        }
    }


    for (int i = 0; i < lanesCount; ++i)
    {
        int c = 0;
        for (int j = 0; j < (pointsCount - 1) * 2; j += 2)
        {
            // First triangle
            unsigned int i1 = i * 2 * pointsCount + j;
            unsigned int i2 = i * 2 * pointsCount + j + 1;
            unsigned int i3 = i * 2 * pointsCount + j + 3;

            // indices
            indices[i * indicesCountInLane + c++] = i1;
            indices[i * indicesCountInLane + c++] = i2;
            indices[i * indicesCountInLane + c++] = i3;

            // normals
            glm::vec3 normal = glm::cross(vertices[i3].position - vertices[i2].position,
										  vertices[i1].position - vertices[i2].position);
			normal = glm::normalize(normal);

			vertices[i1].normal += normal;   vertices[i1].normal /= 2.0f;
			vertices[i2].normal += normal;   vertices[i2].normal /= 2.0f;
			vertices[i3].normal += normal;   vertices[i3].normal /= 2.0f;

			vertices[i1].normal = glm::normalize(vertices[i1].normal);
			vertices[i2].normal = glm::normalize(vertices[i2].normal);
			vertices[i3].normal = glm::normalize(vertices[i3].normal);

            // Second triangle
            i1 = i * 2 * pointsCount + j;
            i2 = i * 2 * pointsCount + j + 3;
            i3 = i * 2 * pointsCount + j + 2;

            // indices
            indices[i * indicesCountInLane + c++] = i1;
            indices[i * indicesCountInLane + c++] = i2;
            indices[i * indicesCountInLane + c++] = i3;

            // normals
            normal = glm::cross(vertices[i3].position - vertices[i2].position,
                                vertices[i1].position - vertices[i2].position);
			normal = glm::normalize(normal);

			vertices[i1].normal += normal;   vertices[i1].normal /= 2.0f;
			vertices[i2].normal += normal;   vertices[i2].normal /= 2.0f;
			vertices[i3].normal += normal;   vertices[i3].normal /= 2.0f;

			vertices[i1].normal = glm::normalize(vertices[i1].normal);
			vertices[i2].normal = glm::normalize(vertices[i2].normal);
			vertices[i3].normal = glm::normalize(vertices[i3].normal);
        }
    }


    Mesh* meshes = new Mesh[lanesCount];

	for (int i = 0; i < lanesCount; ++i)
    {
        meshes[i].material = roadLanes[i].material;
        meshes[i].firstVertex = i * indicesCountInLane;
        meshes[i].quantumOfVertice = indicesCountInLane;
    }


    Model* model = new Model(vertices, verticesCount, indices, indicesCount, meshes, lanesCount);

    return model;
}*/
