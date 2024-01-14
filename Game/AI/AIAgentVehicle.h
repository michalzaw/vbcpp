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

		float getSteeringValue();
		float calculateDestinationRotation(const glm::vec3& destinationPoint);

	protected:
		void onAttachedToScenObject() override;

	public:
		AIAgentVehicle();

		void setCurrentPath(PathComponent* path);
		inline PathComponent* getCurrentPath() { return _currentPath; }

		void update(float deltaTime) override;
};


#endif // AIAGENTVEHICLE_H_INCLUDED