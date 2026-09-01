#include "EventDispatcher.h"

#include <iostream>


EventDispatcher::EventDispatcher()
	: _nextSubscriptionId(0)
{

}


void EventDispatcher::unsubscribe(EventTypeId eventTypeId, SubscriptionId subscriptionId)
{
    auto it = _subscribers.find(eventTypeId);

    if (it == _subscribers.end())
    {
        return;
    }

    for (std::vector<CallbackEntry>::iterator i = it->second.begin(); i != it->second.end(); ++i)
    {
        if (i->id == subscriptionId)
        {
            it->second.erase(i);
            break;
        }
    }
}
