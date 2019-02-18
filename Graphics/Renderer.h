#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <string.h>
#include <list>

#include "OGLDriver.h"
#include "RenderData.h"
#include "RShader.h"
#include "GraphicsManager.h"
#include "TreeComponent.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/RefCounter.h"

#include "../GUI/GUIObject.h"
#include "../GUI/Image.h"
#include "../GUI/Label.h"
#include "../GUI/GUIRenderListElement.h"


//#define DRAW_AABB


class Renderer
{
    private:
        bool _isInitialized;

        //OGLDriver* _OGLDriver;
        std::vector<RShader*> _shaderList;
        UBO* _lightUBO;

        std::vector<RenderData*> _renderDataList;

        unsigned int _screenWidth;
        unsigned int _screenHeight;

        bool _isShadowMappingEnable;
        ShadowMap* _shadowMap;      // Cienie wspierane tylko dla jednego swiatla keirunkowego

        void prepareLightsData();
        void prepareRenderData();

        void createRenderDatasForShadowMap(ShadowMap* shadowMap);
        void deleteRenderDatasForShadowMap(ShadowMap* shadowMap);

        Renderer();

    public:
        ~Renderer();

        static Renderer& getInstance();

        glm::vec4 grassColor;

        void init(unsigned int screenWidth, unsigned int screenHeight);

        void setIsShadowMappingEnable(bool isEnable);
        bool isShadowMappingEnable();
        void registerShadowMap(ShadowMap* shadowMap);
        void unregisterShadowMap(ShadowMap* shadowMap);

        void setCurrentMainCamera(CameraStatic* camera);

        void renderAll();
        void renderDepth(RenderData* renderData);
        void renderScene(RenderData* renderData);
        void renderGUI(GUIRenderList* renderList);//std::list<GUIObject*>* GUIObjectsList);

        RTexture* envMap;

};


#endif // RENDERER_H_INCLUDED
