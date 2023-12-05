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
#include "PostProcessingToneMapping.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/RefCounter.h"

#include "../GUI/GUIObject.h"
#include "../GUI/Image.h"
#include "../GUI/Label.h"
#include "../GUI/GUIRenderListElement.h"


#define DRAW_AABB


enum UniformName
{
	UNIFORM_MVP,
	UNIFORM_VP,
	UNIFORM_MODEL_MATRIX,
	UNIFORM_NORMAL_MATRIX,
	UNIFORM_DIFFUSE_TEXTURE,
	UNIFORM_NOTMALMAP_TEXTURE,
	UNIFORM_ALPHA_TEXTURE,
	UNIFORM_ENVIRONMENTMAP_TEXTURE,
	UNIFORM_ENVIRONMENTMAP_2_TEXTURE,
	UNIFORM_ENVIRONMENTMAP_MATRIX,
	UNIFORM_ENVIRONMENTMAP_2_MATRIX,
	UNIFORM_GLASS_TEXTURE,
	UNIFORM_MATERIAL_AMBIENT_COLOR,
	UNIFORM_MATERIAL_DIFFUSE_COLOR,
	UNIFORM_MATERIAL_SPECULAR_COLOR,
	UNIFORM_MATERIAL_EMISSIVE_COLOR,
	UNIFORM_MATERIAL_SPECULAR_POWER,
	UNIFORM_MATERIAL_TRANSPARENCY,
	UNIFORM_MATERIAL_FIX_DISAPPEARANCE_ALPHA,
	UNIFORM_ALPHA_TEST_THRESHOLD,
	UNIFORM_CAMERA_POSITION,
	UNIFORM_LIGHT_SPACE_MATRIX_1,
	UNIFORM_LIGHT_SPACE_MATRIX_2,
	UNIFORM_LIGHT_SPACE_MATRIX_3,
	UNIFORM_SHADOW_MAP,
	UNIFORM_GRASS_COLOR,
	UNIFORM_HEIGHTMAP,
	UNIFORM_GRASS_DENSITY,
	UNIFORM_GRASS_MIN,
	UNIFORM_GRASS_WIDTH,
	UNIFORM_WIND_DIRECTION,
	UNIFORM_GUI_VERTICES_TRANSFORM_MATRIX,
	UNIFORM_GUI_TEXCOORDS_TRANSFORM_MATRIX,
	UNIFORM_GUI_COLOR,
	UNIFORM_DAY_NIGHT_RATIO,
	UNIFORM_DEBUG_VERTEX_INDEX_1,
	UNIFORM_DEBUG_VERTEX_INDEX_2,
	UNIFORM_DEBUG_VERTEX_INDEX_3,
	UNIFORM_DEBUG_VERTEX_INDEX_4,
	UNIFORM_DEBUG_VERTEX_INDEX_5,
	UNIFORM_DEBUG_VERTEX_INDEX_6,
	UNIFORM_DEBUG_VERTEX_INDEX_7,
	UNIFORM_DEBUG_VERTEX_INDEX_8,
	UNIFORM_EMISSIVE_TEXTURE,
	UNIFORM_OBJECT_ID,

	UNIFORM_ALBEDO_TEXTURE,
	UNIFORM_METALIC_TEXTURE,
	UNIFORM_ROUGHNESS_TEXTURE,
	UNIFORM_AO_TEXTURE,
	UNIFORM_IRRADIANCE_MAP,
	UNIFORM_SPECULAR_IRRADIANCE_MAP,
	UNIFORM_BRDF_LUT,

	UNIFORM_COLOR_1,
	UNIFORM_COLOR_2,
	UNIFORM_COLOR_3,
	UNIFORM_COLOR_4,

	UNIFORM_GRASS_TEXTURE_1,
	UNIFORM_GRASS_TEXTURE_2,
	UNIFORM_GRASS_TEXTURE_3,
	UNIFORM_GRASS_TEXTURE_4,
	UNIFORM_GRASS_TEXTURE_5,
	UNIFORM_GRASS_TEXTURE_6,
	UNIFORM_GRASS_TEXTURE_7,

	UNIFORM_GRASS_TEXTURE_SCALE_1,
	UNIFORM_GRASS_TEXTURE_SCALE_2,
	UNIFORM_GRASS_TEXTURE_SCALE_3,
	UNIFORM_GRASS_TEXTURE_SCALE_4,
	UNIFORM_GRASS_TEXTURE_SCALE_5,
	UNIFORM_GRASS_TEXTURE_SCALE_6,
	UNIFORM_GRASS_TEXTURE_SCALE_7,

	UNIFORM_GRASS_TEXTURES_COUNT,
	UNIFORM_GRASS_WIND_TIMER,

	NUMBER_OF_UNIFORMS
};


class Renderer
{
    private:
        bool _isInitialized;

		GraphicsManager* _graphicsManager;

        bool _alphaToCoverage;

        float _exposure;
		ToneMappingType _toneMappingType;

		float _sceneVisibility;

		TextureFormat _framebufferTextureFormat;
        bool _msaaAntialiasing;
        int _msaaAntialiasingLevel;

		bool _fxaa;

        bool _bloom;

		bool _renderObjectIdsForPicking;

		Framebuffer* _defaultFramebuffer;
		Framebuffer* _depthFramebuffer;

        //OGLDriver* _OGLDriver;
        std::vector<RShader*> _shaderList;
        std::vector<ShaderType> _shaderListForMirrorRendering;
        UBO* _lightUBO;
		const char* _uniformsNames[NUMBER_OF_UNIFORMS];
		GLint _uniformsLocations[NUMBER_OF_SHADERS][NUMBER_OF_UNIFORMS];

