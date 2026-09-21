// Generated file
#include "ObjectLoadedEventLuaBindings.h"

#include "../../../Scene/Events/ObjectLoadedEvent.h"

#include "../../Scene/SceneObject.h"


namespace ObjectLoadedEventLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<ObjectLoadedEvent>("ObjectLoadedEvent",




            "loadedObject", &ObjectLoadedEvent::loadedObject
        );
    }
}
