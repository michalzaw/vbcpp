#ifndef MATHFUNCTIONSLUABINDINGS_H_INCLUDED
#define MATHFUNCTIONSLUABINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace MathFunctionsLuaBindings
{
	void bind(sol::state* lua);
}


#endif // MATHFUNCTIONSLUABINDINGS_H_INCLUDED
