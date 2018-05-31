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
#include <sstream>

typedef std::list<std::unique_ptr<Resource>> resourcePtrList;


class ResourceManager
{
    public:
        virtual ~ResourceManager();

        static ResourceManager& getInstance();

        Resource* findResource(std::string path);

        RTexture2D* loadTexture(std::string path);
        void reloadTexture(RTexture2D* texture);
        void reloadTexture(std::string path);
        void reloadAllTextures();

        RShader* loadShader(std::string vertexPath, std::string fragmPath, const std::vector<std::string>& defines = std::vector<std::string>());
        void reloadShader(RShader* shader);
        void reloadAllShaders();

        RModel* loadModel(std::string path, std::string texturePath /*, OGLDriver* driver */);

        RFont* loadFont(std::string path, int pixelSize = 32);

    protected:
        resourcePtrList _resources;

    private:
        ResourceManager();
};

#endif // RESOURCEMANAGER_H_INCLUDED
