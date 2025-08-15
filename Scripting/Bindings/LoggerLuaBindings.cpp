#include "LoggerLuaBindings.h"

#include "../../Utils/Logger.h"


namespace LoggerLuaBindings
{
	const std::string LUA_PREFIX = "[LUA] ";

	void bind(sol::state* lua)
	{
		(*lua)["log_debug"] = [](const std::string& message) { LOG_DEBUG(LUA_PREFIX + message); };
		(*lua)["log_info"] = [](const std::string& message) { LOG_INFO(LUA_PREFIX + message); };
		(*lua)["log_warning"] = [](const std::string& message) { LOG_WARNING(LUA_PREFIX + message); };
		(*lua)["log_error"] = [](const std::string& message) { LOG_ERROR(LUA_PREFIX + message); };
	}
}
