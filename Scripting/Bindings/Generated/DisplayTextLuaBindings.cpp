// Generated file
#include "DisplayTextLuaBindings.h"

#include "../../../Graphics/DisplayText.h"


namespace DisplayTextLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<DisplayText>("DisplayText",




            "head", &DisplayText::head,
            "line1", &DisplayText::line1,
            "line2", &DisplayText::line2,
            "type", &DisplayText::type
        );
    }
}
