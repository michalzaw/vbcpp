#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"
#include "ModelNode.h"
#include "AABB.h"

#include "../Utils/RStaticModel.h"

#include "../Scripting/Utils/LuaMacros.h"

#include "../Scene/Component.h"


struct ModelData
{
	RStaticModel* model;
	ModelNode* modelRootNode;

	ModelData()
		: model(NULL),
		modelRootNode(NULL)
	{}
};


class RenderObject : public Component
{
    protected:
		std::vector<ModelData> _modelsDatas;

        std::vector<Material*> _mirrorMaterials;

        bool _isCastShadows;
		bool _isDynamicObject;

        bool _isHighlighted;
        bool _isRenderObjectId;

        AABB _aabb;
        bool _isCalculatedAABB;

        void calculateNewAABB();

        ModelNode* getModelNodeByName(std::string name, ModelNode* node, int lod = 0);

    public:
        RenderObject(RStaticModel* model = NULL, bool isDynamicObject = false);
        RenderObject(RStaticModel* model, const std::vector<std::string>& nodesToSkip, bool isDynamicObject = false);
        RenderObject(RStaticModel* model, StaticModelNode* modelNode, bool isDynamicObject = false);
        virtual ~RenderObject();

        LUAF void setModel(RStaticModel* model, int lod = 0);
        LUAF void setModel(RStaticModel* model, StaticModelNode* modelNode, int lod = 0);
        LUAF void setModel(RStaticModel* model, const std::vector<std::string>& nodesToSkip, StaticModelNode* modelNode, int lod = 0);
        LUAF RStaticModel* getModel(int lod = 0);
        LUAF ModelNode* getModelRootNode(int lod = 0);
        LUAR ModelNode* getModelNodeByName(std::string name, int lod = 0);

        LUAF void updateLocalMaterialFromModel(unsigned int index, int lod = 0);
        LUAF void replaceMaterialsByName(const std::vector<Material*>& materials, int lod = 0);

        LUAF void addMirrorMaterial(Material* material);
        LUAF const std::vector<Material*>& getMirrorMaterials();

        LUAF void setCastShadows(bool isCastShadows);
        LUAF bool isCastShadows();
        LUAF void setDynamicObject(bool isDynamic);
        LUAF bool isDynamicObject();

        LUAF void setIsHighlighted(bool isHighlighted);
        LUAF bool isHighlighted();
        LUAF inline void setIsRenderObjectId(bool isRenderObjectId) { _isRenderObjectId = isRenderObjectId; }
        LUAF inline bool isRenderObjectId() { return _isRenderObjectId; }

        LUAF AABB* getAABB();

        LUAF unsigned int getNumberOfLod();

        LUAF virtual void changedTransform();

};


#endif // RENDEROBJECT_H_INCLUDED
