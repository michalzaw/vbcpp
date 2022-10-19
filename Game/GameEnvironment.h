#ifndef GAMEENVIRONMENT_H_INCLUDED
#define GAMEENVIRONMENT_H_INCLUDED


#include <unordered_map>


namespace GameEnvironment
{
	namespace PredefinedVariables
	{
		const std::string BUS_SPEED = "busSpeed";
		const std::string BUS_ENGINE_RPM = "busEngineRpm";
	}

	class Variables
	{
		public:
			static std::unordered_map<std::string, float> floatVaribles;
			static std::unordered_map<std::string, std::string> stringVaribles;

	};
}


#endif // GAMEENVIRONMENT_H_INCLUDED
