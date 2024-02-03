#ifndef AIAGENTVEHICLE_H_INCLUDED
#define AIAGENTVEHICLE_H_INCLUDED


#include "../../Scene/Component.h"


class PhysicalBodyRaycastVehicle;
class PathComponent;
class BezierCurve;


class AIAgentVehicle final : public Component
{
	private:
		PhysicalBodyRaycastVehicle* _vehicle;

		PathComponent* _currentPath;
		BezierCurve* _currentPathBezierCurve;

		bool _isStop;
		float _timeToStart;
		float _brakeForce;

		float getSteeringValue();
		float calculateDestinationRotation(const glm::vec3& destinationPoint);

		void lookForward();

	protected:
		void onAttachedToScenObject() override;

	public:
		AIAgentVehicle();

		void setCurrentPath(PathComponent* path);
		inline PathComponent* getCurrentPath() { return _currentPath; }

		void stop(float distance);

		void update(float deltaTime) override;
};


#endif // AIAGENTVEHICLE_H_INCLUDED