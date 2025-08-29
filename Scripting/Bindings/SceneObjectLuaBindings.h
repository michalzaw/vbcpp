#ifndef SCENEOBJECTLUADBINDINGS_H_INCLUDED
#define SCENEOBJECTLUADBINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace SceneObjectLuaBindings
{
	void bind(sol::state* lua);
}


#endif // SCENEOBJECTLUADBINDINGS_H_INCLUDED
