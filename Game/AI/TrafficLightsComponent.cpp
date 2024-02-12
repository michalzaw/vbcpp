#include "TrafficLightsComponent.h"

#include "../../Graphics/RenderObject.h"

#include "../../Scene/SceneObject.h"


TrafficLightsComponent::TrafficLightsComponent(const std::string& redLightNodeName, const std::string& yellowLightNodeName, const std::string& greenLighNodeName)
	: Component(CT_TRAFFIC_LIGHTS),
	_redLightNodeName(redLightNodeName), _yellowLightNodeName(yellowLightNodeName), _greenLightNodeName(greenLighNodeName),
	_redLightMaterial(nullptr), _yellowLightMaterial(nullptr), _greenLightMaterial(nullptr),
	_redLightColor(1.0f, 0.0f, 0.0f, 1.0f), _yellowLightColor(1.0f, 1.0f, 0.0f, 1.0f), _greenLightColor(0.0f, 1.0f, 0.0f, 1.0f),
	_timer(0.0f), _currentLight(0)
{

}


TrafficLightsComponent::~TrafficLightsComponent()
{

}


void TrafficLightsComponent::onAttachedToScenObject()
{
	RenderObject* renderObject = dynamic_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
	if (renderObject != nullptr)
	{
		//for (int i = 0; i < renderObject->getModelRootNode()->getChildrenCount(); ++i)
		//{
			ModelNode* modelNode = renderObject->getModelRootNode();
			for (int j = 0; j < modelNode->getMeshesCount(); ++j)
			{
				ModelNodeMesh* mesh = modelNode->getMesh(j);
				if (mesh->material->name == _redLightNodeName)
				{
					_redLightMaterial = mesh->material;
					//_redLightColor = _redLightMaterial->emissiveColor;
					_redLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				}
				if (mesh->material->name == _yellowLightNodeName)
				{
					_yellowLightMaterial = mesh->material;
					//_yellowLightColor = _yellowLightMaterial->emissiveColor;
					_yellowLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				}
				if (mesh->material->name == _greenLightNodeName)
				{
					_greenLightMaterial = mesh->material;
					//_greenLightColor = _greenLightMaterial->emissiveColor;
					_greenLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		//}

		// Saver materia³ów nie zapisuje emmisive color

		/*ModelNode* redLightModelNode = renderObject->getModelNodeByName(_redLightNodeName);
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
		}*/
	}
}


void TrafficLightsComponent::update(float deltaTime)
{
	/*_timer += deltaTime;

	if (_timer > 1.0f)
	{
		_timer -= 1.0f;

		if (_currentLight == 0)
		{
			_currentLight = 1;
			_yellowLightMaterial->emissiveColor = _yellowLightColor;
		}
		else if (_currentLight == 1)
		{
			_currentLight = 0;
			_yellowLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}*/

	/*_timer += deltaTime;

	if (_timer > 3.0f)
	{
		_timer -= 3.0f;

		_currentLight = (_currentLight + 1) % 3;

		_greenLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		_yellowLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		_redLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		if (_currentLight == 0)
		{
			_redLightMaterial->emissiveColor = _redLightColor;
		}
		else if (_currentLight == 1)
		{
			_yellowLightMaterial->emissiveColor = _yellowLightColor;
		}
		else if (_currentLight == 2)
		{
			_greenLightMaterial->emissiveColor = _greenLightColor;
		}
	}*/

	_timer -= deltaTime;
	if (_timer <= 0.0f)
	{
		_currentLight = (_currentLight + 1) % 4;
		LOG_DEBUG(LOG_VARIABLE(_currentLight));

		_greenLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		_yellowLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		_redLightMaterial->emissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		if (_currentLight == 0)
		{
			_redLightMaterial->emissiveColor = _redLightColor;
			
			_timer = 10.0f;
		}
		else if (_currentLight == 1)
		{
			_redLightMaterial->emissiveColor = _redLightColor;
			_yellowLightMaterial->emissiveColor = _yellowLightColor;

			_timer = 1.0f;
		}
		else if (_currentLight == 2)
		{
			_greenLightMaterial->emissiveColor = _greenLightColor;

			_timer = 10.0f;
		}
		else if (_currentLight == 3)
		{
			_yellowLightMaterial->emissiveColor = _yellowLightColor;

			_timer = 3.0f;
		}
	}
}
