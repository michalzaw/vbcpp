// Generated file
#include "RTextureLuaBindings.h"

#include "../../../Utils/RTexture.h"


namespace RTextureLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<RTexture>("RTexture",
            sol::base_classes, sol::bases<Resource>(),

            "getSize", &RTexture::getSize



        );
    }
}
