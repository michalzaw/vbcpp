#ifndef {{ classNameUpperCase }}LUABINDINGS_H_INCLUDED
#define {{ classNameUpperCase }}LUABINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace {{ className }}LuaBindings
{
	void bind(sol::state* lua);
}


#endif // {{ classNameUpperCase }}LUABINDINGS_H_INCLUDED
