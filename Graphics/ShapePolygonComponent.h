#ifndef SHAPEPOLYGONCOMPONENT_H_INCLUDED
#define SHAPEPOLYGONCOMPONENT_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "NVMeshMender/NVMeshMender.h"

#include "../Scene/Component.h"


class RStaticModel;
struct StaticModelMesh;
struct Vertex;


class ShapePolygonComponent final : public Component
{
	private:
		static const unsigned int DEFAULT_ROAD_BUFFER_SIZE = 1024 * 1024; // ~ 4600 vertices

		std::vector<glm::vec3> _points;

		RStaticModel* _generatedModel;

		void triangulateAndGenerateMeshIndices(std::vector<unsigned int>& outIndices);
		void calculateNormals(std::vector<MeshMender::Vertex>& vertices, std::vector<unsigned int>& indices);
		void runMeshMender(std::vector<MeshMender::Vertex>& vertices, std::vector<unsigned int>& indices);
		Vertex* mapVerticesToInternalVertexFormat(std::vector<MeshMender::Vertex>& vertices);
		glm::vec3* generateCollistionMesh(StaticModelMesh* meshes, unsigned int meshesCount, unsigned int totalIndicesCount);

	public:
		ShapePolygonComponent();
		ShapePolygonComponent(std::vector<glm::vec3>& points);
		virtual ~ShapePolygonComponent();

		std::vector<glm::vec3>& getPoints();
		void addPoint(const glm::vec3& point);
		void setPointPostion(int index, glm::vec3 newPosition);

		void buildAndCreateRenderObject();

};


#endif // SHAPEPOLYGONCOMPONENT_H_INCLUDED
