#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include "RTexture.h"
#include "../Graphics/RShader.h"
#include "../Graphics/Model.h"
#include "RModel.h"
#include "RFont.h"

#include <iostream>
#include <list>
#include <memory>

typedef std::list<std::unique_ptr<Resource>> resourcePtrList;


class ResourceManager
{
    public:
        virtual ~ResourceManager();

        static ResourceManager& getInstance();

        RTexture2D* loadTexture(std::string path);

        RShader* loadShader(std::string vertexPath, std::string fragmPath, const std::vector<std::string>& defines = std::vector<std::string>());

        RModel* loadModel(std::string path, std::string texturePath /*, OGLDriver* driver */);

        RFont* loadFont(std::string path, int pixelSize = 32);

    protected:
        resourcePtrList _resources;

    private:
        ResourceManager();
};

#endif // RESOURCEMANAGER_H_INCLUDED
