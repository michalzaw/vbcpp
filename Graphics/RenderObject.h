#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"
#include "ModelNode.h"
#include "AABB.h"

#include "../Utils/RStaticModel.h"

#include "../Scene/Component.h"


class RenderObject : public Component
{
    protected:
        RStaticModel* _model;
        ModelNode*    _modelRootNode;
        Material*     _materials;
		unsigned int  _materialsCount;

        bool _isCastShadows;
		bool _isDynamicObject;

        AABB _aabb;
        bool _isCalculatedAABB;

        void calculateNewAABB();

        ModelNode* getModelNodeByName(std::string name, ModelNode* node);

    public:
        RenderObject(RStaticModel* model = NULL, bool isDynamicObject = false);
        virtual ~RenderObject();

        void setModel(RStaticModel* model);
        RStaticModel* getModel();
        ModelNode* getModelRootNode();
        ModelNode* getModelNodeByName(std::string name);

        Material* getMaterial(unsigned int index);
		unsigned int getMaterialsCount();

        void setIsCastShadows(bool isCastShadows);
        bool isCastShadows();
		void setIsDynamicObject(bool isDynamic);
		bool isDynamicObject();

        AABB* getAABB();

        virtual void changedTransform();

};


#endif // RENDEROBJECT_H_INCLUDED
