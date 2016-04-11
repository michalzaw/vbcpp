#include "SceneManager.h"


SceneManager::SceneManager(OGLDriver* driver)
    : _OGLDriver(driver), _camera(0), _light(0)
{

}


SceneManager::~SceneManager()
{
    for (int i = 0; i < _renderObjects.size(); ++i)
    {
        delete _renderObjects[i];
    }

    if (_camera != NULL)
        delete _camera;

    if (_light != NULL)
        delete _light;
}


RenderObject* SceneManager::AddRenderObject(Model* model, glm::vec3 position, glm::vec3 rotation,
                                            glm::vec3 scale, std::string name)
{
    RenderObject* object = new RenderObject(model, name);

    Transform t;
    t.SetPosition(position);
    t.SetRotation(rotation);
    t.SetScale(scale);
    object->SetTransform(t);

    _renderObjects.push_back(object);



    return object;
}

CameraStatic* SceneManager::AddCameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
{
    if ( _camera )
    {
        //delete old camera
        delete _camera;
    }

    _camera = new CameraStatic(width, height, viewAngle, nearValue, farValue);

    return _camera;
}

CameraFPS* SceneManager::AddCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
{
    if ( _camera )
    {
        //delete old camera
        delete _camera;
    }

    _camera = new CameraFPS(width, height, viewAngle, nearValue, farValue);

    return dynamic_cast<CameraFPS*>(_camera);
}

/*
CCamera* SceneManager::AddCamera(glm::vec3 position, float zNear, float zFar, float fieldOfView, float aspect)
{
    if ( !_camera )
    {
        _camera = new CCamera(position, zNear, zFar, fieldOfView, aspect);

        //return _camera;
    }

    return _camera;
}
*/

Light* SceneManager::AddLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, glm::vec3 direction)
{
    if ( !_light )
    {
        _light = new Light(color, ambientIntensity, diffuseIntensity, direction);

        //return _light;
    }

    return _light;
}


RenderData* SceneManager::GetRenderData()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _camera;
    renderData->light = _light;

    for (int i = 0; i < _renderObjects.size(); ++i)
    {
        for (int j = 0; j < _renderObjects[i]->GetModel()->GetQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(_renderObjects[i]->GetModel(), _renderObjects[i]->GetModel()->GetMesh(j), _renderObjects[i]->GetTransform(),
                                            glm::length(renderData->camera->getPosition() - _renderObjects[i]->GetTransform()->GetPosition()));
            if (_renderObjects[i]->GetModel()->GetMesh(j)->material.transparency == 0.0f)
                renderData->renderList.insert(renderData->renderList.begin(), renderElement);
            else
                renderData->renderList.push_back(renderElement);
        }
    }

    return renderData;
}
