#ifndef GLMLUADBINDINGS_H_INCLUDED
#define GLMLUADBINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace GlmLuaBindings
{
	void bind(sol::state* lua);
}


#endif // GLMLUADBINDINGS_H_INCLUDED
