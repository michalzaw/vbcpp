#include "RenderObject.h"

#include "GraphicsManager.h"

#include "../Scene/SceneManager.h"

#include "../Utils/Logger2.h"

RenderObject::RenderObject(RStaticModel* model, bool isDynamicObject)
    : Component(CT_RENDER_OBJECT),
    _isCastShadows(true), _isDynamicObject(isDynamicObject),
    _isCalculatedAABB(false)
{
    LOG_DEBUG("*** RenderObject: Konstruktor");
	
	_modelsDatas.resize(1);

    if (model != NULL)
    {
        setModel(model);
    }
}


RenderObject::RenderObject(RStaticModel* model, const std::vector<std::string>& nodesToSkip, bool isDynamicObject)
    : Component(CT_RENDER_OBJECT),
    _isCastShadows(true), _isDynamicObject(isDynamicObject),
    _isCalculatedAABB(false)
{
    LOG_DEBUG("*** RenderObject: Konstruktor");

    _modelsDatas.resize(1);

    if (model != NULL)
    {
        setModel(model, nodesToSkip, model->getRootNode());
    }
}


RenderObject::RenderObject(RStaticModel* model, StaticModelNode* modelNode, bool isDynamicObject)
    : Component(CT_RENDER_OBJECT),
    _isCastShadows(true), _isDynamicObject(isDynamicObject),
    _isCalculatedAABB(false)
{
    LOG_DEBUG("*** RenderObject: Konstruktor");

    _modelsDatas.resize(1);

    if (model != NULL)
    {
        setModel(model, modelNode);
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
	}
    LOG_DEBUG("*** RenderObject: Destruktor");
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
    setModel(model, std::vector<std::string>{}, model->getRootNode(), lod);
}


void RenderObject::setModel(RStaticModel* model, StaticModelNode* modelNode, int lod)
{
    setModel(model, std::vector<std::string>{}, modelNode, lod);
    _modelsDatas[lod].modelRootNode->getTransformNode().setPosition(0.0f, 0.0f, 0.0f);
    _modelsDatas[lod].modelRootNode->getTransformNode().setRotation(0.0f, 0.0f, 0.0f);
    _modelsDatas[lod].modelRootNode->getTransformNode().setScale(1.0f, 1.0f, 1.0f);
}


void RenderObject::setModel(RStaticModel* model, const std::vector<std::string>& nodesToSkip, StaticModelNode* modelNode, int lod)
{
    if (lod <= _modelsDatas.size())
    {
        _modelsDatas.resize(lod + 1);
    }

    _modelsDatas[lod].model = model;

    _modelsDatas[lod].modelRootNode = new ModelNode(model, modelNode, nodesToSkip, this);

    for (int i = 0; i < model->getMaterialsCount(); ++i)
    {
        //updateLocalMaterialFromModel(i, lod);
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
    Material* material = _modelsDatas[lod].model->getMaterial(index);
	if (material->shader == MIRROR_MATERIAL)
	{
        GraphicsManager* graphicsManager = getSceneObject()->getSceneManager()->getGraphicsManager();
		MirrorComponent* mirrorComponent = graphicsManager->findMirrorComponent(getSceneObject(), material->mirrorName);
		if (mirrorComponent != NULL)
		{
            material->diffuseTexture = mirrorComponent->getFramebuffer()->getTexture();
		}
		else
		{
            graphicsManager->registerPendingMaterialForMirrorComponent(material);
		}
	}
}


void RenderObject::replaceMaterialsByName(const std::vector<Material*>& materials, int lod)
{
    _modelsDatas[lod].modelRootNode->replaceMaterialsByName(materials);
}


void RenderObject::addMirrorMaterial(Material* material)
{
    _mirrorMaterials.push_back(material);
}


const std::vector<Material*>& RenderObject::getMirrorMaterials()
{
    return _mirrorMaterials;
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
