#include "Subscription.h"

#include "EventDispatcher.h"


Subscription::Subscription()
    : _dispatcher(nullptr), _eventType(0), _subscriptionId(0)
{

}


Subscription::Subscription(EventDispatcher* dispatcher, EventTypeId eventType, SubscriptionId subscriptionId)
    : _dispatcher(dispatcher), _eventType(eventType), _subscriptionId(subscriptionId)
{

}


Subscription::~Subscription()
{
    unsubscribe();
}


Subscription::Subscription(Subscription&& other) noexcept
    : _dispatcher(other._dispatcher),
    _eventType(other._eventType),
    _subscriptionId(other._subscriptionId)
{
    other._dispatcher = nullptr;
    other._eventType = 0;
    other._subscriptionId = 0;
}


Subscription& Subscription::operator=(Subscription&& other) noexcept
{
    if (this == &other)
        return *this;

    unsubscribe();

    _dispatcher = other._dispatcher;
    _eventType = other._eventType;
    _subscriptionId = other._subscriptionId;

    other._dispatcher = nullptr;
    other._eventType = 0;
    other._subscriptionId = 0;

    return *this;
}


void Subscription::unsubscribe()
{
    if (_dispatcher != nullptr)
    {
        _dispatcher->unsubscribe(_eventType, _subscriptionId);
    }

    _dispatcher = nullptr;
    _eventType = 0;
    _subscriptionId = 0;
}
