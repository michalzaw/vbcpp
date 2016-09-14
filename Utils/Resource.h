#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

#include <string>
using std::string;

#include "RefCounter.h"

class Resource
{
    public:
        Resource(string path)
        : _path(path) {}

        virtual ~Resource() {}

        string getPath() { return _path; }

    protected:
        string  _path;
};


#endif // RESOURCE_H_INCLUDED
