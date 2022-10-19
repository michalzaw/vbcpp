#include "GraphicsManager.h"

#include "Renderer.h"

#include "../Scene/SceneObject.h"


GraphicsManager::GraphicsManager()
    : _windDirection(0.0f, 0.0f, 0.0f), _windVelocity(0.0f), _windValue(0.0f), _windVector(0.0f, 0.0f, 0.0f), _windTimer(0.0f),
    _globalEnvironmentCaptureComponent(NULL), _sky(NULL)
{
    //_quadTree = new QuadTree(glm::vec3(512, 512, 512));
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

    for (std::list<EnvironmentCaptureComponent*>::iterator i = _environmentCaptureComponents.begin(); i != _environmentCaptureComponents.end(); ++i)
    {
        delete *i;
    }

    for (std::vector<MirrorComponent*>::iterator i = _mirrorComponents.begin(); i != _mirrorComponents.end(); ++i)
    {
        delete *i;
    }

    for (std::list<ClickableObject*>::iterator i = _clickableObjects.begin(); i != _clickableObjects.end(); ++i)
    {
        delete *i;
    }

	for (std::list<DisplayComponent*>::iterator i = _displayComponents.begin(); i != _displayComponents.end(); ++i)
	{
		delete* i;
	}

	if (_sky != NULL)
	{
		delete _sky;
	}

    //delete _quadTree;
}


RenderObject* GraphicsManager::addRenderObject(RenderObject* object, SceneObject* owner)//RModel* model/*, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale*/)
{
    //RenderObject* object = new RenderObject(model);

    owner->addComponent(object);

    _renderObjects.push_back(object);
    //_quadTree->addObject(object);

    for (Material* material : object->getMirrorMaterials())
    {
        MirrorComponent* mirrorComponent = findMirrorComponent(object->getSceneObject(), material->mirrorName);
        if (mirrorComponent != NULL)
        {
            material->diffuseTexture = mirrorComponent->getFramebuffer()->getTexture();
        }
        else
        {
            registerPendingMaterialForMirrorComponent(material);
        }
    }

    return object;
}

RoadObject* GraphicsManager::addRoadObject(RoadType roadType, RRoadProfile* roadProfile, const std::vector<glm::vec3>& points, const std::vector<RoadSegment>& segments, bool buildModelAfterCreate, SceneObject* owner)
{
	RoadObject* roadObject = new RoadObject(roadType, roadProfile, points, segments, buildModelAfterCreate);

	owner->addComponent(roadObject);

	_renderObjects.push_back(roadObject);
    _roadObjects.push_back(roadObject);

	return roadObject;
}

Terrain* GraphicsManager::addTerrain(std::string heightmapFileName, std::string dirPath, std::string materialName, float maxHeight, bool is16bitTexture, SceneObject* owner)
{
	Terrain* terrain = new Terrain(heightmapFileName, dirPath, materialName, maxHeight, is16bitTexture);

	owner->addComponent(terrain);

	_renderObjects.push_back(terrain);

	return terrain;
}

Grass* GraphicsManager::addGrassComponent(RStaticModel* model, RTexture2D* terrainHeightmap, RTexture2D* grassDensityTexture)
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


EnvironmentCaptureComponent* GraphicsManager::addEnvironmentCaptureComponent(RTextureCubeMap* environmentMap, RTextureCubeMap* irradianceMap, RTextureCubeMap* specularIrradianceMap)
{
    EnvironmentCaptureComponent* component = new EnvironmentCaptureComponent(environmentMap, irradianceMap, specularIrradianceMap);

    _environmentCaptureComponents.push_back(component);

    return component;
}


EnvironmentCaptureComponent* GraphicsManager::addGlobalEnvironmentCaptureComponent(RTextureCubeMap* environmentMap, RTextureCubeMap* irradianceMap, RTextureCubeMap* specularIrradianceMap)
{
    _globalEnvironmentCaptureComponent = addEnvironmentCaptureComponent(environmentMap, irradianceMap, specularIrradianceMap);

    return _globalEnvironmentCaptureComponent;
}


MirrorComponent* GraphicsManager::addMirrorComponent(std::string name, float renderingDistance)
{
    MirrorComponent* mirrorComponent = new MirrorComponent(name, renderingDistance);

    _mirrorComponents.push_back(mirrorComponent);

    findAndAssigneMirrorComponentToPendingMaterials(mirrorComponent);

    return mirrorComponent;
}


