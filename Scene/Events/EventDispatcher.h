#ifndef EVENTDISPATCHER_H_INCLUDED
#define EVENTDISPATCHER_H_INCLUDED


#include <functional>
#include <unordered_map>

#include "Subscription.h"



//using EventCallback = std::function<void(const void*)>;


struct CallbackEntry
{
    SubscriptionId id;
    std::function<void(const void*)> callback;
};


class EventDispatcher
{
    private:
        std::unordered_map<EventTypeId, std::vector<CallbackEntry>> _subscribers;

        SubscriptionId _nextSubscriptionId;

    public:
        EventDispatcher();

        template<typename Event, typename CallbackType>
        Subscription subscribe(CallbackType&& callback);

        template<typename Event, typename Class>
        Subscription subscribe(void (Class::*method)(const Event&), Class* object);

        void unsubscribe(EventTypeId eventTypeId, SubscriptionId subscriptionId);

        template<typename Event>
        void dispatch(const Event& event);

};


template<typename Event, typename CallbackType>
Subscription EventDispatcher::subscribe(CallbackType&& callback)
{
    EventTypeId typeId = getEventTypeId<Event>();

    std::vector<CallbackEntry>& callbacks = _subscribers[typeId];

    SubscriptionId subscriptionId = _nextSubscriptionId++;

    callbacks.push_back({
        subscriptionId,
        [callback = std::forward<CallbackType>(callback)]
        (const void* data)
        {
            callback(*static_cast<const Event*>(data));
        }
        });

    return Subscription(this, typeId, subscriptionId);
}


template<typename Event, typename Class>
Subscription EventDispatcher::subscribe(void (Class::*method)(const Event&), Class* object)
{
    return subscribe<Event>([object, method](const Event& event)
        {
            (object->*method)(event);
        });
}


template<typename Event>
void EventDispatcher::dispatch(const Event& event)
{
    EventTypeId typeId = getEventTypeId<Event>();

    auto it = _subscribers.find(typeId);

    if (it == _subscribers.end())
    {
        return;
    }

    for (CallbackEntry& callbackEntry : it->second)
    {
        callbackEntry.callback(&event);
    }
}


#endif
