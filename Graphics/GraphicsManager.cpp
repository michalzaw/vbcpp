#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(/*OGLDriver* driver*/)
    /*: _OGLDriver(driver), _camera(0), _light(0)*/
{

}


GraphicsManager::~GraphicsManager()
{
    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
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
}


RenderObject* GraphicsManager::AddRenderObject(Model* model/*, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale*/)
{
    RenderObject* object = new RenderObject(model);

    _renderObjects.push_back(object);

    return object;
}

CameraStatic* GraphicsManager::AddCameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
{
    CameraStatic* camera = new CameraStatic(width, height, viewAngle, nearValue, farValue);

    _cameras.push_back(camera);

    return camera;
}

CameraFPS* GraphicsManager::AddCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
{
    CameraFPS* camera = new CameraFPS(width, height, viewAngle, nearValue, farValue);

    _cameras.push_back(camera);

    return camera;
}

Light* GraphicsManager::AddLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, glm::vec3 direction)
{
    Light* light = new Light(color, ambientIntensity, diffuseIntensity/*, direction*/);

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


RenderData* GraphicsManager::GetRenderData()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _cameras[0];
    renderData->light = *(_lights.begin());

    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        for (int j = 0; j < object->GetModel()->GetQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(object->GetModel(), object->GetModel()->GetMesh(j), object->getTransform(),
                                            glm::length(renderData->camera->getPosition() - object->getTransform()->GetPosition()));
            if (object->GetModel()->GetMesh(j)->material.transparency == 0.0f)
                renderData->renderList.insert(renderData->renderList.begin(), renderElement);
            else
                renderData->renderList.push_back(renderElement);
        }
    }

    return renderData;
}
