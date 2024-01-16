#ifndef STOPCOMPONENT_H_INCLUDED
#define STOPCOMPONENT_H_INCLUDED


#include "../../Scene/Component.h"


class PhysicalBody;


class StopComponent : public Component
{
	private:
		PhysicalBody* _physicalBody;

		float _distanceToStop;

	protected:
		void onAttachedToScenObject() override;

	public:
		StopComponent();

		inline void setDistanceToStop(float distanceToStop) { _distanceToStop = distanceToStop; }
		inline float getDistanceToStop() { return _distanceToStop; }

		void update(float deltaTime) override;
};


#endif // STOPCOMPONENT_H_INCLUDED
