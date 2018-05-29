#include "GraphicsManager.h"

#include "../Scene/SceneObject.h"


static std::unique_ptr<GraphicsManager> gmInstance;

GraphicsManager::GraphicsManager()
    : _windDirection(0.0f, 0.0f, 0.0f), _windVelocity(0.0f), _windValue(0.0f), _windVector(0.0f, 0.0f, 0.0f)
{
    _quadTree = new QuadTree(glm::vec3(512, 512, 512));
}


GraphicsManager::~GraphicsManager()
{
    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        delete *i;
    }

    for (std::list<Grass*>::iterator i = _grassComponents.begin(); i != _grassComponents.end(); ++i)
    {
        delete *i;
    }

    for (std::vector<CameraStatic*>::iterator i = _cameras.begin(); i != _cameras.end(); ++i)
    {
        delete *i;
    }

    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        delete *i;
    }

    delete _quadTree;
}

GraphicsManager& GraphicsManager::getInstance()
{
    if ( !gmInstance )
        gmInstance = std::unique_ptr<GraphicsManager>(new GraphicsManager);

    return *gmInstance;
}


RenderObject* GraphicsManager::addRenderObject(RenderObject* object, SceneObject* owner)//RModel* model/*, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale*/)
{
    //RenderObject* object = new RenderObject(model);

    owner->addComponent(object);

    _renderObjects.push_back(object);
    _quadTree->addObject(object);

    return object;
}

Grass* GraphicsManager::addGrassComponent(RModel* model, RTexture2D* terrainHeightmap, RTexture2D* grassDensityTexture)
{
    Grass* grass = new Grass(model, terrainHeightmap, grassDensityTexture);

    _grassComponents.push_back(grass);

    return grass;
}

CameraStatic* GraphicsManager::addCameraStatic(CameraProjectionType projectionType)
{
    CameraStatic* camera = new CameraStatic(projectionType);

    _cameras.push_back(camera);

    return camera;
}

CameraFPS* GraphicsManager::addCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
{
    CameraFPS* camera = new CameraFPS(width, height, viewAngle, nearValue, farValue);

    _cameras.push_back(camera);

    return camera;
}

Light* GraphicsManager::addDirectionalLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity)
{
    Light* light = new Light(LT_DIRECTIONAL, color, ambientIntensity, diffuseIntensity);

    _lights.push_back(light);

    return light;
}


Light* GraphicsManager::addPointLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, LightAttenuation attenuation)
{
    Light* light = new Light(LT_POINT, color, ambientIntensity, diffuseIntensity);
    light->setAttenuation(attenuation);

    _lights.push_back(light);

    return light;
}


Light* GraphicsManager::addSpotLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, float cutoff, LightAttenuation attenuation)
{
    Light* light = new Light(LT_SPOT, color, ambientIntensity, diffuseIntensity);
    light->setAttenuation(attenuation);
    light->setCutoff(cutoff);

    _lights.push_back(light);

    return light;
}


void GraphicsManager::removeRenderObject(RenderObject* object)
{
    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _renderObjects.erase(i);

            delete object;

            return;
        }
    }
}

void GraphicsManager::removeGrassComponent(Grass* grass)
{
    for (std::list<Grass*>::iterator i = _grassComponents.begin(); i != _grassComponents.end(); ++i)
    {
        if (*i == grass)
        {
            i = _grassComponents.erase(i);

            delete grass;

            return;
        }
    }
}

void GraphicsManager::removeCamera(CameraStatic* camera)
{
    for (std::vector<CameraStatic*>::iterator i = _cameras.begin(); i != _cameras.end(); ++i)
    {
        if (*i == camera)
        {
            i = _cameras.erase(i);

            delete camera;

            return;
        }
    }
}

void GraphicsManager::removeLight(Light* light)
{
    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        if (*i == light)
        {
            i = _lights.erase(i);

            delete light;

            return;
        }
    }
}


void GraphicsManager::setCurrentCamera(CameraStatic* camera)
{
    _currentCamera = camera;
}


CameraStatic* GraphicsManager::getCurrentCamera()
{
    return _currentCamera;
}


void GraphicsManager::setWindDirection(glm::vec3 direction)
{
    _windDirection = direction;
}


void GraphicsManager::setWindVelocity(float velocity)
{
    _windVelocity = velocity;
}


glm::vec3 GraphicsManager::getWindDirection()
{
    return _windDirection;
}


float GraphicsManager::getWindVelocity()
{
    return _windVelocity;
}


glm::vec3 GraphicsManager::getWindVector()
{
    return _windVector;
}


float GraphicsManager::getWindValue()
{
    return _windValue;
}


void GraphicsManager::update(float deltaTime)
{
    _windValue += _windVelocity * deltaTime;
    _windVector = _windDirection * sinf(_windValue);
}


