#ifndef BUSCONFIGURATIONSLOADER_H_INCLUDED
#define BUSCONFIGURATIONSLOADER_H_INCLUDED


#include <string>
#include <unordered_map>


struct PredefinedConfiguration final
{
	std::string configurationName;
	std::string displayName;
	std::unordered_map<std::string, std::string> configuration; //map[varName] = varValue
};


namespace BusConfigurationsLoader
{
	void loadBusPredefinedConfigurationByName(const std::string& busName, const std::string& configurationName, std::unordered_map<std::string, std::string>& outVariables);

	void loadAllBusPredefinedConfigurations(const std::string& busName, std::vector<PredefinedConfiguration>& outPredefinedConfigurations,
											const std::unordered_map<std::string, std::string>& variablesDefaultValues = {});
}


#endif // BUSCONFIGURATIONSLOADER_H_INCLUDED
