#include "LoggerLuaBindings.h"

#include "../Utils/LuaUtils.h"

#include "../../Utils/Logger.h"


namespace LoggerLuaBindings
{
	void bind(sol::state* lua)
	{
		(*lua)["log_debug"] = [](const std::string& message) { LOG_DEBUG(LuaUtils::LUA_LOGGER_PREFIX + message); };
		(*lua)["log_info"] = [](const std::string& message) { LOG_INFO(LuaUtils::LUA_LOGGER_PREFIX + message); };
		(*lua)["log_warning"] = [](const std::string& message) { LOG_WARNING(LuaUtils::LUA_LOGGER_PREFIX + message); };
		(*lua)["log_error"] = [](const std::string& message) { LOG_ERROR(LuaUtils::LUA_LOGGER_PREFIX + message); };
	}
}
