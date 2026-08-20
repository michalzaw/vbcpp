// Generated file
#include "DisplayComponentLuaBindings.h"

#include "../../../Graphics/DisplayComponent.h"


namespace DisplayComponentLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<DisplayComponent>("DisplayComponent",
            sol::base_classes, sol::bases<Component>(),

            "setText", &DisplayComponent::setText,
            "getText", &DisplayComponent::getText



        );
    }
}
