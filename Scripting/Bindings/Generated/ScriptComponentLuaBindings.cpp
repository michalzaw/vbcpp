// Generated file
#include "ScriptComponentLuaBindings.h"

#include "../../../Scripting/ScriptComponent.h"


namespace ScriptComponentLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<ScriptComponent>("ScriptComponent",
            sol::base_classes, sol::bases<Component>(),

            "isInitialized", &ScriptComponent::isInitialized



        );
    }
}
