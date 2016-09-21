#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include "RTexture.h"
#include "../Graphics/RShader.h"
#include "../Graphics/Model.h"
#include "RModel.h"

#include <iostream>
#include <list>
#include <memory>

typedef std::list<std::unique_ptr<Resource>> resourcePtrList;


class ResourceManager
{
    public:
        virtual ~ResourceManager();

        static ResourceManager& getInstance();

        GLuint loadTexture(std::string path);

        RShader* loadShader(std::string vertexPath, std::string fragmPath);

        RModel* loadModel(std::string path, std::string texturePath /*, OGLDriver* driver */);

    protected:
        resourcePtrList _resources;

    private:
        ResourceManager();
};

#endif // RESOURCEMANAGER_H_INCLUDED
