// Generated file
#include "GUIObjectLuaBindings.h"

#include "../../../GUI/GUIObject.h"


namespace GUIObjectLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<GUIObject>("GUIObject",

            "setRotation", &GUIObject::setRotation,
            "rotate", &GUIObject::rotate,
            "getPosition", &GUIObject::getPosition,
            "getScale", &GUIObject::getScale,
            "getRotation", &GUIObject::getRotation,
            "setIsActive", &GUIObject::setIsActive,
            "isActive", &GUIObject::isActive,
            "setColor", &GUIObject::setColor,
            "getColor", &GUIObject::getColor,


            "setPosition", sol::overload(
                sol::resolve<void(glm::vec3)>(&GUIObject::setPosition),
                sol::resolve<void(glm::vec2)>(&GUIObject::setPosition),
                sol::resolve<void(float, float, float)>(&GUIObject::setPosition)
            ),
            "setScale", sol::overload(
                sol::resolve<void(glm::vec2)>(&GUIObject::setScale),
                sol::resolve<void(float, float)>(&GUIObject::setScale)
            ),
            "move", sol::overload(
                sol::resolve<void(glm::vec3)>(&GUIObject::move),
                sol::resolve<void(glm::vec2)>(&GUIObject::move),
                sol::resolve<void(float, float, float)>(&GUIObject::move)
            ),
            "scale", sol::overload(
                sol::resolve<void(glm::vec2)>(&GUIObject::scale),
                sol::resolve<void(float, float)>(&GUIObject::scale)
            )

        );
    }
}
