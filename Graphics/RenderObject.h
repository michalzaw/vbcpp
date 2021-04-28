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

        bool _isCastShadows;
		bool _isDynamicObject;

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

        void setIsCastShadows(bool isCastShadows);
        bool isCastShadows();
		void setIsDynamicObject(bool isDynamic);
		bool isDynamicObject();

        AABB* getAABB();

		unsigned int getNumberOfLod();

        virtual void changedTransform();

};


#endif // RENDEROBJECT_H_INCLUDED
