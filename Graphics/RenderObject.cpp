#include "RenderObject.h"

#include "GraphicsManager.h"

RenderObject::RenderObject(RStaticModel* model, bool isDynamicObject)
    : Component(CT_RENDER_OBJECT),
    _isCastShadows(true), _isDynamicObject(isDynamicObject),
    _isCalculatedAABB(false)
{
    #ifdef _DEBUG_MODE
        printf("*** RenderObject: Konstruktor\n");
    #endif // _DEBUG_MODE
	
	_modelsDatas.resize(1);

    if (model != NULL)
    {
        setModel(model);
    }
}

RenderObject::~RenderObject()
{
	for (ModelData& modelData : _modelsDatas)
	{
		if (modelData.modelRootNode)
		{
			delete modelData.modelRootNode;
		}

		if (modelData.materials)
		{
			delete[] modelData.materials;
		}
	}
    #ifdef _DEBUG_MODE
        printf("*** RenderObject: Destruktor\n");
    #endif // _DEBUG_MODE
}


void RenderObject::calculateNewAABB()
{
	RStaticModel* model = _modelsDatas[0].model;

    glm::vec3 newMin(FLT_MAX, FLT_MAX, FLT_MAX); //_object->transformLocalPointToGlobal(aabbVertices[0]);
    glm::vec3 newMax(-FLT_MAX, -FLT_MAX, -FLT_MAX); //_object->transformLocalPointToGlobal(aabbVertices[0]);

    glm::vec3 min = model->getAABB()->getMinCoords();
    glm::vec3 max = model->getAABB()->getMaxCoords();

    glm::vec3 aabbVertices[] = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, max.y, max.z)
    };

    for (int i = 0; i < 8; ++i)
    {
        glm::vec3 vertex = _object->transformLocalPointToGlobal(aabbVertices[i]);

        if (vertex.x < newMin.x)
            newMin.x = vertex.x;
        if (vertex.x > newMax.x)
            newMax.x = vertex.x;

        if (vertex.y < newMin.y)
            newMin.y = vertex.y;
        if (vertex.y > newMax.y)
            newMax.y = vertex.y;

        if (vertex.z < newMin.z)
            newMin.z = vertex.z;
        if (vertex.z > newMax.z)
            newMax.z = vertex.z;
    }

    _aabb.setSize(newMin, newMax);
}


void RenderObject::setModel(RStaticModel* model, int lod)
{
	if (lod <= _modelsDatas.size())
	{
		_modelsDatas.resize(lod + 1);
	}

    _modelsDatas[lod].model = model;

    _modelsDatas[lod].modelRootNode = new ModelNode(model->getRootNode());

	_modelsDatas[lod].materialsCount = model->getMaterialsCount();
    _modelsDatas[lod].materials = new Material[model->getMaterialsCount()];
    for (int i = 0; i < model->getMaterialsCount(); ++i)
    {
		updateLocalMaterialFromModel(i, lod);
    }
}


RStaticModel* RenderObject::getModel(int lod)
{
    return _modelsDatas[lod].model;

}


ModelNode* RenderObject::getModelRootNode(int lod)
{
    return _modelsDatas[lod].modelRootNode;
}



ModelNode* RenderObject::getModelNodeByName(std::string name, ModelNode* node, int lod)
{
    if (node->getName() == name)
    {
        return node;
    }

    for (int i = 0; i < node->getChildrenCount(); ++i)
    {
        ModelNode* returnedNode = getModelNodeByName(name, node->getChildren()[i]);
        if (returnedNode != NULL)
        {
            return returnedNode;
        }
    }

    return NULL;
}


ModelNode* RenderObject::getModelNodeByName(std::string name, int lod)
{
    return getModelNodeByName(name, _modelsDatas[lod].modelRootNode, lod);
}


void RenderObject::updateLocalMaterialFromModel(unsigned int index, int lod)
{
	_modelsDatas[lod].materials[index] = *(_modelsDatas[lod].model->getMaterial(index));

	if (_modelsDatas[lod].materials[index].shader == MIRROR_MATERIAL)
	{
		MirrorComponent* mirrorComponent = GraphicsManager::getInstance().findMirrorComponent(getSceneObject(), _modelsDatas[lod].materials[index].mirrorName);
		if (mirrorComponent != NULL)
		{
			_modelsDatas[lod].materials[index].diffuseTexture = mirrorComponent->getFramebuffer()->getTexture();
		}
		else
		{
			GraphicsManager::getInstance().registerPendingMaterialForMirrorComponent(&_modelsDatas[lod].materials[index]);
		}
	}
}


Material* RenderObject::getMaterial(unsigned int index, int lod)
{
    return &_modelsDatas[lod].materials[index];
}


unsigned int RenderObject::getMaterialsCount(int lod)
{
	return _modelsDatas[lod].materialsCount;
}


void RenderObject::setIsCastShadows(bool isCastShadows)
{
    _isCastShadows = isCastShadows;
}


bool RenderObject::isCastShadows()
{
    return _isCastShadows;
}


void RenderObject::setIsDynamicObject(bool isDynamic)
{
	_isDynamicObject = isDynamic;
}


bool RenderObject::isDynamicObject()
{
	return _isDynamicObject;
}


AABB* RenderObject::getAABB()
{
    if (!_isCalculatedAABB)
    {
        calculateNewAABB();

        _isCalculatedAABB = true;
    }

    return &_aabb;
}


unsigned int RenderObject::getNumberOfLod()
{
	return _modelsDatas.size();
}


void RenderObject::changedTransform()
{
    _isCalculatedAABB = false;
}
