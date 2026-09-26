#ifndef TRANSFORMCOMPLETEDEVENTT_H_INCLUDED
#define TRANSFORMCOMPLETEDEVENTT_H_INCLUDED


#include "../Scene/Events/EventTypes.h"

#include "../Scripting/Utils/LuaMacros.h"


enum class TransformCompletedEventType
{
	TRANSLATION,
	ROTATION,
	ROTATION_LOCAL,
	SCALE

};


VBCPP_EVENT
struct TransformCompletedEvent final
{
	LUAV TransformCompletedEventType type;

};


#endif // TRANSFORMCOMPLETEDEVENTT_H_INCLUDED
