#ifndef INPUTSYSTEMLUABINDINGS_H_INCLUDED
#define INPUTSYSTEMLUABINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace InputSystemLuaBindings
{
	void bind(sol::state* lua);
}


#endif // INPUTSYSTEMLUABINDINGS_H_INCLUDED
