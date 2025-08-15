#ifndef SCRIPTCOMPONENTLUADBINDINGS_H_INCLUDED
#define SCRIPTCOMPONENTLUADBINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace ScriptComponentLuaBindings
{
	void bind(sol::state* lua);
}


#endif // SCRIPTCOMPONENTLUADBINDINGS_H_INCLUDED
