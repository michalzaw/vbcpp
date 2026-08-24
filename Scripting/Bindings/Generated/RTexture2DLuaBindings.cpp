// Generated file
#include "RTexture2DLuaBindings.h"

#include "../../../Utils/RTexture2D.h"


namespace RTexture2DLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<RTexture2D>("RTexture2D",
            sol::base_classes, sol::bases<RTexture>()




        );
    }
}
