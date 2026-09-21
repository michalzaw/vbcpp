#ifndef OBJECTCLICKEDEVENT_H_INCLUDED
#define OBJECTCLICKEDEVENT_H_INCLUDED


#include "EventTypes.h"

#include "../../Scripting/Utils/LuaMacros.h"


class ModelNode;


VBCPP_EVENT
struct ObjectClickedEvent
{
	LUAV ModelNode* modelNode;
};


#endif // OBJECTCLICKEDEVENT_H_INCLUDED
