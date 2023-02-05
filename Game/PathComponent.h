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


class PathComponent;


struct ConnectedPath final
{
	PathComponent* path;
	int index;
};


class PathComponent final : public Component
{
	private:
		PathDirection _direction;

		BezierCurve* _bezierCurveComponent;
		RoadObject* _pathHelperComponent;

		std::vector<glm::vec3> _curvePoints;

		std::vector<ConnectedPath> _nextPaths;
		std::vector<ConnectedPath> _previousPaths;

		void onAttachedToScenObject() override;

	public:
		PathComponent(PathDirection direction);

		inline PathDirection getDirection() { return _direction; }
		inline const std::vector<glm::vec3>& getCurvePoints() { return _curvePoints; }
		inline const std::vector<ConnectedPath>& getNextPaths() { return _nextPaths; }
		inline const std::vector<ConnectedPath>& getPreviousPaths() { return _previousPaths; }

		inline void setPathDirection(PathDirection direction) { _direction = direction; }

		void setConnection(int index, PathComponent* otherPath, int indexInOtherPath);

		void recalculate();

		void update(float deltaTime) override;
};


#endif // PATHCOMPONENT_H_INCLUDED