ClickableObject* GraphicsManager::addClickableObject()
{
    ClickableObject* clickableObject = new ClickableObject();

    _clickableObjects.push_back(clickableObject);

    return clickableObject;
}


DisplayComponent* GraphicsManager::addDisplayComponent(RDisplayFont* font, int displayWidth, int displayHeight, glm::vec3 textColor)
{
	DisplayComponent* displayComponent = new DisplayComponent(font, displayWidth, displayHeight, textColor);

	_displayComponents.push_back(displayComponent);

	return displayComponent;
}


Sky* GraphicsManager::addSky(RTexture* texture, SceneObject* owner)
{
	if (_sky != NULL)
	{
		return NULL;
	}

	_sky = new Sky(texture);

	owner->addComponent(_sky);

	return _sky;
}


CrossroadComponent* GraphicsManager::addCrossRoad(std::vector<CrossroadConnectionPoint>& connectionPoints)
{
	CrossroadComponent* crossroadComponent = new CrossroadComponent(connectionPoints);

	_crossroadComponents.push_back(crossroadComponent);

	return crossroadComponent;
}


RoadIntersectionComponent* GraphicsManager::addRoadIntersection(RRoadProfile* edgeRoadProfile, bool interactiveMode)
{
    RoadIntersectionComponent* roadIntersection = new RoadIntersectionComponent(edgeRoadProfile, interactiveMode);

    _roadIntersectionComponents.push_back(roadIntersection);

    return roadIntersection;
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

void GraphicsManager::removeRoadObject(RoadObject* object)
{
    for (std::vector<RoadObject*>::iterator i = _roadObjects.begin(); i != _roadObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _roadObjects.erase(i);

            break;
        }
    }

	removeRenderObject(object);
}


