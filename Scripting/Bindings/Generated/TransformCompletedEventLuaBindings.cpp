// Generated file
#include "TransformCompletedEventLuaBindings.h"

#include "../../../Game/TransformCompletedEvent.h"


namespace TransformCompletedEventLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<TransformCompletedEvent>("TransformCompletedEvent",




            "type", &TransformCompletedEvent::type
        );
    }
}
