#ifndef AIAGENTPHYSICALVECHICLE_H_INCLUDED
#define AIAGENTPHYSICALVECHICLE_H_INCLUDED


#include "AIAgent.h"


class PhysicalBodyRaycastVehicle;


class AIAgentPhysicalVechicle final : public AIAgent
{
	VBCPP_COMPONENT(AIAgentPhysicalVechicle, CT_AI_AGENT)

	private:
		PhysicalBodyRaycastVehicle* _vechicle;

		glm::vec3 _centerPoint;

	protected:
		void onAttachedToScenObject() override;

	public:
		AIAgentPhysicalVechicle();

		void moveToStartPoint();

		void update(float deltaTime) override;
};


#endif // AIAGENTPHYSICALVECHICLE_H_INCLUDED
