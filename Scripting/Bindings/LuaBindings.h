#ifndef LUADBINDINGS_H_INCLUDED
#define LUADBINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace LuaBindings
{
	void bind(sol::state* lua);
}


#endif // LUADBINDINGS_H_INCLUDED
