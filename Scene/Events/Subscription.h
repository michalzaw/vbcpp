#ifndef SUBSCRIPTION_H_INCLUDED
#define SUBSCRIPTION_H_INCLUDED


#include "EventTypes.h"


class EventDispatcher;


using SubscriptionId = std::size_t;


class Subscription
{
    private:
        EventDispatcher* _dispatcher;
        EventTypeId _eventType;
        SubscriptionId _subscriptionId;

    public:
        Subscription();
        Subscription(EventDispatcher* dispatcher, EventTypeId eventType, SubscriptionId subscriptionId);
        ~Subscription();

        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;

        Subscription(Subscription&& other) noexcept;
        Subscription& operator=(Subscription&& other) noexcept;

        void unsubscribe();

};


#endif // SUBSCRIPTION_H_INCLUDED
