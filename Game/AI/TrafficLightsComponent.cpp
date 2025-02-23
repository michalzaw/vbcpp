#include "TrafficLightsComponent.h"

#include "AIAgentVehicle.h"

#include "../../Graphics/RenderObject.h"

#include "../../Scene/SceneManager.h"
#include "../../Scene/SceneObject.h"


TrafficLightsState getTrafficLightsStateFromString(const std::string& name)
{
	for (int i = 0; i < TLS_STATE_COUNT; ++i)
	{
		if (trafficLightsStateStrings[i] == name)
			return static_cast<TrafficLightsState>(i);
	}

	return TLS_STATE_COUNT;
}


TrafficLightsComponent::TrafficLightsComponent(const std::string& redLightNodeName, const std::string& yellowLightNodeName, const std::string& greenLighNodeName,
											   const glm::vec3& triggerBoxPosition, const glm::vec3& triggerBoxRotation, const glm::vec3& triggerBoxSize,
											   const glm::vec3& stopPointPosition, const TrafficLightsState initState)
	: Component(CT_TRAFFIC_LIGHTS),
	_redLightNodeName(redLightNodeName), _yellowLightNodeName(yellowLightNodeName), _greenLightNodeName(greenLighNodeName),
	_redLightMaterial(nullptr), _yellowLightMaterial(nullptr), _greenLightMaterial(nullptr),
	_redLightColor(1.0f, 0.0f, 0.0f, 1.0f), _yellowLightColor(1.0f, 1.0f, 0.0f, 1.0f), _greenLightColor(0.0f, 1.0f, 0.0f, 1.0f),
	_initState(initState),
	_timer(0.0f), _currentState(initState),
	_triggerBox(nullptr), _triggerBoxPositionInitialValue(triggerBoxPosition), _triggerBoxRotationInitialValue(triggerBoxRotation), _triggerBoxSizeInitialValue(triggerBoxSize),
	_stopPoint(stopPointPosition)
{

}


TrafficLightsComponent::~TrafficLightsComponent()
{
	if (_triggerBox != nullptr)
	{
		getSceneObject()->getSceneManager()->removeSceneObject(_triggerBox->getSceneObject());
	}
}


void TrafficLightsComponent::onAttachedToScenObject()
{
	RenderObject* renderObject = dynamic_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
	if (renderObject != nullptr)
	{
		ModelNode* redLightModelNode = renderObject->getModelNodeByName(_redLightNodeName);
		if (redLightModelNode)
		{
			_redLightMaterial = redLightModelNode->getMesh(0)->material;
			_redLightColor = _redLightMaterial->emissiveColor;
			_redLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		ModelNode* yellowLightModelNode = renderObject->getModelNodeByName(_yellowLightNodeName);
		if (yellowLightModelNode)
		{
			_yellowLightMaterial = yellowLightModelNode->getMesh(0)->material;
			_yellowLightColor = _yellowLightMaterial->emissiveColor;
			_yellowLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		ModelNode* greenLightModelNode = renderObject->getModelNodeByName(_greenLightNodeName);
		if (greenLightModelNode)
		{
			_greenLightMaterial = greenLightModelNode->getMesh(0)->material;
			_greenLightColor = _greenLightMaterial->emissiveColor;
			_greenLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}


	SceneObject* triggerBoxObject = getSceneObject()->getSceneManager()->addSceneObject(getSceneObject()->getName() + "-triggerBox");
	triggerBoxObject->setFlags(SOF_NOT_SERIALIZABLE);

	_triggerBox = getSceneObject()->getSceneManager()->getPhysicsManager()->createPhysicalBodyGhost(btVector3(_triggerBoxSizeInitialValue.x, _triggerBoxSizeInitialValue.y, _triggerBoxSizeInitialValue.z));
	triggerBoxObject->addComponent(_triggerBox);
	triggerBoxObject->setPosition(_triggerBoxPositionInitialValue);
	triggerBoxObject->setRotation(_triggerBoxRotationInitialValue);

	getSceneObject()->addChild(triggerBoxObject);
	
	setCurrentState(_initState);
}


void TrafficLightsComponent::setInitState(TrafficLightsState state)
{
	_initState = state;

	setCurrentState(state, true);
}


void TrafficLightsComponent::setCurrentState(TrafficLightsState state, bool initialization/* = false*/)
{
	_currentState = state;

	_greenLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	_yellowLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	_redLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	if (_currentState == TLS_RED)
	{
		_redLightMaterial->emissiveColor = _redLightColor;

		_timer = 16.0f;
		if (initialization)
		{
			_timer -= 2.0f;
		}
	}
	else if (_currentState == TLS_RED_AND_YELLOW)
	{
		_redLightMaterial->emissiveColor = _redLightColor;
		_yellowLightMaterial->emissiveColor = _yellowLightColor;

		_timer = 1.0f;
	}
	else if (_currentState == TLS_GREEN)
	{
		_greenLightMaterial->emissiveColor = _greenLightColor;

		_timer = 10.0f;
	}
	else if (_currentState == TLS_YELLOW)
	{
		_yellowLightMaterial->emissiveColor = _yellowLightColor;

		_timer = 3.0f;
	}
}


void TrafficLightsComponent::changedTransform()
{

}


void TrafficLightsComponent::update(float deltaTime)
{
	_timer -= deltaTime;

	if (_timer <= 0.0f)
	{
		setCurrentState((TrafficLightsState) ((_currentState + 1) % TLS_STATE_COUNT));
	}

	if (_triggerBox != nullptr)
	{
		for (PhysicalBody* body : _triggerBox->getObjectsCollidesWith())
		{
			AIAgentVehicle* aiAgentVehicle = body->getSceneObject()->getComponentWithCasting<AIAgentVehicle>(CT_AI_AGENT_VEHICLE);
			if (aiAgentVehicle != nullptr)
			{
				if (_currentState == TLS_RED || _currentState == TLS_YELLOW)
				{
					glm::vec3 stopPosition = getSceneObject()->transformLocalPointToGlobal(_stopPoint);
					aiAgentVehicle->stopOnPoint(stopPosition);
				}
			}
		}
	}
}
