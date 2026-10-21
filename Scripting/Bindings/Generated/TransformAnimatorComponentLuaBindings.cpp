// Generated file
#include "TransformAnimatorComponentLuaBindings.h"

#include "../../../Game/TransformAnimatorComponent.h"


namespace TransformAnimatorComponentLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<TransformAnimatorComponent>("TransformAnimatorComponent",
            sol::base_classes, sol::bases<Component>(),



            "move", sol::overload(
                sol::resolve<void(const glm::vec3&, float)>(&TransformAnimatorComponent::move),
                sol::resolve<void(float, float, float, float)>(&TransformAnimatorComponent::move)
            ),
            "rotate", sol::overload(
                sol::resolve<void(const glm::vec3&, float)>(&TransformAnimatorComponent::rotate),
                sol::resolve<void(float, float, float, float)>(&TransformAnimatorComponent::rotate)
            ),
            "rotateLocal", sol::overload(
                sol::resolve<void(const glm::vec3&, float)>(&TransformAnimatorComponent::rotateLocal),
                sol::resolve<void(float, float, float, float)>(&TransformAnimatorComponent::rotateLocal)
            ),
            "scale", sol::overload(
                sol::resolve<void(const glm::vec3&, float)>(&TransformAnimatorComponent::scale),
                sol::resolve<void(float, float, float, float)>(&TransformAnimatorComponent::scale)
            )

        );
    }
}
