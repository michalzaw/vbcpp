#ifndef GRAPHICSMANAGER_H_INCLUDED
#define GRAPHICSMANAGER_H_INCLUDED


#include <list>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "RenderObject.h"
#include "RoadObject.h"
#include "Terrain.h"
#include "RenderData.h"
#include "Camera.h"
//#include "OGLDriver.h"
#include "Light.h"
#include "QuadTree.h"
#include "Grass.h"
#include "EnvironmentCaptureComponent.h"
#include "MirrorComponent.h"
#include "Sky.h"
#include "DisplayComponent.h"
#include "CrossroadComponent.h"

#include "CameraStatic.hpp"
#include "CameraFPS.hpp"

#include "../Scene/ClickableObject.h"

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
        std::list<EnvironmentCaptureComponent*> _environmentCaptureComponents;
        std::vector<MirrorComponent*> _mirrorComponents;
        std::list<ClickableObject*> _clickableObjects;
		std::list<DisplayComponent*>_displayComponents;
		std::vector<CrossroadComponent*> _crossroadComponents;

		Sky*						_sky;

        QuadTree* _quadTree;

        CameraStatic* _currentCamera;

        glm::vec3   _windDirection;
        float       _windVelocity;
        float       _windValue;
        glm::vec3   _windVector;

        EnvironmentCaptureComponent* _globalEnvironmentCaptureComponent;

        std::list<Material*> _pendingMaterialsForMirrorComponent;

        GraphicsManager();

        void findAndAssigneMirrorComponentToPendingMaterials(MirrorComponent* mirrorComponent);

    public:
        ~GraphicsManager();

        static GraphicsManager& getInstance();


        //RenderObject*   qddRenderObject(RModel* model = NULL/*, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        //                                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)*/);

        RenderObject*   addRenderObject(RenderObject* object, SceneObject* owner); //Model* model = NULL);
		RoadObject*		addRoadObject(RRoadProfile* roadProfile, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, SceneObject* owner);
		Terrain*		addTerrain(std::string heightmapFileName, std::string dirPath, std::string materialName, float maxHeight, bool is16bitTexture, SceneObject* owner);
        Grass*          addGrassComponent(RStaticModel* model, RTexture2D* terrainHeightmap, RTexture2D* grassDensityTexture);
        CameraStatic*   addCameraStatic(CameraProjectionType projectionType = CPT_PERSPECTIVE);
        CameraFPS*      addCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue);
        Light*          addDirectionalLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity);
        Light*          addPointLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, LightAttenuation attenuation = LightAttenuation());
        Light*          addSpotLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, float cutoff, LightAttenuation attenuation = LightAttenuation());
        EnvironmentCaptureComponent* addEnvironmentCaptureComponent(RTextureCubeMap* environmentMap, RTextureCubeMap* irradianceMap = NULL, RTextureCubeMap* specularIrradianceMap = NULL);
        EnvironmentCaptureComponent* addGlobalEnvironmentCaptureComponent(RTextureCubeMap* environmentMap, RTextureCubeMap* irradianceMap = NULL, RTextureCubeMap* specularIrradianceMap = NULL);
        MirrorComponent*addMirrorComponent(std::string name, float renderingDistance);
        ClickableObject*addClickableObject();
		DisplayComponent* addDisplayComponent(RDisplayFont* font, int displayWidth, int displayHeight);
		Sky*			addSky(RTexture* texture, SceneObject* owner); // return NULL if sky exist
		CrossroadComponent* addCrossRoad(std::vector<CrossroadConnectionPoint>& connectionPoints);


        // Funkcje wywolywana przez SceneObject, nie wywolywac recznie
        void removeRenderObject(RenderObject* object);
		void removeRoadObject(RoadObject* object);
		void removeTerrain(Terrain* object);
        void removeGrassComponent(Grass* grass);
        void removeCamera(CameraStatic* camera);
        void removeLight(Light* light);
        void removeEnvironmetnCaptureComponent(EnvironmentCaptureComponent* component);
        void removeMirrorComponent(MirrorComponent* mirrorComponent);
        void removeClickableObject(ClickableObject* clickableObject);
		void removeDisplayComponent(DisplayComponent* displayComponent);
		void removeSky(Sky* sky);
		void removeCrossroadComponent(CrossroadComponent* crossroadComponent);


        void setCurrentCamera(CameraStatic* camera);
        CameraStatic* getCurrentCamera();


        void setWindDirection(glm::vec3 direction);
        void setWindVelocity(float velocity);

        glm::vec3   getWindDirection();
        float       getWindVelocity();
        glm::vec3   getWindVector();
        float       getWindValue();


        std::list<RenderObject*>& getRenderObjects();
		std::vector<CrossroadComponent*>& getCrossroadComponents();

		Sky* getSky();


        EnvironmentCaptureComponent* getGlobalEnvironmentCaptureComponent();

        // Znajdz mirror component w obiekcie i wsrod jego dzieci, name - nazwa obiektu
        MirrorComponent* findMirrorComponent(SceneObject* object, std::string name);
        // Zarejestruj material w liscie oczekujacej na nieistniejacy jeszcze MirrorComponent
        void registerPendingMaterialForMirrorComponent(Material* material);


        void clearQuadTree()
        {
            //delete _quadTree;
            //_quadTree = new QuadTree(glm::vec3(512, 512, 512));
        }


        void update(float deltaTime);


        RenderData* getRenderData();
        RenderData* getRenderDataForDepthRendering();
};


#endif // GRAPHICSMANAGER_H_INCLUDED
