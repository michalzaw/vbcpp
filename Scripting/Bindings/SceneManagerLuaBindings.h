#ifndef SCENEMANAGERLUADBINDINGS_H_INCLUDED
#define SCENEMANAGERLUADBINDINGS_H_INCLUDED


#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


namespace SceneManagerLuaBindings
{
	void bind(sol::state* lua);
}


#endif // SCENEMANAGERLUADBINDINGS_H_INCLUDED
