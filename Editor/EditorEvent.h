#ifndef EDITOREVENT_H_INCLUDED
#define EDITOREVENT_H_INCLUDED


#include <string>


enum EditorEventType
{
    EET_NEW_CLICK,
    EET_OPEN_CLICK
};


struct EditorEvent
{
    EditorEventType type;
    std::string params;

    EditorEvent(EditorEventType t, std::string p = "")
        : type(t), params(p)
    {}
};


#endif // EDITOREVENT_H_INCLUDED
