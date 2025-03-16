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

		PathComponent* _nextPath;

		glm::vec3 _frontSensorPosition;

		bool _isStop;
		float _timeToStart;
		float _brakeForce;

		float _targetSpeed;

		int _nextPointIndex;

		float getSteeringValue();
		float calculateDestinationRotation(const glm::vec3& destinationPoint);

		void lookForward();

		bool isNearToPathEnd(float expectedDistance);
		void chooseNextPath();

	protected:
		void onAttachedToScenObject() override;

	public:
		AIAgentVehicle();

		void setCurrentPath(PathComponent* path);
		inline PathComponent* getCurrentPath() { return _currentPath; }

		inline void setFrontSensorPosition(const glm::vec3& position) { _frontSensorPosition = position;}
		inline const glm::vec3 getFrontSensorPosition() { return _frontSensorPosition; }

		void stop(float distance);
		void stopOnPoint(const glm::vec3& position);
		void brake(float brakeForce);

		void update(float deltaTime) override;
};


#endif // AIAGENTVEHICLE_H_INCLUDED