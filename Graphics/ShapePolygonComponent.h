#ifndef SHAPEPOLYGONCOMPONENT_H_INCLUDED
#define SHAPEPOLYGONCOMPONENT_H_INCLUDED


#include <vector>

#include "../Scene/Component.h"


class ShapePolygonComponent final : public Component
{
	private:
		std::vector<glm::vec3> _points;

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
