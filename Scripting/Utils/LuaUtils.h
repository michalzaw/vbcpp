#ifndef LUAUTILS_H_INCLUDED
#define LUAUTILS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


#define LUA_MAP_ENUM(enumValue) #enumValue, enumValue


namespace LuaUtils
{
	const std::string LUA_LOGGER_PREFIX = "[LUA] ";

	void createConstantTable(sol::state* lua, const std::string& constName, const sol::table& table);
}


#endif // LUAUTILS_H_INCLUDED
