#ifndef TRAFFICLIGHTSCOMPONENT_HPP_INCLUDED
#define TRAFFICLIGHTSCOMPONENT_HPP_INCLUDED


#include <string>

#include <glm/glm.hpp>

#include "../../Scene/Component.h"


class Material;


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

		float _timer;
		int _currentLight;

	protected:
		void onAttachedToScenObject() override;

	public:
		TrafficLightsComponent(const std::string& redLightNodeName, const std::string& yellowLightNodeName, const std::string& greenLighNodeName);
		~TrafficLightsComponent();

		void update(float deltaTime) override;

};


#endif // TRAFFICLIGHTSCOMPONENT_HPP_INCLUDED