#ifdef ALL_OBJECTS
RenderData* GraphicsManager::getRenderData()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _currentCamera;
    //renderData->light = *(_lights.begin());

    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        if ((*i)->isActive())
            renderData->lights.push_back(*i);
    }
//std::cout << _renderObjects.size() << std::endl;
    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive())
            continue;

        for (int j = 0; j < object->getModel()->getQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(RET_SINGLE, object->getModel(), object->getModel()->getMesh(j), TransformMatrices(object->getSceneObject()->getGlobalTransformMatrix(), object->getSceneObject()->getGlobalNormalMatrix()),
                                            glm::length(renderData->camera->getPosition() - object->getSceneObject()->getPosition()), object->getSceneObject(), object);
            if (object->getModel()->getMesh(j)->material.transparency == 0.0f)
                renderData->renderList.insert(renderData->renderList.begin(), renderElement);
            else
                renderData->renderList.push_back(renderElement);
        }
    }

    return renderData;
}
#endif // ALL_OBJECTS

#ifdef FRUSTUM_CULLING
RenderData* GraphicsManager::getRenderData()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _currentCamera;
    //renderData->light = *(_lights.begin());

    Frustum frustum(renderData->camera->getProjectionMatrix() * renderData->camera->getViewMatrix());

    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        if ((*i)->isActive())
            renderData->lights.push_back(*i);
    }
//int k = 0;
    for (std::list<Grass*>::iterator i = _grassComponents.begin(); i != _grassComponents.end(); ++i)
    {
        RenderObject* object = *i;

        for (int j = 0; j < object->getModel()->getQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(RET_GRASS, object->getModel(), object->getModel()->getMesh(j), TransformMatrices(object->getSceneObject()->getGlobalTransformMatrix(), object->getSceneObject()->getGlobalNormalMatrix()),
                                            glm::length(renderData->camera->getPosition() - object->getSceneObject()->getPosition()), object->getSceneObject(), object);

            renderData->renderList.push_back(renderElement);
        }
    }

    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive() || !isAABBIntersectFrustum(frustum, *object->getAABB()))//!isPointInFrustum(frustum, object->getAABB()->getCenterPosition()))
            continue;
//k++;
        for (int j = 0; j < object->getModel()->getQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(RET_SINGLE, object->getModel(), object->getModel()->getMesh(j), TransformMatrices(object->getSceneObject()->getGlobalTransformMatrix(), object->getSceneObject()->getGlobalNormalMatrix()),
                                            glm::length(renderData->camera->getPosition() - object->getSceneObject()->getPosition()), object->getSceneObject(), object);
            if (object->getModel()->getMesh(j)->material.transparency == 0.0f)
                renderData->renderList.insert(renderData->renderList.begin(), renderElement);
            else
                renderData->renderList.push_back(renderElement);
        }
    }

//std::cout << k << std::endl;
    return renderData;
}
#endif // FRUSTUM_CULLING

RenderData* GraphicsManager::getRenderDataForDepthRendering()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _currentCamera;

    AABB* cameraAabb = renderData->camera->getAABB();

    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive() || !isAABBIntersectAABB(*cameraAabb, *object->getAABB()))
            continue;

        for (int j = 0; j < object->getModel()->getQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(RET_SINGLE, object->getModel(), object->getModel()->getMesh(j), TransformMatrices(object->getSceneObject()->getGlobalTransformMatrix(), object->getSceneObject()->getGlobalNormalMatrix()),
                                            glm::length(renderData->camera->getPosition() - object->getSceneObject()->getPosition()), object->getSceneObject(), object);
            if (object->getModel()->getMesh(j)->material.transparency == 0.0f)
                renderData->renderList.push_back(renderElement);
        }
    }

    return renderData;
}

#ifdef QUAD_TREE
RenderData* GraphicsManager::getRenderData()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _currentCamera;
    //renderData->light = *(_lights.begin());

    Frustum frustum(renderData->camera->getProjectionMatrix() * renderData->camera->getViewMatrix());

    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        if ((*i)->isActive())
            renderData->lights.push_back(*i);
    }

    std::list<RenderObject*> objects;
    _quadTree->getObjectsInFrustum(&objects, frustum);

//std::cout << objects.size() << std::endl;
    for (std::list<RenderObject*>::iterator i = objects.begin(); i != objects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive())
            continue;

        for (int j = 0; j < object->getModel()->getQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(RET_SINGLE, object->getModel(), object->getModel()->getMesh(j), TransformMatrices(object->getSceneObject()->getGlobalTransformMatrix(), object->getSceneObject()->getGlobalNormalMatrix()),
                                            glm::length(renderData->camera->getPosition() - object->getSceneObject()->getPosition()), object->getSceneObject(), object);
            if (object->getModel()->getMesh(j)->material.transparency == 0.0f)
                renderData->renderList.insert(renderData->renderList.begin(), renderElement);
            else
                renderData->renderList.push_back(renderElement);
        }
    }

    return renderData;
}
#endif // QUAD_TREE
