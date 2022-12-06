#ifndef PATHCOMPONENT_H_INCLUDED
#define PATHCOMPONENT_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class BezierCurve;
class RoadObject;


enum PathDirection
{
	PD_FORWARD = 1,
	PD_BACKWARD = -1,
	PD_BOOTH = 0

};


class PathComponent final : public Component
{
	private:
		PathDirection _direction;

		BezierCurve* _bezierCurveComponent;
		RoadObject* _pathHelperComponent;

		std::vector<glm::vec3> _curvePoints;

		void onAttachedToScenObject() override;

	public:
		PathComponent(PathDirection direction);

		inline PathDirection getDirection() { return _direction; }
		inline const std::vector<glm::vec3>& getCurvePoints() { return _curvePoints; }

		inline void setPathDirection(PathDirection direction) { _direction = direction; }

		void recalculate();

		void update(float deltaTime) override;
};


#endif // PATHCOMPONENT_H_INCLUDED
