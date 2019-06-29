#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include "RTexture.h"
#include "../Graphics/RShader.h"
#include "../Graphics/Model.h"
#include "RStaticModel.h"
#include "RFont.h"
#include "SoundLoader.h"

#include <iostream>
#include <list>
#include <memory>
#include <sstream>

typedef std::list<std::unique_ptr<Resource>> resourcePtrList;


class ResourceManager
{
    static constexpr char* DEFAULT_WHITE_TEXTURE_NAME = ".defaultTexture";

    public:
        virtual ~ResourceManager();

        static ResourceManager& getInstance();

        Resource* findResource(std::string path);

        RTexture2D* loadTexture(std::string path);
        // filesNames: pos_x, neg_x, pos_y, neg_y, pos_z, neg_z
        RTextureCubeMap* loadTextureCubeMap(std::string* fileNames);
        void reloadTexture(RTexture2D* texture);
        void reloadTexture(std::string path);
        void reloadTexture(RTextureCubeMap* texture);
        void reloadAllTextures();

        RTexture2D* loadDefaultWhiteTexture();

        RShader* loadShader(std::string vertexPath, std::string fragmPath, const std::vector<std::string>& defines = std::vector<std::string>());
        void reloadShader(RShader* shader);
        void reloadAllShaders();

        RStaticModel* loadModelWithHierarchy(std::string path, std::string texturePath /*, OGLDriver* driver */);
        RStaticModel* loadModelWithHierarchy(std::string path, std::string texturePath, std::vector<std::string> nodesToSkipNames);
        RStaticModel* loadModelWithHierarchyOnlyNode(std::string path, std::string texturePath, std::string nodeToLoadName, Transform& loadedNodeTransformInModel);
        RStaticModel* loadModel(std::string path, std::string texturePath);

        RFont* loadFont(std::string path, int pixelSize = 32);

        RSound* loadSound(std::string path);

    protected:
        resourcePtrList _resources;

    private:
        ResourceManager();
};

#endif // RESOURCEMANAGER_H_INCLUDED
