#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include "RTexture.h"

#include <iostream>
#include <list>
#include <memory>
typedef std::list<std::unique_ptr<Resource>> resourcePtrList;

class ResourceManager
{
    public:
        virtual ~ResourceManager();

        static ResourceManager& getInstance();

        RTexture* loadTexture(std::string path);

        void getString();

    protected:
        resourcePtrList _resources;

    private:
        ResourceManager();
};

#endif // RESOURCEMANAGER_H_INCLUDED
