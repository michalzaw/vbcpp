#ifndef OBJECTCLICKEDEVENT_H_INCLUDED
#define OBJECTCLICKEDEVENT_H_INCLUDED


#include "EventTypes.h"


class ModelNode;


VBCPP_EVENT
struct ObjectClickedEvent
{
	ModelNode* modelNode;
};


#endif // OBJECTCLICKEDEVENT_H_INCLUDED