        std::vector<RenderData*> _renderDataList;
        std::vector<RenderData*> _renderDataListForShadowmapping;
		std::vector<RenderData*> _renderDataListForStaticShadowmapping;
        RenderData*              _mainRenderData;

        unsigned int _screenWidth;
        unsigned int _screenHeight;

        bool _isShadowMappingEnable;
        ShadowMap* _shadowMap;      // Cienie wspierane tylko dla jednego swiatla keirunkowego
		float _shadowCameraFrustumDiagonal;
		bool _shadowCameraFrustumDiagonalIsCalculated;

        // Data for debug rendering
        VBO* _aabbVbo;
        std::vector<RenderObject*> _renderObjectsInCurrentFrame;
        bool _renderObjectsAAABB;
        bool _renderObjectsOBB;

        float _dayNightRatio;

		// post processing
		std::vector<PostProcessingEffect*> _postProcessingEffectsStack;
		VBO* _quadVBO;
		Framebuffer* _postProcessingFramebuffers[2];


		RTexture2D* _brdfLutTexture;

		bool _requiredRebuildStaticLighting;


		unsigned int* _objectsIdsTextureData;
		RShader* _pickingComputeShader;
		ShaderStorageBuffer* _pickingSSBO;


		void addPostProcessingEffect(PostProcessingEffect* postProcessingEffect);
		void removePostProcessingEffect(PostProcessingType type);

		PostProcessingBloom* createBloomPostProcessing();
		PostProcessingEffect* createFxaaPostPorcessing();

		void createFramebuffersForPostProcessing();
		void initPostProcessingEffectsStack();


		void recreateAllFramebuffers();


		void initUniformLocations();


        void prepareLightsData();


        void addStaticModelNodeToRenderList(ModelNode* modelNode, RenderListElement& tempRenderElement, std::list<RenderListElement>& renderList, RenderPass renderPass, int lod,
                                            glm::mat4 parentTransform = glm::mat4(1.0f), glm::mat4 parentNormalMatrix = glm::mat4(1.0f));
        void addGrassStaticModelNodeToRenderList(ModelNode* modelNode, RenderListElement& tempRenderElement, std::list<RenderListElement>& renderList,
                                                 glm::mat4 parentTransform = glm::mat4(1.0f), glm::mat4 parentNormalMatrix = glm::mat4(1.0f));
        void prepareRenderData();
		void prepareRenderDataForStaticShadowmaps();


        bool isObjectInCamera(RenderObject* object, CameraStatic* camera);

        void createRenderDatasForShadowMap(ShadowMap* shadowMap);
        void deleteRenderDatasForShadowMap(ShadowMap* shadowMap);
		void clearRenderDatasForShadowMap();

		const std::vector<glm::mat4>& getFinalMatrices(SceneObject* sceneObject);


		void rebuildStaticLightingInternal();


		void recreateObjectsIdsTextureData();

        Renderer();

    public:
		glm::vec3 color1;
		glm::vec3 color2;
		glm::vec3 color3;
		glm::vec3 color4;
        int t;
        ~Renderer();

        static Renderer& getInstance();

        glm::vec4 grassColor;

        void init(unsigned int screenWidth, unsigned int screenHeight);

		void setGraphicsManager(GraphicsManager* graphicsManager);

        void setAlphaToCoverage(bool isEnable);
        bool isAlphaToCoverageEnable();
        void setExposure(float exposure);
        float getExposure();
		void setSceneVisibility(float visibility);
		float getSceneVisibility();
		void setToneMappingType(ToneMappingType type);
		ToneMappingType getToneMappingType();
		void setFramebufferTextureFormat(TextureFormat format);
		TextureFormat getFramebufferTextureFormat();
        void setMsaaAntialiasing(bool isEnable);
        bool isMsaaAntialiasingEnable();
        void setMsaaAntialiasingLevel(int level);
        int getMsaaAntialiasingLevel();
		void setFxaa(bool isEnable);
		bool isFxaaEnabled();
        void setBloom(bool isEnable);
        bool isBloomEnable();
		void setRenderObjectIdsForPicking(bool isEnabled);
		bool isRenderObjectIdsForPickingEnabled();
        void setIsShadowMappingEnable(bool isEnable);
        bool isShadowMappingEnable();
        void registerShadowMap(ShadowMap* shadowMap);
        void unregisterShadowMap(ShadowMap* shadowMap);

        void setCurrentMainCamera(CameraStatic* camera);
		CameraStatic* getCurrentMainCamera();

        void setDayNightRatio(float ratio);
        float getDayNightRatio();

        void setWindowDimensions(unsigned int screenWidth, unsigned int screenHeight);
        glm::uvec2 getWindowDimensions();

        void toogleRenderAABBFlag();
        void toogleRenderOBBFlag();
		bool getAABBFlag();
		bool getOBBFlag();

		VBO* getQuadVbo();

		PostProcessingEffect* findEffect(PostProcessingType type);

		void bakeStaticShadows();
		void rebuildStaticLighting();

        void renderAll();
        void renderDepth(RenderData* renderData);
        void renderToMirrorTexture(RenderData* renderData);
        void renderScene(RenderData* renderData);
        void renderGUI(GUIRenderList* renderList);//std::list<GUIObject*>* GUIObjectsList);

		unsigned int pickObject(int x, int y);

};


#endif // RENDERER_H_INCLUDED
