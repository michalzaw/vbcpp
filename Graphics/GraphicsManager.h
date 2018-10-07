#ifndef GRAPHICSMANAGER_H_INCLUDED
#define GRAPHICSMANAGER_H_INCLUDED


#include <list>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "RenderObject.h"
#include "RenderData.h"
#include "Camera.h"
//#include "OGLDriver.h"
#include "Light.h"
#include "QuadTree.h"
#include "Grass.h"

#include "CameraStatic.hpp"
#include "CameraFPS.hpp"

#include "../Utils/Collision.h"
#include "../Utils/RModel.h"


//#define ALL_OBJECTS
//#define FRUSTUM_CULLING
//#define QUAD_TREE

class GraphicsManager
{
    friend class Renderer;

    private:
        std::list<RenderObject*>    _renderObjects;
        std::list<Grass*>           _grassComponents;
        std::vector<CameraStatic*>  _cameras;
        std::list<Light*>           _lights;

        QuadTree* _quadTree;

        CameraStatic* _currentCamera;

        glm::vec3   _windDirection;
        float       _windVelocity;
        float       _windValue;
        glm::vec3   _windVector;

        GraphicsManager();

    public:
        ~GraphicsManager();

        static GraphicsManager& getInstance();


        //RenderObject*   qddRenderObject(RModel* model = NULL/*, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        //                                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)*/);

        RenderObject*   addRenderObject(RenderObject* object, SceneObject* owner); //Model* model = NULL);
        Grass*          addGrassComponent(RStaticModel* model, RTexture2D* terrainHeightmap, RTexture2D* grassDensityTexture);
        CameraStatic*   addCameraStatic(CameraProjectionType projectionType = CPT_PERSPECTIVE);
        CameraFPS*      addCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue);
        Light*          addDirectionalLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity);
        Light*          addPointLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, LightAttenuation attenuation = LightAttenuation());
        Light*          addSpotLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, float cutoff, LightAttenuation attenuation = LightAttenuation());


        // Funkcje wywolywana przez SceneObject, nie wywolywac recznie
        void removeRenderObject(RenderObject* object);
        void removeGrassComponent(Grass* grass);
        void removeCamera(CameraStatic* camera);
        void removeLight(Light* light);


        void setCurrentCamera(CameraStatic* camera);
        CameraStatic* getCurrentCamera();


        void setWindDirection(glm::vec3 direction);
        void setWindVelocity(float velocity);

        glm::vec3   getWindDirection();
        float       getWindVelocity();
        glm::vec3   getWindVector();
        float       getWindValue();


        void update(float deltaTime);


        RenderData* getRenderData();
        RenderData* getRenderDataForDepthRendering();
};


#endif // GRAPHICSMANAGER_H_INCLUDED
