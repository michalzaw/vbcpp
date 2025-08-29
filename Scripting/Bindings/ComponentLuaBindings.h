#ifndef COMPONENTLUADBINDINGS_H_INCLUDED
#define COMPONENTLUADBINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace ComponentLuaBindings
{
	void bind(sol::state* lua);
}


#endif // COMPONENTLUADBINDINGS_H_INCLUDED
