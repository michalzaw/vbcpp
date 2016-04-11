#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "RenderObject.h"
#include "RenderData.h"
#include "Camera.h"
#include "OGLDriver.h"
#include "Light.h"

#include "CameraStatic.hpp"
#include "CameraFPS.hpp"

class SceneManager
{
    private:
        std::vector<RenderObject*> _renderObjects;
        //CCamera* _camera;
        CameraStatic* _camera;
        Light* _light;

        OGLDriver* _OGLDriver;

    public:
        SceneManager(OGLDriver* driver);
        ~SceneManager();

        RenderObject* AddRenderObject(Model* model = NULL, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
                                      std::string name = "RenderObject");
        //CCamera* AddCamera(glm::vec3 position, float zNear, float zFar, float fieldOfView, float aspect);

        CameraStatic* AddCameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue);
        CameraFPS* AddCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue);

        Light* AddLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, glm::vec3 direction);

        RenderData* GetRenderData();
};


#endif // SCENEMANAGER_H_INCLUDED
