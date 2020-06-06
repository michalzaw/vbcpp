#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

#include <string>
using std::string;

#include "RefCounter.h"

enum ResourceType
{
    RT_TEXTURE,
    RT_SHADER,
    RT_MODEL,
    RT_FONT,
    RT_SOUND,
	RT_OBJECT,
	RT_ROAD_PROFILE,
	RT_DISPLAY_FONT
};

class Resource
{
    public:
        Resource(ResourceType type, string path)
        : _type(type), _path(path) {}

        virtual ~Resource() {}

        string getPath() { return _path; }
        ResourceType getType() { return _type; }

    protected:
        string  _path;

    private:
        ResourceType _type;
};


#endif // RESOURCE_H_INCLUDED
