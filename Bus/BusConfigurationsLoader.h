#ifndef BUSCONFIGURATIONSLOADER_H_INCLUDED
#define BUSCONFIGURATIONSLOADER_H_INCLUDED


#include <string>
#include <unordered_map>


namespace BusConfigurationsLoader
{
	void loadBusPredefinedConfigurationByName(const std::string& busName, const std::string& configurationName, std::unordered_map<std::string, std::string>& outVariables);

	void loadAllBusPredefinedConfigurations(const std::string& busName, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& outPredefinedConfigurations,
											const std::unordered_map<std::string, std::string>& variablesDefaultValues = {});
}


#endif // BUSCONFIGURATIONSLOADER_H_INCLUDED
