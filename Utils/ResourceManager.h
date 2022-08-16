#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include "RTexture.h"
#include "../Graphics/RShader.h"
#include "../Graphics/Model.h"
#include "../Graphics/RRoadProfile.h"
#include "../Graphics/RoadProfileLoader.h"
#include "../Graphics/RDisplayFont.h"
#include "../Graphics/RMaterialsCollection.h"
#include "../Graphics/RAnimation.h"
#include "RAnimatedModel.h"
#include "RStaticModel.h"
#include "RFont.h"
#include "SoundLoader.h"
#include "RObject.h"
#include "RObjectLoader.h"

#include <iostream>
#include <list>
#include <memory>
#include <sstream>

#define DEVELOPMENT_RESOURCES

typedef std::list<std::unique_ptr<Resource>> resourcePtrList;


class ResourceManager
{
    static constexpr char* DEFAULT_WHITE_TEXTURE_NAME = ".defaultTexture";

    public:
        virtual ~ResourceManager();

        static ResourceManager& getInstance();

        Resource* findResource(std::string path);

        RTexture2D* loadTexture(std::string path, bool useCompression = true, bool mipmapping = true, bool useAnisotropicFiltering = true);
        // filesNames: pos_x, neg_x, pos_y, neg_y, pos_z, neg_z
        RTextureCubeMap* loadTextureCubeMap(std::string* fileNames);
        void reloadTexture(RTexture2D* texture);
        void reloadTexture(std::string path);
        void reloadTexture(RTextureCubeMap* texture);
        void reloadAllTextures();

        RTexture2D* loadDefaultWhiteTexture();
		RTexture2D* loadOneColorTexture(glm::vec4 color);

        RShader* loadShader(std::string vertexPath, std::string fragmPath, const std::vector<std::string>& defines = std::vector<std::string>(),
                            const std::unordered_map<std::string, std::string>& constants = std::unordered_map<std::string, std::string>());
        void reloadShader(RShader* shader);
        void reloadAllShaders();

        RStaticModel* loadModelWithHierarchy(std::string path, std::string texturePath, bool normalsSmoothing = true /*, OGLDriver* driver */);
        RStaticModel* loadModelWithHierarchy(std::string path, std::string texturePath, std::vector<std::string> nodesToSkipNames, bool normalsSmoothing = true);
        RStaticModel* loadModelWithHierarchyOnlyNode(std::string path, std::string texturePath, std::string nodeToLoadName, Transform& loadedNodeTransformInModel,
													 bool normalsSmoothing = true);
        void loadModelWithHierarchyOnlyNodes(std::string path, std::string texturePath, std::vector<std::string> nodesToLoadNames,
                                             std::vector<Transform>& loadedNodesTransformsInModel, std::vector<RStaticModel*>& loadedNodes,
											 bool normalsSmoothing = true);
        RStaticModel* loadModel(std::string path, std::string texturePath, bool normalsSmoothing = true);

        RAnimatedModel* loadAnimatedModel(const std::string& path, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel = {});
        RAnimation* loadAnimation(const std::string& path);

        RFont* loadFont(std::string path, int pixelSize = 32);

        RSound* loadSound(std::string path);

		RObject* loadRObject(std::string name);

		RRoadProfile* loadRoadProfile(std::string name);

		RDisplayFont* loadDisplayFont(std::string name);

        RMaterialsCollection* loadMaterialsCollection(std::string path);

		void setAlternativeResourcePath(std::string path);
		std::string getAlternativeResourcePath();

    protected:
        resourcePtrList _resources;

    private:
        ResourceManager();

		std::string _alternativeResourcePath;
};

#endif // RESOURCEMANAGER_H_INCLUDED
