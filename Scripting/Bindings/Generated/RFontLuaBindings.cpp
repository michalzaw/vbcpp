// Generated file
#include "RFontLuaBindings.h"

#include "../../../Utils/RFont.h"


namespace RFontLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<RFont>("RFont"




        );
    }
}
