#ifndef DISPLAYTEXTTYPELUABINDINGS_H_INCLUDED
#define DISPLAYTEXTTYPELUABINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace DisplayTextTypeLuaBindings
{
	void bind(sol::state* lua);
}


#endif // DISPLAYTEXTTYPELUABINDINGS_H_INCLUDED
