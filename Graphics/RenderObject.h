#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"
#include "ModelNode.h"
#include "AABB.h"

#include "../Utils/RStaticModel.h"

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

        AABB _aabb;
        bool _isCalculatedAABB;

        void calculateNewAABB();

        ModelNode* getModelNodeByName(std::string name, ModelNode* node, int lod = 0);

    public:
        RenderObject(RStaticModel* model = NULL, bool isDynamicObject = false);
        RenderObject(RStaticModel* model, const std::vector<std::string>& nodesToSkip, bool isDynamicObject = false);
        RenderObject(RStaticModel* model, StaticModelNode* modelNode, bool isDynamicObject = false);
        virtual ~RenderObject();

        void setModel(RStaticModel* model, int lod = 0);
        void setModel(RStaticModel* model, StaticModelNode* modelNode, int lod = 0);
        void setModel(RStaticModel* model, const std::vector<std::string>& nodesToSkip, StaticModelNode* modelNode, int lod = 0);
        RStaticModel* getModel(int lod = 0);
        ModelNode* getModelRootNode(int lod = 0);
        ModelNode* getModelNodeByName(std::string name, int lod = 0);

		void updateLocalMaterialFromModel(unsigned int index, int lod = 0);
        void replaceMaterialsByName(const std::vector<Material*>& materials, int lod = 0);

        void addMirrorMaterial(Material* material);
        const std::vector<Material*>& getMirrorMaterials();

        void setCastShadows(bool isCastShadows);
        bool isCastShadows();
		void setDynamicObject(bool isDynamic);
		bool isDynamicObject();

        void setIsHighlighted(bool isHighlighted);
        bool isHighlighted();

        AABB* getAABB();

		unsigned int getNumberOfLod();

        virtual void changedTransform();

};


#endif // RENDEROBJECT_H_INCLUDED
