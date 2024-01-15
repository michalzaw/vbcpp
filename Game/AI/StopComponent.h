#ifndef STOPCOMPONENT_H_INCLUDED
#define STOPCOMPONENT_H_INCLUDED


#include "../../Scene/Component.h"


class PhysicalBody;


class StopComponent : public Component
{
	protected:
		PhysicalBody* _physicalBody;

		void onAttachedToScenObject() override;

	public:
		StopComponent();

		void update(float deltaTime) override;
};


#endif // STOPCOMPONENT_H_INCLUDED
