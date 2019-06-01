#include "RenderObject.h"

#include "GraphicsManager.h"

RenderObject::RenderObject(RStaticModel* model)
    : Component(CT_RENDER_OBJECT),
    _model(NULL),
    _materials(NULL),
    _isCastShadows(true),
    _isCalculatedAABB(false)
{
    #ifdef _DEBUG_MODE
        printf("*** RenderObject: Konstruktor\n");
    #endif // _DEBUG_MODE

    if (model != NULL)
    {
        setModel(model);
    }
}

RenderObject::~RenderObject()
{
    if (_modelRootNode)
    {
        delete _modelRootNode;
    }

    if (_materials)
    {
        delete[] _materials;
    }
    #ifdef _DEBUG_MODE
        printf("*** RenderObject: Destruktor\n");
    #endif // _DEBUG_MODE
}


void RenderObject::calculateNewAABB()
{
    glm::vec3 newMin(FLT_MAX, FLT_MAX, FLT_MAX); //_object->transformLocalPointToGlobal(aabbVertices[0]);
    glm::vec3 newMax(-FLT_MAX, -FLT_MAX, -FLT_MAX); //_object->transformLocalPointToGlobal(aabbVertices[0]);

    glm::vec3 min = _model->getAABB()->getMinCoords();
    glm::vec3 max = _model->getAABB()->getMaxCoords();

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


void RenderObject::setModel(RStaticModel* model)
{
    _model = model;

    _modelRootNode = new ModelNode(_model->getRootNode());

    if (_materials)
    {
        delete[] _materials;
    }

    _materials = new Material[_model->getMaterialsCount()];
    for (int i = 0; i < _model->getMaterialsCount(); ++i)
    {
        _materials[i] = *(_model->getMaterial(i));

        if (_materials[i].shader == MIRROR_MATERIAL)
        {
            MirrorComponent* mirrorComponent = GraphicsManager::getInstance().findMirrorComponent(getSceneObject(), _materials[i].mirrorName);
            if (mirrorComponent != NULL)
            {
                _materials[i].diffuseTexture = mirrorComponent->getFramebuffer()->getTexture();
            }
            else
            {
                GraphicsManager::getInstance().registerPendingMaterialForMirrorComponent(&_materials[i]);
            }
        }
    }
}


RStaticModel* RenderObject::getModel()
{
    return _model;

}


ModelNode* RenderObject::getModelRootNode()
{
    return _modelRootNode;
}



ModelNode* RenderObject::getModelNodeByName(std::string name, ModelNode* node)
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


ModelNode* RenderObject::getModelNodeByName(std::string name)
{
    return getModelNodeByName(name, _modelRootNode);
}


Material* RenderObject::getMaterial(unsigned int index)
{
    return &_materials[index];
}


void RenderObject::setIsCastShadows(bool isCastShadows)
{
    _isCastShadows = isCastShadows;
}


bool RenderObject::isCastShadows()
{
    return _isCastShadows;
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


void RenderObject::changedTransform()
{
    _isCalculatedAABB = false;
}
