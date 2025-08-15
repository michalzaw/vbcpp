#ifndef LOGGERLUABINDINGS_H_INCLUDED
#define LOGGERLUABINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace LoggerLuaBindings
{
	void bind(sol::state* lua);
}


#endif // LOGGERLUABINDINGS_H_INCLUDED
