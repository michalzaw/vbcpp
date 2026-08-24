// Generated file
#include "LabelLuaBindings.h"

#include "../../../GUI/Label.h"


namespace LabelLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Label>("Label",
            sol::base_classes, sol::bases<GUIObject>(),

            "setFont", &Label::setFont,
            "setText", &Label::setText,
            "setMaxWidth", &Label::setMaxWidth,
            "setMaxHeight", &Label::setMaxHeight,
            "getFont", &Label::getFont,
            "getText", &Label::getText,
            "getWidth", &Label::getWidth,
            "getHeight", &Label::getHeight,
            "getMaxWidth", &Label::getMaxWidth,
            "getMaxHeight", &Label::getMaxHeight



        );
    }
}
