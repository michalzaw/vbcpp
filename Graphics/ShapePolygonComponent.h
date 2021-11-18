#ifndef SHAPEPOLYGONCOMPONENT_H_INCLUDED
#define SHAPEPOLYGONCOMPONENT_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class RStaticModel;
struct StaticModelMesh;


class ShapePolygonComponent final : public Component
{
	private:
		std::vector<glm::vec3> _points;

		RStaticModel* _generatedModel;

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