void GraphicsManager::removeTerrain(Terrain* object)
{
	removeRenderObject(object);
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


void GraphicsManager::removeEnvironmetnCaptureComponent(EnvironmentCaptureComponent* component)
{
    for (std::list<EnvironmentCaptureComponent*>::iterator i = _environmentCaptureComponents.begin(); i != _environmentCaptureComponents.end(); ++i)
    {
        if (*i == component)
        {
            i = _environmentCaptureComponents.erase(i);

            if (_globalEnvironmentCaptureComponent == component)
                _globalEnvironmentCaptureComponent = NULL;

            delete component;

            return;
        }
    }
}


void GraphicsManager::removeMirrorComponent(MirrorComponent* mirrorComponent)
{
    for (std::vector<MirrorComponent*>::iterator i = _mirrorComponents.begin(); i != _mirrorComponents.end(); ++i)
    {
        if (*i == mirrorComponent)
        {
            i = _mirrorComponents.erase(i);

            delete mirrorComponent;

            return;
        }
    }
}


void GraphicsManager::removeClickableObject(ClickableObject* clickableObject)
{
    for (std::list<ClickableObject*>::iterator i = _clickableObjects.begin(); i != _clickableObjects.end(); ++i)
    {
        if (*i == clickableObject)
        {
            i = _clickableObjects.erase(i);

            delete clickableObject;

            return;
        }
    }
}


void GraphicsManager::removeDisplayComponent(DisplayComponent* displayComponent)
{
	for (std::list<DisplayComponent*>::iterator i = _displayComponents.begin(); i != _displayComponents.end(); ++i)
	{
		if (*i == displayComponent)
		{
			i = _displayComponents.erase(i);

			delete displayComponent;

			return;
		}
	}
}


void GraphicsManager::removeSky(Sky* sky)
{
	if (sky != _sky)
		return;
	
	delete _sky;
	_sky = NULL;
}


void GraphicsManager::removeCrossroadComponent(CrossroadComponent* crossroadComponent)
{
	for (std::vector<CrossroadComponent*>::iterator i = _crossroadComponents.begin(); i != _crossroadComponents.end(); ++i)
	{
		if (*i == crossroadComponent)
		{
			i = _crossroadComponents.erase(i);

			delete crossroadComponent;

			return;
		}
	}
}


void GraphicsManager::removeRoadIntersectionComponent(RoadIntersectionComponent* roadIntersectionComponent)
{
    for (std::vector<RoadIntersectionComponent*>::iterator i = _roadIntersectionComponents.begin(); i != _roadIntersectionComponents.end(); ++i)
    {
        if (*i == roadIntersectionComponent)
        {
            i = _roadIntersectionComponents.erase(i);

            delete roadIntersectionComponent;

            return;
        }
    }
}


void GraphicsManager::setCurrentCamera(CameraStatic* camera)
{
    _currentCamera = camera;
    Renderer::getInstance().setCurrentMainCamera(camera);
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


float GraphicsManager::getWindTimer()
{
	return _windTimer;
}


std::list<RenderObject*>& GraphicsManager::getRenderObjects()
{
    return _renderObjects;
}


std::vector<CrossroadComponent*>& GraphicsManager::getCrossroadComponents()
{
	return _crossroadComponents;
}


std::vector<RoadIntersectionComponent*>& GraphicsManager::getRoadIntersectionComponents()
{
    return _roadIntersectionComponents;
}


std::vector<RoadObject*>& GraphicsManager::getRoadObjects()
{
    return _roadObjects;
}


Sky* GraphicsManager::getSky()
{
	return _sky;
}


EnvironmentCaptureComponent* GraphicsManager::getGlobalEnvironmentCaptureComponent()
{
    return _globalEnvironmentCaptureComponent;
}


MirrorComponent* GraphicsManager::findMirrorComponent(SceneObject* object, std::string name)
{
    if (object != NULL)
    {
        Component* mirrorComponent = object->getComponent(CT_MIRROR);
        if (mirrorComponent != NULL)
        {
            MirrorComponent* m = static_cast<MirrorComponent*>(mirrorComponent);
            if (m->getName() == name)
                return m;
        }

        std::list<SceneObject*>& children = object->getChildren();
        for (std::list<SceneObject*>::iterator i = children.begin(); i != children.end(); ++i)
        {
            MirrorComponent* mirrorComponent = findMirrorComponent(*i, name);
            if (mirrorComponent != NULL)
                return mirrorComponent;
        }
    }

    return NULL;
}


void GraphicsManager::registerPendingMaterialForMirrorComponent(Material* material)
{
    _pendingMaterialsForMirrorComponent.push_back(material);
}


void GraphicsManager::findAndAssigneMirrorComponentToPendingMaterials(MirrorComponent* mirrorComponent)
{
    for (std::list<Material*>::iterator i = _pendingMaterialsForMirrorComponent.begin();
         i != _pendingMaterialsForMirrorComponent.end();
         ++i)
    {
        if ((*i)->mirrorName == mirrorComponent->getName())
        {
            (*i)->diffuseTexture = mirrorComponent->getFramebuffer()->getTexture();

            i = _pendingMaterialsForMirrorComponent.erase(i);
            break;
        }
    }
}


void GraphicsManager::update(float deltaTime)
{
    _windValue += _windVelocity * deltaTime;
    _windVector = _windDirection * sinf(_windValue);

	_windTimer += deltaTime;

    for (std::list<ClickableObject*>::iterator i = _clickableObjects.begin(); i != _clickableObjects.end(); ++i)
    {
        (*i)->clear();
    }

    for (DisplayComponent* displayComponent : _displayComponents)
    {
        displayComponent->update(deltaTime);
    }

    for (RoadIntersectionComponent* roadIntersectionComponent : _roadIntersectionComponents)
    {
        roadIntersectionComponent->update(deltaTime);
    }
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

        for (int j = 0; j < object->getModel()->getMeshesCount(); ++j)
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

        for (int j = 0; j < object->getModel()->getMeshesCount(); ++j)
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
        for (int j = 0; j < object->getModel()->getMeshesCount(); ++j)
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

RenderData* GraphicsManager::getRenderDataForDepthRendering()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _currentCamera;

    AABB* cameraAabb = renderData->camera->getAABB();

    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive() || !isAABBIntersectAABB(*cameraAabb, *object->getAABB()) || !object->isCastShadows())
            continue;

        for (int j = 0; j < object->getModel()->getMeshesCount(); ++j)
        {
            RenderListElement renderElement(RET_SINGLE, object->getModel(), object->getModel()->getMesh(j), TransformMatrices(object->getSceneObject()->getGlobalTransformMatrix(), object->getSceneObject()->getGlobalNormalMatrix()),
                                            glm::length(renderData->camera->getPosition() - object->getSceneObject()->getPosition()), object->getSceneObject(), object);
            if (object->getModel()->getMesh(j)->material.transparency == 0.0f)
                renderData->renderList.push_back(renderElement);
        }
    }

    return renderData;
}
#endif // FRUSTUM_CULLING

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

        for (int j = 0; j < object->getModel()->getMeshesCount(); ++j)
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
