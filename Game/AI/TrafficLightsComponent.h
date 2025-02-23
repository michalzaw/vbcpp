#ifndef TRAFFICLIGHTSCOMPONENT_HPP_INCLUDED
#define TRAFFICLIGHTSCOMPONENT_HPP_INCLUDED


#include <string>

#include <glm/glm.hpp>

#include "../../Scene/Component.h"


class Material;
class PhysicalBodyGhost;


enum TrafficLightsState
{
	TLS_RED,
	TLS_RED_AND_YELLOW,
	TLS_GREEN,
	TLS_YELLOW,

	TLS_STATE_COUNT
};


const std::string trafficLightsStateStrings[] = { "red", "red_and_yellow", "green", "yellow" };
TrafficLightsState getTrafficLightsStateFromString(const std::string& name);


class TrafficLightsComponent : public Component
{
	private:
		std::string _redLightNodeName;
		std::string _yellowLightNodeName;
		std::string _greenLightNodeName;

		Material* _redLightMaterial;
		Material* _yellowLightMaterial;
		Material* _greenLightMaterial;

		glm::vec4 _redLightColor;
		glm::vec4 _yellowLightColor;
		glm::vec4 _greenLightColor;

		TrafficLightsState _initState;

		float _timer;
		TrafficLightsState _currentState;

		PhysicalBodyGhost* _triggerBox;
		glm::vec3 _triggerBoxPositionInitialValue;
		glm::vec3 _triggerBoxRotationInitialValue;
		glm::vec3 _triggerBoxSizeInitialValue;

		glm::vec3 _stopPoint;

	protected:
		void onAttachedToScenObject() override;

	public:
		TrafficLightsComponent(const std::string& redLightNodeName, const std::string& yellowLightNodeName, const std::string& greenLighNodeName,
							   const glm::vec3& triggerBoxPosition, const glm::vec3& triggerBoxRotation, const glm::vec3& triggerBoxSize,
							   const glm::vec3& stopPointPosition, const TrafficLightsState initState);
		~TrafficLightsComponent();

		inline TrafficLightsState getInitState() { return _initState; }
		void setInitState(TrafficLightsState state);

		inline TrafficLightsState getCurrentState() { return _currentState; }
		void setCurrentState(TrafficLightsState state, bool initialization = false);

		inline PhysicalBodyGhost* getTriggerBox() { return _triggerBox; }

		inline const glm::vec3& getStopPointPosition() { return _stopPoint; }
		inline void setStopPointPosition(const glm::vec3& stopPointPosition) { _stopPoint = stopPointPosition; }

		void changedTransform() override;
		void update(float deltaTime) override;

};


#endif // TRAFFICLIGHTSCOMPONENT_HPP_INCLUDED
