#ifndef BUSCONFIGURATIONSLOADER_H_INCLUDED
#define BUSCONFIGURATIONSLOADER_H_INCLUDED


#include <string>
#include <unordered_map>


namespace BusConfigurationsLoader
{
	void loadBusPredefinedConfigurationByName(const std::string& busName, const std::string& configurationName, std::unordered_map<std::string, std::string>& outVariables);
}


#endif // BUSCONFIGURATIONSLOADER_H_INCLUDED
