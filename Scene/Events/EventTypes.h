#ifndef EVENTTYPES_H_INCLUDED
#define EVENTTYPES_H_INCLUDED


#include <cstddef>


using EventTypeId = std::size_t;


inline EventTypeId nextEventTypeId()
{
    static EventTypeId id = 0;
    return id++;
}


template<typename T>
EventTypeId getEventTypeId()
{
    static const EventTypeId id = nextEventTypeId();
    return id;
}


#define VBCPP_EVENT


#endif // EVENTTYPES_H_INCLUDED
