#include "LuaUtils.h"

#include "../../Utils/Logger.h"


namespace LuaUtils
{
	void createConstantTable(sol::state* lua, const std::string& constName, const sol::table& table)
	{
		sol::table constProxy = lua->create_table();

		sol::table metatable = lua->create_table();
		metatable[sol::meta_function::index] = table;
		metatable[sol::meta_function::new_index] = [&constName](sol::object value, sol::object key) {
			if (key.is<std::string>())
			{
				std::string keyName = key.as<std::string>();
				LOG_ERROR(LUA_LOGGER_PREFIX + "Cannot change value in const table for key: " + keyName);
			}
			else
			{
				LOG_ERROR(LUA_LOGGER_PREFIX + "Cannot change value in const table");
			}
		};

		constProxy[sol::metatable_key] = metatable;

		(*lua)[constName] = constProxy;
	}
}
