// Generated file
#include "ObjectClickedEventLuaBindings.h"

#include "../../../Scene/Events/ObjectClickedEvent.h"

#include "../../../Graphics/ModelNode.h"


namespace ObjectClickedEventLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<ObjectClickedEvent>("ObjectClickedEvent",




            "modelNode", &ObjectClickedEvent::modelNode
        );
    }
}
