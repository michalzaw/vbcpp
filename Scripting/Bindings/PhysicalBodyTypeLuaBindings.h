#ifndef PHYSICALBODYTYPELUABINDINGS_H_INCLUDED
#define PHYSICALBODYTYPELUABINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace PhysicalBodyTypeLuaBindings
{
	void bind(sol::state* lua);
}


#endif // PHYSICALBODYTYPELUABINDINGS_H_INCLUDED
