#ifndef BUSPREVIEWLOADER_H_INCLUDED
#define BUSPREVIEWLOADER_H_INCLUDED


#include "BusConfigurationsLoader.h"
#include "BusLoader.h"
#include "BusRepaintDescription.h"


struct VariableValue final
{
	std::string value;
	std::string displayValue;
};


struct GameVariable final
{
	std::string name;
	std::vector<VariableValue> values;
	std::string displayName;
	std::string description;
	std::string defaultValue;
};


struct BusPreview final
{
	std::string busName;

	Bus* bus;

	std::vector<GameVariable> availableVariables;

	std::unordered_map<std::string, std::unordered_map<std::string, std::vector<SceneObject*>>> variableDependentObjects; // map[varName][varValue] = std::vector<SceneObject*>

	std::vector<PredefinedConfiguration> predefinedConfigurations;

	std::vector<BusRepaintDescription> availableRepaints;

	void setConfiguration(std::unordered_map<std::string, std::string>& variables)
	{
		for (const auto& variable : variableDependentObjects)
		{
			const std::string& variableName = variable.first;
			for (const auto& variableValuePair : variable.second)
			{
				const std::string& variableValue = variableValuePair.first;

				bool isActive = variables[variableName] == variableValue;

				for (SceneObject* sceneObject : variableValuePair.second)
				{
					sceneObject->setIsActive(isActive);
				}
			}
		}
	}
};


class BusPreviewLoader final : public BusLoader
{
	private:
		BusPreview* _busPreview;

		void loadAvailableVariables(XMLElement* busElement) override;

		void loadModuleConditionalElements(XMLElement* moduleElement, XMLElement* busElement, BusRayCastModule& busModule) override;

		void loadWheels(XMLElement* moduleElement, BusRayCastModule& busModule) override;
		void loadDoors(XMLElement* moduleElement, BusRayCastModule& busModule) override;

	public:
		BusPreviewLoader(SceneManager* smgr, GraphicsManager* gmgr, PhysicsManager* pmgr, SoundManager* sndMgr);

		BusPreview* loadBusPreview(const std::string& busName);
};


#endif // BUSPREVIEWLOADER_H_INCLUDED
