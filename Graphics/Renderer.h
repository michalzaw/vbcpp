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
#include "PostProcessingEffect.h"
#include "PostProcessingBloom.h"

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

        bool _alphaToCoverage;

        float _exposure;

        bool _msaaAntialiasing;
        int _msaaAntialiasingLevel;
        Framebuffer* _defaultFramebuffer;
        VBO* _quadVBO;
		Framebuffer* _postProcessingFramebuffers[2];
        //Framebuffer* _blurFramebuffers[2];
        bool _bloom;

        //OGLDriver* _OGLDriver;
        std::vector<RShader*> _shaderList;
        std::vector<ShaderType> _shaderListForMirrorRendering;
        UBO* _lightUBO;

        std::vector<RenderData*> _renderDataList;
        std::vector<RenderData*> _renderDataListForShadowmapping;
        RenderData*              _mainRenderData;

        unsigned int _screenWidth;
        unsigned int _screenHeight;

        bool _isShadowMappingEnable;
        ShadowMap* _shadowMap;      // Cienie wspierane tylko dla jednego swiatla keirunkowego

        // Data for debug rendering
        VBO* _aabbVbo;
        std::vector<RenderObject*> _renderObjectsInCurrentFrame;
        bool _renderObjectsAAABB;
        bool _renderObjectsOBB;

        float _dayNightRatio;

		std::vector<PostProcessingEffect*> _postProcessingEffectsStack;

        void prepareLightsData();


        void addStaticModelNodeToRenderList(ModelNode* modelNode, RenderListElement& tempRenderElement, std::list<RenderListElement>& renderList, RenderPass renderPass,
                                            glm::mat4 parentTransform = glm::mat4(1.0f), glm::mat4 parentNormalMatrix = glm::mat4(1.0f));
        void addGrassStaticModelNodeToRenderList(ModelNode* modelNode, RenderListElement& tempRenderElement, std::list<RenderListElement>& renderList,
                                                 glm::mat4 parentTransform = glm::mat4(1.0f), glm::mat4 parentNormalMatrix = glm::mat4(1.0f));
        void prepareRenderData();


        bool isObjectInCamera(RenderObject* object, CameraStatic* camera);

        void createRenderDatasForShadowMap(ShadowMap* shadowMap);
        void deleteRenderDatasForShadowMap(ShadowMap* shadowMap);

        Renderer();

    public:
        int t;
        ~Renderer();

        static Renderer& getInstance();

        glm::vec4 grassColor;

        void init(unsigned int screenWidth, unsigned int screenHeight);

        void setAlphaToCoverage(bool isEnable);
        bool isAlphaToCoverageEnable();
        void setExposure(float exposure);
        float getExposure();
        void setMsaaAntialiasing(bool isEnable);
        bool isMsaaAntialiasingEnable();
        void setMsaaAntialiasingLevel(int level);
        int getMsaaAntialiasingLevel();
        void setBloom(bool isEnable);
        bool isBloomEnable();
        void setIsShadowMappingEnable(bool isEnable);
        bool isShadowMappingEnable();
        void registerShadowMap(ShadowMap* shadowMap);
        void unregisterShadowMap(ShadowMap* shadowMap);

        void setCurrentMainCamera(CameraStatic* camera);

        void setDayNightRatio(float ratio);
        float getDayNightRatio();

        void setWindowDimensions(unsigned int screenWidth, unsigned int screenHeight);
        glm::uvec2 getWindowDimensions();

        void toogleRenderAABBFlag();
        void toogleRenderOBBFlag();

        void renderAll();
        void renderDepth(RenderData* renderData);
        void renderToMirrorTexture(RenderData* renderData);
        void renderScene(RenderData* renderData);
        void renderGUI(GUIRenderList* renderList);//std::list<GUIObject*>* GUIObjectsList);

};


#endif // RENDERER_H_INCLUDED
