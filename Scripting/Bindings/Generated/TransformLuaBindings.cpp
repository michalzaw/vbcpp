// Generated file
#include "TransformLuaBindings.h"

#include "../../../Graphics/Transform.h"


namespace TransformLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Transform>("Transform",

            "getPosition", &Transform::getPosition,
            "getRotation", &Transform::getRotation,
            "getScale", &Transform::getScale,

            "setRotationQuaternion", sol::resolve<void(float, float, float, float)>(&Transform::setRotationQuaternion),

            "setPosition", sol::overload(
                sol::resolve<void(glm::vec3)>(&Transform::setPosition),
                sol::resolve<void(float, float, float)>(&Transform::setPosition)
            ),
            "setRotation", sol::overload(
                sol::resolve<void(glm::vec3)>(&Transform::setRotation),
                sol::resolve<void(float, float, float)>(&Transform::setRotation)
            ),
            "setScale", sol::overload(
                sol::resolve<void(glm::vec3)>(&Transform::setScale),
                sol::resolve<void(float, float, float)>(&Transform::setScale),
                sol::resolve<void(float)>(&Transform::setScale)
            ),
            "move", sol::overload(
                sol::resolve<void(glm::vec3)>(&Transform::move),
                sol::resolve<void(float, float, float)>(&Transform::move)
            ),
            "rotate", sol::overload(
                sol::resolve<void(glm::vec3)>(&Transform::rotate),
                sol::resolve<void(float, float, float)>(&Transform::rotate)
            ),
            "scale", sol::overload(
                sol::resolve<void(glm::vec3)>(&Transform::scale),
                sol::resolve<void(float, float, float)>(&Transform::scale)
            )

        );
    }
}
