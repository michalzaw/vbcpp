#ifndef PATHCOMPONENT_H_INCLUDED
#define PATHCOMPONENT_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class PathComponent final : public Component
{
	private:
		std::vector<glm::vec3> _controlPoints;
		std::vector<glm::vec3> _curvePoints;

	public:
		PathComponent();

		std::vector<glm::vec3>& getControlPoints();
		std::vector<glm::vec3>& getCurvePoints();

		void recalculate();

		void update(float deltaTime) override;
};


#endif // PATHCOMPONENT_H_INCLUDED
