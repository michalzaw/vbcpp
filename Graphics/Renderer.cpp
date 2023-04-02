#include "Renderer.h"

#include "SkeletalAnimationComponent.h"
#include "SkeletalAnimationComponent2.h"
#include "StorageBuffer/PickingShaderStorageBufferData.h"

#include "../Utils/Helpers.hpp"
#include "../Utils/Logger.h"
#include "../Utils/Timer.h"


//#define OBJECT_PICKING_GET_TEX_IMAGE
#define OBJECT_PICKING_STORAGE_BUFFER


static std::unique_ptr<Renderer> rendererInstance;


Renderer::Renderer()
    : _isInitialized(false),
    _graphicsManager(nullptr),
    _screenWidth(0), _screenHeight(0),
    _alphaToCoverage(true), _exposure(0.05f), _toneMappingType(TMT_CLASSIC), _sceneVisibility(1.0f),
	_framebufferTextureFormat(TF_RGBA_16F), _msaaAntialiasing(false), _msaaAntialiasingLevel(8),
    _bloom(false),
    _renderObjectIdsForPicking(false),
	_isShadowMappingEnable(false), _shadowMap(NULL), _shadowCameraFrustumDiagonalIsCalculated(false),
    _mainRenderData(NULL),
    _renderObjectsAAABB(false), _renderObjectsOBB(false),
	//color1(1.0f, 1.0f, 1.0f), color2(1.0f, 1.0f, 1.0f), color3(1.0f, 1.0f, 1.0f), color4(1.0f, 1.0f, 1.0f)
	color1(0.733f, 0.769f, 0.475f), color2(0.773f, 0.804f, 0.537f), color3(1.0f, 1.0f, 1.0f), color4(1.0f, 1.0f, 1.0f),
    _requiredRebuildStaticLighting(false),
    _objectsIdsTextureData(nullptr), _pickingComputeShader(nullptr), _pickingSSBO(nullptr)
{
    float indices[24] = {0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 1, 5, 3, 7, 2, 6, 0, 4};

    _aabbVbo = OGLDriver::getInstance().createVBO(24 * sizeof(float));
    _aabbVbo->addVertexData(indices, 24);

    float quadVertices[] =
    {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
    };
    _quadVBO = OGLDriver::getInstance().createVBO(4 * 2 * sizeof(float));
    _quadVBO->addVertexData(quadVertices, 4 * 2);

	_brdfLutTexture = ResourceManager::getInstance().loadTexture("brdfLut.png", false);
}

Renderer::~Renderer()
{
    OGLDriver::getInstance().deleteUBO(_lightUBO);

    if (_aabbVbo)
    {
        OGLDriver::getInstance().deleteVBO(_aabbVbo);
    }

    OGLDriver::getInstance().deleteFramebuffer(_mainRenderData->framebuffer);

    delete _mainRenderData;

	for (int i = 0; i < _postProcessingEffectsStack.size(); ++i)
	{
		delete _postProcessingEffectsStack[i];
	}

	OGLDriver::getInstance().deleteVBO(_quadVBO);
	OGLDriver::getInstance().deleteFramebuffer(_postProcessingFramebuffers[0]);
	OGLDriver::getInstance().deleteFramebuffer(_postProcessingFramebuffers[1]);

    if (_objectsIdsTextureData != nullptr)
    {
        delete _objectsIdsTextureData;
    }

    if (_pickingSSBO != nullptr)
    {
        OGLDriver::getInstance().deleteShaderStorageBuffer(_pickingSSBO);
    }
}


Renderer& Renderer::getInstance()
{
    if ( !rendererInstance )
        rendererInstance = std::unique_ptr<Renderer>(new Renderer);

    return *rendererInstance;
}


bool compareByShader(const RenderListElement& a, const RenderListElement& b)
{
    return a.material->shader < b.material->shader;
}


// obiekty ktore nie renderuja swojego id trafiaja na koniec
bool compareByShaderAndObjectId(const RenderListElement& a, const RenderListElement& b)
{
    return a.material->shader + (a.renderObject->isRenderObjectId() ? 0 : 100) < b.material->shader + (b.renderObject->isRenderObjectId() ? 0 : 100);
}


void Renderer::addPostProcessingEffect(PostProcessingEffect* postProcessingEffect)
{
	for (std::vector<PostProcessingEffect*>::iterator i = _postProcessingEffectsStack.begin(); i != _postProcessingEffectsStack.end(); ++i)
	{
		if (postProcessingEffect->getType() < (*i)->getType())
		{
			_postProcessingEffectsStack.insert(i, postProcessingEffect);
			return;
		}
	}

	_postProcessingEffectsStack.push_back(postProcessingEffect);
}


void Renderer::removePostProcessingEffect(PostProcessingType type)
{
	for (std::vector<PostProcessingEffect*>::iterator i = _postProcessingEffectsStack.begin(); i != _postProcessingEffectsStack.end(); ++i)
	{
		if (type == (*i)->getType())
		{
			delete *i;
			_postProcessingEffectsStack.erase(i);

			break;
		}
	}
}


PostProcessingEffect* Renderer::findEffect(PostProcessingType type)
{
	for (int i = 0; i < _postProcessingEffectsStack.size(); ++i)
	{
		if (_postProcessingEffectsStack[i]->getType() == type)
		{
			return _postProcessingEffectsStack[i];
		}
	}

	return NULL;
}


PostProcessingBloom* Renderer::createBloomPostProcessing()
{
	return new PostProcessingBloom(_quadVBO, _screenWidth, _screenHeight, _mainRenderData->framebuffer->getTexture(1), _msaaAntialiasing, _msaaAntialiasingLevel);
}


void Renderer::createFramebuffersForPostProcessing()
{
	for (int i = 0; i < 2; ++i)
	{
		_postProcessingFramebuffers[i] = OGLDriver::getInstance().createFramebuffer();
		_postProcessingFramebuffers[i]->addTexture(_framebufferTextureFormat, _screenWidth, _screenHeight, false);
		_postProcessingFramebuffers[i]->setTextureFiltering(0, TFM_LINEAR, TFM_LINEAR);
        _postProcessingFramebuffers[i]->init();
	}
}


void Renderer::initPostProcessingEffectsStack()
{
	// msaa
	if (_msaaAntialiasing)
	{
		std::vector<std::string> defines;
		std::unordered_map<std::string, std::string> constants;
		constants["samplesCount"] = toString(_msaaAntialiasingLevel);

		RShader* shader = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingMsaa.frag", defines, constants);

		PostProcessingEffect* postProcessingMsaa = new PostProcessingEffect(PPT_MSAA, _quadVBO, shader);
		addPostProcessingEffect(postProcessingMsaa);
	}

	// bloom
	if (_bloom)
	{
		PostProcessingBloom* postProcessingBloom = createBloomPostProcessing();
		addPostProcessingEffect(postProcessingBloom);
	}

	// tone mapping
	PostProcessingToneMapping* postProcessingToneMapping = new PostProcessingToneMapping(_quadVBO, _toneMappingType);
	postProcessingToneMapping->setExposure(_exposure);
	addPostProcessingEffect(postProcessingToneMapping);
}


void Renderer::recreateAllFramebuffers()
{
	OGLDriver::getInstance().deleteFramebuffer(_mainRenderData->framebuffer);
	OGLDriver::getInstance().deleteFramebuffer(_postProcessingFramebuffers[0]);
	OGLDriver::getInstance().deleteFramebuffer(_postProcessingFramebuffers[1]);

	Framebuffer* framebuffer = OGLDriver::getInstance().createFramebuffer();
	framebuffer->addDepthRenderbuffer(_screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
	framebuffer->addTexture(_framebufferTextureFormat, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
	framebuffer->addTexture(_framebufferTextureFormat, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);

    if (_renderObjectIdsForPicking)
    {
        framebuffer->addTexture(TF_RED_32, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
        recreateObjectsIdsTextureData();
    }

    framebuffer->init();
	//framebuffer->getTexture(0)->setFiltering(TFM_NEAREST, TFM_NEAREST);
	//framebuffer->getTexture(1)->setFiltering(TFM_NEAREST, TFM_NEAREST);
	_mainRenderData->framebuffer = framebuffer;

	createFramebuffersForPostProcessing();
}


void Renderer::initUniformLocations()
{
	_uniformsNames[UNIFORM_MVP] = "MVP";
	_uniformsNames[UNIFORM_VP] = "VP";
	_uniformsNames[UNIFORM_MODEL_MATRIX] = "ModelMatrix";
	_uniformsNames[UNIFORM_NORMAL_MATRIX] = "NormalMatrix";
	_uniformsNames[UNIFORM_DIFFUSE_TEXTURE] = "Texture";
	_uniformsNames[UNIFORM_NOTMALMAP_TEXTURE] = "NormalmapTexture";
	_uniformsNames[UNIFORM_ALPHA_TEXTURE] = "AlphaTexture";
	_uniformsNames[UNIFORM_ENVIRONMENTMAP_TEXTURE] = "environmentMap";
	_uniformsNames[UNIFORM_ENVIRONMENTMAP_2_TEXTURE] = "environmentMap2";
	_uniformsNames[UNIFORM_ENVIRONMENTMAP_MATRIX] = "environmentMap1Rotation";
	_uniformsNames[UNIFORM_ENVIRONMENTMAP_2_MATRIX] = "environmentMap2Rotation";
	_uniformsNames[UNIFORM_GLASS_TEXTURE] = "glassTexture";
	_uniformsNames[UNIFORM_MATERIAL_AMBIENT_COLOR] = "matAmbient";
	_uniformsNames[UNIFORM_MATERIAL_DIFFUSE_COLOR] = "matDiffuse";
	_uniformsNames[UNIFORM_MATERIAL_SPECULAR_COLOR] = "matSpecular";
	_uniformsNames[UNIFORM_MATERIAL_EMISSIVE_COLOR] = "matEmissive";
	_uniformsNames[UNIFORM_MATERIAL_SPECULAR_POWER] = "SpecularPower";
	_uniformsNames[UNIFORM_MATERIAL_TRANSPARENCY] = "Transparency";
	_uniformsNames[UNIFORM_MATERIAL_FIX_DISAPPEARANCE_ALPHA] = "fixDisappearanceAlphaRatio";
	_uniformsNames[UNIFORM_ALPHA_TEST_THRESHOLD] = "alphaTestThreshold";
	_uniformsNames[UNIFORM_CAMERA_POSITION] = "CameraPosition";
	_uniformsNames[UNIFORM_LIGHT_SPACE_MATRIX_1] = "LightSpaceMatrix[0]";
	_uniformsNames[UNIFORM_LIGHT_SPACE_MATRIX_2] = "LightSpaceMatrix[1]";
	_uniformsNames[UNIFORM_LIGHT_SPACE_MATRIX_3] = "LightSpaceMatrix[2]";
	_uniformsNames[UNIFORM_SHADOW_MAP] = "ShadowMap";
	_uniformsNames[UNIFORM_GRASS_COLOR] = "grassColor";
	_uniformsNames[UNIFORM_HEIGHTMAP] = "heightmap";
	_uniformsNames[UNIFORM_GRASS_DENSITY] = "grassDensity";
	_uniformsNames[UNIFORM_GRASS_MIN] = "min";
	_uniformsNames[UNIFORM_GRASS_WIDTH] = "width";
	_uniformsNames[UNIFORM_WIND_DIRECTION] = "d";
	_uniformsNames[UNIFORM_GUI_VERTICES_TRANSFORM_MATRIX] = "VerticesTransformMatrix";
	_uniformsNames[UNIFORM_GUI_TEXCOORDS_TRANSFORM_MATRIX] = "TexCoordTransformMatrix";
	_uniformsNames[UNIFORM_GUI_COLOR] = "color";
	_uniformsNames[UNIFORM_DAY_NIGHT_RATIO] = "dayNightRatio";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_1] = "indices[0]";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_2] = "indices[1]";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_3] = "indices[2]";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_4] = "indices[3]";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_5] = "indices[4]";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_6] = "indices[5]";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_7] = "indices[6]";
	_uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_8] = "indices[7]";
	_uniformsNames[UNIFORM_EMISSIVE_TEXTURE] = "emissiveTexture";
    _uniformsNames[UNIFORM_OBJECT_ID] = "objectId";

	_uniformsNames[UNIFORM_ALBEDO_TEXTURE] = "AlbedoTexture";
	_uniformsNames[UNIFORM_METALIC_TEXTURE] = "MetalicTexture";
	_uniformsNames[UNIFORM_ROUGHNESS_TEXTURE] = "RoughnessTexture";
	_uniformsNames[UNIFORM_AO_TEXTURE] = "AoTexture";
	_uniformsNames[UNIFORM_IRRADIANCE_MAP] = "IrradianceMap";
	_uniformsNames[UNIFORM_SPECULAR_IRRADIANCE_MAP] = "SpecularIrradianceMap";
	_uniformsNames[UNIFORM_BRDF_LUT] = "brdfLUT";

	_uniformsNames[UNIFORM_COLOR_1] = "color1";
	_uniformsNames[UNIFORM_COLOR_2] = "color2";
	_uniformsNames[UNIFORM_COLOR_3] = "color3";
	_uniformsNames[UNIFORM_COLOR_4] = "color4";

	_uniformsNames[UNIFORM_GRASS_TEXTURE_1] = "t[0]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_2] = "t[1]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_3] = "t[2]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_4] = "t[3]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_5] = "t[4]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_6] = "t[5]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_7] = "t[6]";

	_uniformsNames[UNIFORM_GRASS_TEXTURE_SCALE_1] = "tScale[0]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_SCALE_2] = "tScale[1]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_SCALE_3] = "tScale[2]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_SCALE_4] = "tScale[3]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_SCALE_5] = "tScale[4]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_SCALE_6] = "tScale[5]";
	_uniformsNames[UNIFORM_GRASS_TEXTURE_SCALE_7] = "tScale[6]";

	_uniformsNames[UNIFORM_GRASS_TEXTURES_COUNT] = "grassTexturesCount";
	_uniformsNames[UNIFORM_GRASS_WIND_TIMER] = "time";

	for (int i = 0; i < NUMBER_OF_SHADERS; ++i)
	{
		for (int j = 0; j < NUMBER_OF_UNIFORMS; ++j)
		{
			_uniformsLocations[i][j] = _shaderList[i]->getUniformLocation(_uniformsNames[j]);
		}
	}
}


void Renderer::prepareLightsData()
{
    // Aktualizacja UBO swiatel
    int d = 0;
    int p = 0;
    int s = 0;
    for (std::list<Light*>::iterator i = _graphicsManager->_lights.begin(); i != _graphicsManager->_lights.end(); ++i)
    {
        if (!(*i)->isActive())
            continue;

        switch ((*i)->getLightType())
        {
            case LT_DIRECTIONAL:
            {
                std::string lname = "LightsBlock.DirLights[";
                lname = lname + toString(d++) + "]";

                const char* names[4];

                std::string a = lname + ".Base.Color";
                names[0] = a.c_str();
                std::string b = lname + ".Base.AmbientIntensity";
                names[1] = b.c_str();
                std::string c = (lname + ".Base.DiffuseIntensity");
                names[2] = c.c_str();
                std::string d = (lname + ".Direction");
                names[3] = d.c_str();

                GLint offsets[4];

                _shaderList[SOLID_MATERIAL]->getUniformOffset(names, 4, offsets);

                _lightUBO->setUniform(offsets[0],                        (*i)->getColor().r);
                _lightUBO->setUniform(offsets[0] + sizeof(float),        (*i)->getColor().g);
                _lightUBO->setUniform(offsets[0] + 2 * sizeof(float),    (*i)->getColor().b);

                _lightUBO->setUniform(offsets[1], (*i)->getAmbientIntensity());

                _lightUBO->setUniform(offsets[2], (*i)->getDiffiseIntenisty());

                _lightUBO->setUniform(offsets[3],                        (*i)->getDirection().x);
                _lightUBO->setUniform(offsets[3] + sizeof(float),        (*i)->getDirection().y);
                _lightUBO->setUniform(offsets[3] + 2 * sizeof(float),    (*i)->getDirection().z);

                break;
            }
            case LT_POINT:
            {
                std::string lname = "LightsBlock.PointLights[";
                lname = lname + toString(p++) + "]";

                const char* names[7];
                std::string s1 = lname + ".Base.Color";
                names[0] = s1.c_str();
                std::string s2 = lname + ".Base.AmbientIntensity";
                names[1] = s2.c_str();
                std::string s3 = lname + ".Base.DiffuseIntensity";
                names[2] = s3.c_str();
                std::string s4 = lname + ".Position";
                names[3] = s4.c_str();
                std::string s5 = lname + ".Attenuation.constant";
                names[4] = s5.c_str();
                std::string s6 = lname + ".Attenuation.linear";
                names[5] = s6.c_str();
                std::string s7 = lname + ".Attenuation.exp";
                names[6] = s7.c_str();

                GLint offsets[7];

                _shaderList[SOLID_MATERIAL]->getUniformOffset(names, 7, offsets);

                _lightUBO->setUniform(offsets[0],                        (*i)->getColor().r);
                _lightUBO->setUniform(offsets[0] + sizeof(float),        (*i)->getColor().g);
                _lightUBO->setUniform(offsets[0] + 2 * sizeof(float),    (*i)->getColor().b);

                _lightUBO->setUniform(offsets[1], (*i)->getAmbientIntensity());

                _lightUBO->setUniform(offsets[2], (*i)->getDiffiseIntenisty());

                _lightUBO->setUniform(offsets[3],                        (*i)->getPosition().x);
                _lightUBO->setUniform(offsets[3] + sizeof(float),        (*i)->getPosition().y);
                _lightUBO->setUniform(offsets[3] + 2 * sizeof(float),    (*i)->getPosition().z);

                _lightUBO->setUniform(offsets[4], (*i)->getAttenuation().constant);
                _lightUBO->setUniform(offsets[5], (*i)->getAttenuation().linear);
                _lightUBO->setUniform(offsets[6], (*i)->getAttenuation().exp);

                break;
            }
            case LT_SPOT:
            {
                std::string lname = "LightsBlock.SpotLights[";
                lname = lname + toString(s++) + "]";

                const char* names[9];
                std::string s1 = lname + ".Base.Base.Color";
                names[0] = s1.c_str();
                std::string s2 = lname + ".Base.Base.AmbientIntensity";
                names[1] = s2.c_str();
                std::string s3 = lname + ".Base.Base.DiffuseIntensity";
                names[2] = s3.c_str();
                std::string s4 = lname + ".Base.Position";
                names[3] = s4.c_str();
                std::string s5 = lname + ".Base.Attenuation.constant";
                names[4] = s5.c_str();
                std::string s6 = lname + ".Base.Attenuation.linear";
                names[5] = s6.c_str();
                std::string s7 = lname + ".Base.Attenuation.exp";
                names[6] = s7.c_str();
                std::string s8 = lname + ".Direction";
                names[7] = s8.c_str();
                std::string s9 = lname + ".CutoffCos";
                names[8] = s9.c_str();

                GLint offsets[9];

                _shaderList[SOLID_MATERIAL]->getUniformOffset(names, 9, offsets);

                _lightUBO->setUniform(offsets[0],                        (*i)->getColor().r);
                _lightUBO->setUniform(offsets[0] + sizeof(float),        (*i)->getColor().g);
                _lightUBO->setUniform(offsets[0] + 2 * sizeof(float),    (*i)->getColor().b);

                _lightUBO->setUniform(offsets[1], (*i)->getAmbientIntensity());

                _lightUBO->setUniform(offsets[2], (*i)->getDiffiseIntenisty());

                _lightUBO->setUniform(offsets[3],                        (*i)->getPosition().x);
                _lightUBO->setUniform(offsets[3] + sizeof(float),        (*i)->getPosition().y);
                _lightUBO->setUniform(offsets[3] + 2 * sizeof(float),    (*i)->getPosition().z);

                _lightUBO->setUniform(offsets[4], (*i)->getAttenuation().constant);
                _lightUBO->setUniform(offsets[5], (*i)->getAttenuation().linear);
                _lightUBO->setUniform(offsets[6], (*i)->getAttenuation().exp);

                _lightUBO->setUniform(offsets[7],                        (*i)->getDirection().x);
                _lightUBO->setUniform(offsets[7] + sizeof(float),        (*i)->getDirection().y);
                _lightUBO->setUniform(offsets[7] + 2 * sizeof(float),    (*i)->getDirection().z);

                _lightUBO->setUniform(offsets[8], (*i)->getCutoffCos());

                break;
            }
        }

    }

    const char* names[3] = { "LightsBlock.DirCount", "LightsBlock.PointCount", "LightsBlock.SpotCount" };
    GLint offsets[3];
    _shaderList[SOLID_MATERIAL]->getUniformOffset(names, 3, offsets);

    _lightUBO->setUniform(offsets[0], d);
    _lightUBO->setUniform(offsets[1], p);
    _lightUBO->setUniform(offsets[2], s);


    // Shadowmaping
    if (_isShadowMappingEnable && _shadowMap != NULL)
    {
        float cascadeEnd[4] = {0.1, 25.0f, 100.0f, 500.0f };

		for (int i = 0; i < ShadowMap::CASCADE_COUNT - 1; ++i)
		{
			CameraStatic* cameraForShadowMap = _shadowMap->getCameraForShadowMap(i);
			CameraStatic* currentCamera = _graphicsManager->getCurrentCamera();

			Frustum frustum(glm::perspective(currentCamera->getViewAngle(), (float)currentCamera->getWindowWidth() / (float)currentCamera->getWindowHeight(),
				cascadeEnd[i], cascadeEnd[i + 1]) * currentCamera->getViewMatrix());

			frustum.applyTransform(cameraForShadowMap->getViewMatrix());
			AABB* frustumAabb = frustum.getAABB();
			glm::vec3 min = frustumAabb->getMinCoords();
			glm::vec3 max = frustumAabb->getMaxCoords();

			// shadows stabilizations

			if (!_shadowCameraFrustumDiagonalIsCalculated) {
				_shadowCameraFrustumDiagonal = std::max(glm::length(frustum.getPoints()[FP_FAR_LEFT_BOTTOM] - frustum.getPoints()[FP_FAR_RIGHT_TOP]),
					glm::length(frustum.getPoints()[FP_NEAR_LEFT_BOTTOM] - frustum.getPoints()[FP_FAR_RIGHT_TOP]));
				_shadowCameraFrustumDiagonalIsCalculated = true;
			}
			glm::vec3 offset = (glm::vec3(_shadowCameraFrustumDiagonal) - (max - min)) * vec3(0.5f);
			offset.z = 0.0f;

			max += offset;
			min -= offset;

			float worldUnitsPerTexel = _shadowCameraFrustumDiagonal / _shadowMap->getShadowMapFramebuffer()->getTexture(0)->getSize().x;
			glm::vec3 worldUnitsPerTexelVector = glm::vec3(worldUnitsPerTexel, worldUnitsPerTexel, 0.0f);

			min.x /= worldUnitsPerTexel;
			min.x = floor(min.x);
			min.x *= worldUnitsPerTexel;

			min.y /= worldUnitsPerTexel;
			min.y = floor(min.y);
			min.y *= worldUnitsPerTexel;

			max.x /= worldUnitsPerTexel;
			max.x = floor(max.x);
			max.x *= worldUnitsPerTexel;

			max.y /= worldUnitsPerTexel;
			max.y = floor(max.y);
			max.y *= worldUnitsPerTexel;

            cameraForShadowMap->setOrthoProjectionParams(min.x, max.x, min.y, max.y, -max.z - 100.0f, -min.z);
        }
    }
}


void Renderer::addStaticModelNodeToRenderList(ModelNode* modelNode, RenderListElement& tempRenderElement, std::list<RenderListElement>& renderList, RenderPass renderPass, int lod, glm::mat4 parentTransform, glm::mat4 parentNormalMatrix)
{
    glm::mat4 t = parentTransform * modelNode->getTransformMatrix();
    glm::mat4 n = parentNormalMatrix * modelNode->getNormalMatrix();
    tempRenderElement.transformMatrix = t;
    tempRenderElement.normalMatrix = n;

    for (int k = 0; k < modelNode->getMeshesCount(); ++k)
    {
        tempRenderElement.mesh = modelNode->getMesh(k);
        tempRenderElement.material = tempRenderElement.mesh->material;
        //tempRenderElement.material = tempRenderElement.model->getMaterial(tempRenderElement.mesh->materialIndex);
        if (renderPass == RP_SHADOWS)
        {
            if (tempRenderElement.material->shader == ALPHA_TEST_MATERIAL || tempRenderElement.material->shader == TREE_MATERIAL)
                renderList.insert(renderList.begin(), tempRenderElement);
            else if (tempRenderElement.material->transparency == 0.0f) // czy materia³ nie jest przezroczysty lub skybox itd.
                renderList.push_back(tempRenderElement);
        }
        else
        {
            renderList.push_back(tempRenderElement);
        }
    }

    for (int i = 0; i < modelNode->getChildrenCount(); ++i)
    {
        addStaticModelNodeToRenderList(modelNode->getChildren()[i], tempRenderElement, renderList, renderPass, lod, t, n);
    }
}


void Renderer::addGrassStaticModelNodeToRenderList(ModelNode* modelNode, RenderListElement& tempRenderElement, std::list<RenderListElement>& renderList, glm::mat4 parentTransform, glm::mat4 parentNormalMatrix)
{
    glm::mat4 t = parentTransform * modelNode->getTransformMatrix();
    glm::mat4 n = parentNormalMatrix * modelNode->getNormalMatrix();
    tempRenderElement.transformMatrix = t;
    tempRenderElement.normalMatrix = n;

    for (int j = 0; j < modelNode->getMeshesCount(); ++j)
    {
        tempRenderElement.mesh = modelNode->getMesh(j);
        tempRenderElement.material = tempRenderElement.mesh->material;
        //tempRenderElement.material = tempRenderElement.model->getMaterial(tempRenderElement.mesh->materialIndex);
        renderList.push_back(tempRenderElement);
    }

    for (int i = 0; i < modelNode->getChildrenCount(); ++i)
    {
        addGrassStaticModelNodeToRenderList(modelNode->getChildren()[i], tempRenderElement, renderList, t, n);
    }
}


void Renderer::prepareRenderData()
{
    for (int i = 0; i < _renderDataList.size(); ++i)
    {
        if ( _renderDataList[i] != _mainRenderData && !isVectorContains(_renderDataListForShadowmapping, _renderDataList[i]))
        {
            delete _renderDataList[i];
        }
    }


    _renderDataList.clear();
    for (int i = 0; i < _renderDataListForShadowmapping.size(); ++i)
    {
        _renderDataList.push_back(_renderDataListForShadowmapping[i]);
    }
    for (int i = 0; i < _graphicsManager->_mirrorComponents.size(); ++i)
    {
        Frustum frustum(_mainRenderData->camera->getProjectionMatrix() * _mainRenderData->camera->getViewMatrix());
        if (_graphicsManager->_mirrorComponents[i]->isActive() && isPointInFrustum(frustum, _graphicsManager->_mirrorComponents[i]->getPosition()))
        {
            _graphicsManager->_mirrorComponents[i]->calculateReflectionVectorAndRotateCamera(_graphicsManager->getCurrentCamera()->getPosition());
            RenderData* renderData = new RenderData;
            renderData->camera = _graphicsManager->_mirrorComponents[i];
            renderData->framebuffer = _graphicsManager->_mirrorComponents[i]->getFramebuffer();
            renderData->renderPass = RP_MIRROR;
            _renderDataList.push_back(renderData);
        }
    }
    _renderDataList.push_back(_mainRenderData);


    for (int i = 0; i < _renderDataList.size(); ++i)
    {
        _renderDataList[i]->renderList.clear();
        _renderDataList[i]->MVMatrix = _renderDataList[i]->camera->getProjectionMatrix() * _renderDataList[i]->camera->getViewMatrix();
    }


    RenderListElement tempRenderElement;

    for (std::list<RenderObject*>::iterator i = _graphicsManager->_renderObjects.begin(); i != _graphicsManager->_renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive())
            continue;

        for (int j = 0; j < _renderDataList.size(); ++j)
        {
            RenderPass renderPass = _renderDataList[j]->renderPass;
            bool isCastShadows = object->isCastShadows();

            if ((renderPass == RP_SHADOWS && isCastShadows || renderPass != RP_SHADOWS) &&
                isObjectInCamera(object, _renderDataList[j]->camera))
            {
                tempRenderElement.model = object->getModel(); // todo: tu powinien byc tez pobierany po lod
                tempRenderElement.object = object->getSceneObject();
                tempRenderElement.renderObject = object;

				ModelNode* modelNode;
				int lod = 0;
				if (tempRenderElement.renderObject->getNumberOfLod() == 1 || renderPass == RP_SHADOWS)
				{
					modelNode = tempRenderElement.renderObject->getModelRootNode();
				}
				else
				{
					if (glm::distance(_renderDataList[j]->camera->getPosition(), tempRenderElement.object->getPosition()) > 30.0f)
					{
						modelNode = tempRenderElement.renderObject->getModelRootNode(1);
						lod = 1;
					}
					else
					{
						modelNode = tempRenderElement.renderObject->getModelRootNode(0);
					}
				}
                addStaticModelNodeToRenderList(modelNode, tempRenderElement, _renderDataList[j]->renderList, renderPass, lod);
            }
        }
#ifdef DRAW_AABB
        _renderObjectsInCurrentFrame.push_back(*i);
#endif // DRAW_AABB
    }

    for (std::list<Grass*>::iterator i = _graphicsManager->_grassComponents.begin(); i != _graphicsManager->_grassComponents.end(); ++i)
    {
        RenderObject* object = *i;

        tempRenderElement.type = RET_GRASS;
        tempRenderElement.model = object->getModel();
        tempRenderElement.object = object->getSceneObject();
        tempRenderElement.renderObject = object;

        ModelNode* modelNode = tempRenderElement.renderObject->getModelRootNode();
        addGrassStaticModelNodeToRenderList(modelNode, tempRenderElement, _renderDataList[_renderDataList.size() - 1]->renderList);
    }


	RenderObject* sky = _graphicsManager->getSky();
	if (sky != NULL)
	{
		for (int j = 0; j < _renderDataList.size(); ++j)
		{
			RenderPass renderPass = _renderDataList[j]->renderPass;
			if (renderPass != RP_SHADOWS)
			{
				tempRenderElement.type = RET_SINGLE;
				tempRenderElement.model = sky->getModel();
				tempRenderElement.object = sky->getSceneObject();
				tempRenderElement.renderObject = sky;

				ModelNode* modelNode = tempRenderElement.renderObject->getModelRootNode();
				addStaticModelNodeToRenderList(modelNode, tempRenderElement, _renderDataList[j]->renderList, renderPass, 0);
			}
		}
	}
    
    if (_renderObjectIdsForPicking)
    {
        _renderDataList[_renderDataList.size() - 1]->renderList.sort(compareByShaderAndObjectId);
    }
    else
    {
        _renderDataList[_renderDataList.size() - 1]->renderList.sort(compareByShader);
    }
}


void Renderer::prepareRenderDataForStaticShadowmaps()
{
	for (int i = 0; i < _renderDataListForStaticShadowmapping.size(); ++i)
	{
		_renderDataListForStaticShadowmapping[i]->renderList.clear();
		_renderDataListForStaticShadowmapping[i]->MVMatrix = _renderDataListForStaticShadowmapping[i]->camera->getProjectionMatrix() * _renderDataListForStaticShadowmapping[i]->camera->getViewMatrix();
	}

	RenderListElement tempRenderElement;

	for (std::list<RenderObject*>::iterator i = _graphicsManager->_renderObjects.begin(); i != _graphicsManager->_renderObjects.end(); ++i)
	{
		RenderObject* object = *i;

		if (!(*i)->isActive())
			continue;

		for (int j = 0; j < _renderDataListForStaticShadowmapping.size(); ++j)
		{
			RenderPass renderPass = _renderDataListForStaticShadowmapping[j]->renderPass;
			bool isCastShadows = object->isCastShadows();
			bool isStaticObject = !object->isDynamicObject();

			if (renderPass == RP_SHADOWS && isCastShadows && isStaticObject &&
				isObjectInCamera(object, _renderDataListForStaticShadowmapping[j]->camera))
			{
				int lod = object->getNumberOfLod() > 1 ? 1 : 0;

				tempRenderElement.type = RET_SINGLE;
				tempRenderElement.model = object->getModel(lod);
				tempRenderElement.object = object->getSceneObject();
				tempRenderElement.renderObject = object;

				ModelNode* modelNode = tempRenderElement.renderObject->getModelRootNode(lod);
				addStaticModelNodeToRenderList(modelNode, tempRenderElement, _renderDataListForStaticShadowmapping[j]->renderList, renderPass, lod); // todo: tu tez powinien byc brany odpowiedni lod
			}
		}
	}
}


bool Renderer::isObjectInCamera(RenderObject* object, CameraStatic* camera)
{
    if (camera->getProjctionType() == CPT_ORTHOGRAPHIC)
    {
        if (isAABBIntersectAABB(*(camera->getAABB()), *object->getAABB()))
            return true;
    }
    else // CPT_PERSPECTIVE
    {
        Frustum frustum(camera->getProjectionMatrix() * camera->getViewMatrix());
        if (isAABBIntersectFrustum(frustum, *object->getAABB()))
            return true;
    }

    return false;
}


/*void Renderer::prepareRenderData()
{
    _renderDataList.clear();
    for (int i = 0; i < _renderDataListForShadowmapping.size(); ++i)
    {
        _renderDataList.push_back(_renderDataListForShadowmapping[i]);
    }
    _renderDataList.push_back(_mainRenderData);

    GraphicsManager& graphicsManager = GraphicsManager::getInstance();

    for (int i = 0; i < _renderDataList.size(); ++i)
    {
        _renderDataList[i]->renderList.clear();
        _renderDataList[i]->MVMatrix = _renderDataList[i]->camera->getProjectionMatrix() * _renderDataList[i]->camera->getViewMatrix();
    }

    RenderListElement tempRenderElement;

    for (std::list<RenderObject*>::iterator i = graphicsManager._renderObjects.begin(); i != graphicsManager._renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive())
            continue;

        if (object->isCastShadows())
        {
            for (int j = 0; j < _renderDataList.size() - 1; ++j)
            {
                if (!isAABBIntersectAABB(*(_renderDataList[j]->camera->getAABB()), *object->getAABB()))
                    continue;

                //if (tempRenderElement.renderObject == NULL)
                //{
                    tempRenderElement.model = object->getModel();
                    tempRenderElement.object = object->getSceneObject();
                    tempRenderElement.renderObject = object;
                //}

                for (int k = 0; k < tempRenderElement.model->getMeshesCount(); ++k)
                {
                    tempRenderElement.mesh = tempRenderElement.model->getMesh(k);

                    if (tempRenderElement.mesh->material.shader == ALPHA_TEST_MATERIAL || tempRenderElement.mesh->material.shader == TREE_MATERIAL)
                        _renderDataList[j]->renderList.insert(_renderDataList[j]->renderList.begin(), tempRenderElement);
                    else if (tempRenderElement.mesh->material.transparency == 0.0f) // czy materia³ nie jest przezroczysty lub skybox itd.
                        _renderDataList[j]->renderList.push_back(tempRenderElement);
                }
            }
        }

        // Main render target
        int listIndex = _renderDataList.size() - 1;

        Frustum frustum(_renderDataList[listIndex]->camera->getProjectionMatrix() * _renderDataList[listIndex]->camera->getViewMatrix());
        if (!isAABBIntersectFrustum(frustum, *object->getAABB()))
            continue;

        //if (tempRenderElement.renderObject == NULL)
        //
            tempRenderElement.model = object->getModel();
            tempRenderElement.object = object->getSceneObject();
            tempRenderElement.renderObject = object;
        //}

        for (int k = 0; k < tempRenderElement.model->getMeshesCount(); ++k)
        {
            tempRenderElement.mesh = tempRenderElement.model->getMesh(k);
            _renderDataList[listIndex]->renderList.push_back(tempRenderElement);
        }
    }

    for (std::list<Grass*>::iterator i = graphicsManager._grassComponents.begin(); i != graphicsManager._grassComponents.end(); ++i)
    {
        RenderObject* object = *i;

        tempRenderElement.type = RET_GRASS;
        tempRenderElement.model = object->getModel();
        tempRenderElement.object = object->getSceneObject();
        tempRenderElement.renderObject = object;

        for (int j = 0; j < object->getModel()->getMeshesCount(); ++j)
        {
            tempRenderElement.mesh = tempRenderElement.model->getMesh(j);
            _renderDataList[_renderDataList.size() - 1]->renderList.push_back(tempRenderElement);
        }
    }

    _renderDataList[_renderDataList.size() - 1]->renderList.sort(compareByShader);
}*/


void Renderer::createRenderDatasForShadowMap(ShadowMap* shadowMap)
{
    for (int i = 0; i < shadowMap->CASCADE_COUNT - 1; ++i)
    {
        RenderData* renderData = new RenderData;
        renderData->camera = shadowMap->getCameraForShadowMap(i);
        renderData->framebuffer = shadowMap->getShadowMapFramebuffer();
        renderData->framebufferLayer = i;
        renderData->renderPass = RP_SHADOWS;
        _renderDataListForShadowmapping.insert(_renderDataListForShadowmapping.begin(), renderData);
    }

	int staticShadowmapIndex = shadowMap->CASCADE_COUNT - 1;
	RenderData* renderData = new RenderData;
	renderData->camera = shadowMap->getCameraForShadowMap(staticShadowmapIndex);
	renderData->framebuffer = shadowMap->getShadowMapFramebuffer();
    renderData->framebufferLayer = staticShadowmapIndex;
	renderData->renderPass = RP_SHADOWS;
	_renderDataListForStaticShadowmapping.insert(_renderDataListForStaticShadowmapping.begin(), renderData);
}


void Renderer::deleteRenderDatasForShadowMap(ShadowMap* shadowMap)
{
    for (int i = 0; i < shadowMap->CASCADE_COUNT - 1; ++i)
    {
        for (std::vector<RenderData*>::iterator j = _renderDataListForShadowmapping.begin(); j != _renderDataListForShadowmapping.end(); ++j)
        {
            if (shadowMap->getShadowMapFramebuffer() == (*j)->framebuffer)
            {
                RenderData* renderData = (*j);
                _renderDataListForShadowmapping.erase(j);

                for (std::vector<RenderData*>::iterator k = _renderDataList.begin(); k != _renderDataList.end(); ++k)
                {
                    if (renderData == *k)
                    {
                        _renderDataList.erase(k);
                        break;
                    }
                }

                delete renderData;

                break;
            }
        }
    }

	int staticShadowmapIndex = shadowMap->CASCADE_COUNT - 1;
	for (std::vector<RenderData*>::iterator i = _renderDataListForStaticShadowmapping.begin(); i != _renderDataListForStaticShadowmapping.end(); ++i)
	{
		if (shadowMap->getShadowMapFramebuffer() == (*i)->framebuffer)
		{
			RenderData* renderData = (*i);
			_renderDataListForStaticShadowmapping.erase(i);
			delete renderData;

			break;
		}
	}
}


void Renderer::clearRenderDatasForShadowMap()
{
    for (std::vector<RenderData*>::iterator j = _renderDataListForShadowmapping.begin(); j != _renderDataListForShadowmapping.end();)
    {
        RenderData* renderData = (*j);
        j = _renderDataListForShadowmapping.erase(j);

        for (std::vector<RenderData*>::iterator k = _renderDataList.begin(); k != _renderDataList.end(); ++k)
        {
            if (renderData == *k)
            {
                _renderDataList.erase(k);
                break;
            }
        }

        delete renderData;
    }

    for (std::vector<RenderData*>::iterator i = _renderDataListForStaticShadowmapping.begin(); i != _renderDataListForStaticShadowmapping.end();)
    {
        RenderData* renderData = (*i);
        i = _renderDataListForStaticShadowmapping.erase(i);
        delete renderData;
    }

    _shadowMap = NULL;
}


const std::vector<glm::mat4>& Renderer::getFinalMatrices(SceneObject* sceneObject)
{
    SkeletalAnimationComponent* skeletatlAnimation = static_cast<SkeletalAnimationComponent*>(sceneObject->getComponent(CT_SKELETAL_ANIMATION));
    if (skeletatlAnimation != nullptr)
    {
        return skeletatlAnimation->getFinalBoneMatrices();
    }

    SkeletalAnimationComponent2* skeletatlAnimation2 = static_cast<SkeletalAnimationComponent2*>(sceneObject->getComponent(CT_SKELETAL_ANIMATION_2));
    if (skeletatlAnimation2 != nullptr)
    {
        return skeletatlAnimation2->getFinalBoneMatrices();
    }

    return {};
}


void Renderer::rebuildStaticLightingInternal()
{
    clearRenderDatasForShadowMap();

    for (Light* light : _graphicsManager->_lights)
    {
        if (light->isShadowMapping())
        {
            registerShadowMap(light->getShadowMap());
        }
    }

    bakeStaticShadows();

    EnvironmentCaptureComponent* envCaptureComponent = _graphicsManager->getGlobalEnvironmentCaptureComponent();
    if (envCaptureComponent != nullptr)
    {
        envCaptureComponent->generateRequiredPbrMaps();
    }

    _requiredRebuildStaticLighting = false;
}


void Renderer::recreateObjectsIdsTextureData()
{
    if (_objectsIdsTextureData != nullptr)
    {
        delete _objectsIdsTextureData;
    }

    _objectsIdsTextureData = new unsigned int[_screenWidth * _screenHeight];
}


void Renderer::init(unsigned int screenWidth, unsigned int screenHeight)
{
    if (_isInitialized)
        return;


    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _defaultFramebuffer = OGLDriver::getInstance().getDefaultFramebuffer();
    _defaultFramebuffer->setViewport(UintRect(0, 0, _screenWidth, _screenHeight));

    _depthFramebuffer = OGLDriver::getInstance().createFramebuffer();
    _depthFramebuffer->addTexture(TF_DEPTH_COMPONENT, _screenWidth, _screenHeight);
    _depthFramebuffer->init();

    Framebuffer* framebuffer = OGLDriver::getInstance().createFramebuffer();
    framebuffer->addDepthRenderbuffer(_screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
    framebuffer->addTexture(_framebufferTextureFormat, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
    framebuffer->addTexture(_framebufferTextureFormat, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);

    if (_renderObjectIdsForPicking)
    {
        framebuffer->addTexture(TF_RED_32, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
        recreateObjectsIdsTextureData();
    }

    framebuffer->init();
    //framebuffer->getTexture(0)->setFiltering(TFM_NEAREST, TFM_NEAREST);
    //framebuffer->getTexture(1)->setFiltering(TFM_NEAREST, TFM_NEAREST);

    _mainRenderData = new RenderData;
    //_mainRenderData->camera = _graphicsManager->getCurrentCamera();
    _mainRenderData->framebuffer = framebuffer;
    _mainRenderData->renderPass = RP_NORMAL;


	// Post processing effects initialize
	createFramebuffersForPostProcessing();
	initPostProcessingEffectsStack();


    _shaderList.resize(NUMBER_OF_SHADERS);

	std::vector<std::string> defines;
	std::unordered_map<std::string, std::string> constants;

    // Load shaders
    // SOLID_MATERIAL
    defines.push_back("SOLID");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
	_shaderList[SOLID_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // NOTEXTURE_MATERIAL
    _shaderList[NOTEXTURE_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader_notexture.frag", defines);

    // NORMALMAPPING_MATERIAL
    defines.clear();
    defines.push_back("NORMALMAPPING");
    defines.push_back("SOLID");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[NORMALMAPPING_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

	// SOLID_EMISSIVE_MATERIAL
	defines.clear();
	defines.push_back("SOLID");
	defines.push_back("EMISSIVE");
	if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
	_shaderList[SOLID_EMISSIVE_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // CAR_PAINT_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("CAR_PAINT");
    defines.push_back("REFLECTION");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
	_shaderList[CAR_PAINT_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // MIRROR_MATERIAL
	_shaderList[MIRROR_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/mirror.frag");

    // ALPHA_TEST_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[ALPHA_TEST_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // TREE_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    defines.push_back("SUBSURFACE_SCATTERING");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[TREE_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/tree.vert", "Shaders/shader.frag", defines);

    // DECAL_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("DECALS");
    defines.push_back("ALPHA_TEST");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[DECAL_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // SOLID_ANIMATED_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ANIMATED");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[SOLID_ANIMATED_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // NORMALMAPPING_ANIMATED_MATERIAL
    defines.clear();
    defines.push_back("NORMALMAPPING");
    defines.push_back("SOLID");
    defines.push_back("ANIMATED");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[NORMALMAPPING_ANIMATED_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // ALPHA_TEST_ANIMATED_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    defines.push_back("ANIMATED");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[ALPHA_TEST_ANIMATED_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // GRASS_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    defines.push_back("SUBSURFACE_SCATTERING");
    defines.push_back("GRASS");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[GRASS_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/grass.vert", "Shaders/shader.frag", defines);

    // SKY_MATERIAL
    defines.clear();
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[SKY_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/sky.vert", "Shaders/sky.frag", defines);

    // GLASS_MATERIAL
    defines.clear();
    defines.push_back("GLASS");
    defines.push_back("REFLECTION");
    //defines.push_back("TRANSPARENCY");
    //if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
    _shaderList[GLASS_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // NOTEXTURE_ALWAYS_VISIBLE_MATERIAL
    _shaderList[NOTEXTURE_ALWAYS_VISIBLE_MATERIAL] = _shaderList[NOTEXTURE_MATERIAL];

    // GUI_IMAGE_SHADER
    _shaderList[GUI_IMAGE_SHADER] = ResourceManager::getInstance().loadShader("Shaders/GUIshader.vert", "Shaders/GUIshader.frag");

    // GUI_LABEL_SHADER
    _shaderList[GUI_LABEL_SHADER] = ResourceManager::getInstance().loadShader("Shaders/GUIshader.vert", "Shaders/LabelShader.frag");

    // GUI_PROGRESS_BAR_SHADER
    _shaderList[GUI_PROGRESS_BAR_SHADER] = ResourceManager::getInstance().loadShader("Shaders/GUIshader.vert", "Shaders/GUIProgressBarshader.frag");

    // DEBUG_SHADER
    _shaderList[DEBUG_SHADER] = ResourceManager::getInstance().loadShader("Shaders/debug.vert", "Shaders/debug.frag");

    // SHADOWMAP_SHADER
    _shaderList[SHADOWMAP_SHADER] = ResourceManager::getInstance().loadShader("Shaders/shadowmap.vert", "Shaders/shadowmap.frag");

    // SHADOWMAP_ALPHA_TEST_SHADER
    defines.clear();
    defines.push_back("ALPHA_TEST");
    _shaderList[SHADOWMAP_ALPHA_TEST_SHADER] = ResourceManager::getInstance().loadShader("Shaders/shadowmap.vert", "Shaders/shadowmap.frag", defines);

    // SHADOWMAP_ANIMATED_SHADER
    defines.clear();
    defines.push_back("ANIMATED");
    _shaderList[SHADOWMAP_ANIMATED_SHADER] = ResourceManager::getInstance().loadShader("Shaders/shadowmap.vert", "Shaders/shadowmap.frag", defines);

    // MIRROR_SOLID_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
	_shaderList[MIRROR_SOLID_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // MIRROR_ALPHA_TEST_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    _shaderList[MIRROR_ALPHA_TEST_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // MIRROR_GLASS_MATERIAL
    defines.clear();
    defines.push_back("GLASS");
    defines.push_back("REFLECTION");
    _shaderList[MIRROR_GLASS_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // MIRROR_SOLID_ANIMATED_MATRIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ANIMATED");
    _shaderList[MIRROR_SOLID_ANIMATED_MATRIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // MIRROR_ALPHA_TEST_ANIMATED_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    defines.push_back("ANIMATED");
    _shaderList[MIRROR_ALPHA_TEST_ANIMATED_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

	// PBR_MATERIAL
	defines.clear();
	if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
	_shaderList[PBR_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/pbr.vert", "Shaders/pbr.frag", defines);

	// PBR_TREE_MATERIAL
	_shaderList[PBR_TREE_MATERIAL] = _shaderList[PBR_MATERIAL];

	// NEW_TREE_MATERIAL
	defines.clear();
	defines.push_back("NORMALMAPPING");
	defines.push_back("TREE");
	if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
	_shaderList[NEW_TREE_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/newTree.frag", defines);

	// NEW_TREE_2_MATERIAL
	defines.clear();
	defines.push_back("NORMALMAPPING");
	defines.push_back("TREE");
	if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    if (_renderObjectIdsForPicking) defines.push_back("RENDER_OBJECT_ID");
	_shaderList[NEW_TREE_2_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/newTree2.frag", defines);

    // EDITOR_AXIS_SHADER
    _shaderList[EDITOR_AXIS_SHADER] = _shaderList[SOLID_MATERIAL];

    // WIREFRAME_MATERIAL
    _shaderList[WIREFRAME_MATERIAL] = _shaderList[NOTEXTURE_MATERIAL];

    _shaderListForMirrorRendering.resize(NUMBER_OF_SHADERS);
    _shaderListForMirrorRendering[SOLID_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[NOTEXTURE_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[NORMALMAPPING_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[SOLID_EMISSIVE_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[CAR_PAINT_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[MIRROR_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[ALPHA_TEST_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[NEW_TREE_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[NEW_TREE_2_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[PBR_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[PBR_TREE_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[TREE_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[DECAL_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[SOLID_ANIMATED_MATERIAL] = MIRROR_SOLID_ANIMATED_MATRIAL;
    _shaderListForMirrorRendering[NORMALMAPPING_ANIMATED_MATERIAL] = MIRROR_SOLID_ANIMATED_MATRIAL;
    _shaderListForMirrorRendering[ALPHA_TEST_ANIMATED_MATERIAL] = MIRROR_ALPHA_TEST_ANIMATED_MATERIAL;
    _shaderListForMirrorRendering[GRASS_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[SKY_MATERIAL] = SKY_MATERIAL;
    _shaderListForMirrorRendering[GLASS_MATERIAL] = MIRROR_GLASS_MATERIAL;


	initUniformLocations();


    // Create UBO for lights
    _lightUBO = OGLDriver::getInstance().createUBO(4096);
    _lightUBO->bindBufferBase(0);

    _shaderList[SOLID_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);
    _shaderList[NORMALMAPPING_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);


    if (_renderObjectIdsForPicking)
    {
        defines.clear();
        defines.push_back(_msaaAntialiasing ? "MSAA_ENABLED" : "MSAA_DISABLED");

        _pickingComputeShader = ResourceManager::getInstance().loadComputeShader("Shaders/compute/pickingShader.comp", defines);
        _pickingSSBO = OGLDriver::getInstance().createShaderStorageBuffesr(sizeof(PickingShaderStorageBufferData));
    }


    _isInitialized = true;
}


void Renderer::setGraphicsManager(GraphicsManager* graphicsManager)
{
    _graphicsManager = graphicsManager;
}


void Renderer::setAlphaToCoverage(bool isEnable)
{
    _alphaToCoverage = isEnable;
}


bool Renderer::isAlphaToCoverageEnable()
{
    return _alphaToCoverage;
}


void Renderer::setExposure(float exposure)
{
	if (_isInitialized)
	{
		PostProcessingToneMapping* toneMappingEffect = static_cast<PostProcessingToneMapping*>(findEffect(PPT_TONE_MAPPING));
		toneMappingEffect->setExposure(exposure);
	}

    _exposure = exposure;
}


float Renderer::getExposure()
{
    return _exposure;
}


void Renderer::setSceneVisibility(float visibility)
{
    if (_isInitialized)
    {
        PostProcessingToneMapping* toneMappingEffect = static_cast<PostProcessingToneMapping*>(findEffect(PPT_TONE_MAPPING));
        toneMappingEffect->setVisibility(visibility);
    }

    _sceneVisibility = visibility;
}


float Renderer::getSceneVisibility()
{
    return _sceneVisibility;
}

void Renderer::setToneMappingType(ToneMappingType type)
{
	if (_isInitialized)
	{
		PostProcessingToneMapping* toneMappingEffect = static_cast<PostProcessingToneMapping*>(findEffect(PPT_TONE_MAPPING));
		toneMappingEffect->setToneMappingType(type);
	}

	_toneMappingType = type;
}


ToneMappingType Renderer::getToneMappingType()
{
	return _toneMappingType;
}


void Renderer::setFramebufferTextureFormat(TextureFormat format)
{
	_framebufferTextureFormat = format;
}


TextureFormat Renderer::getFramebufferTextureFormat()
{
	return _framebufferTextureFormat;
}


void Renderer::setMsaaAntialiasing(bool isEnable)
{
    _msaaAntialiasing = isEnable;
}


bool Renderer::isMsaaAntialiasingEnable()
{
    return _msaaAntialiasing;
}


void Renderer::setMsaaAntialiasingLevel(int level)
{
    _msaaAntialiasingLevel = level;
}


int Renderer::getMsaaAntialiasingLevel()
{
    return _msaaAntialiasingLevel;
}


void Renderer::setBloom(bool isEnable)
{
	if (_isInitialized)
	{
		if (isEnable && findEffect(PPT_BLOOM) == NULL)
		{
			PostProcessingBloom* postProcessingBloom = createBloomPostProcessing();
			addPostProcessingEffect(postProcessingBloom);
		}
		else if (!isEnable)
		{
			removePostProcessingEffect(PPT_BLOOM);
		}
	}

    _bloom = isEnable;
}


bool Renderer::isBloomEnable()
{
    return _bloom;
}


void Renderer::setRenderObjectIdsForPicking(bool isEnabled)
{
    _renderObjectIdsForPicking = isEnabled;
}


bool Renderer::isRenderObjectIdsForPickingEnabled()
{
    return _renderObjectIdsForPicking;
}


void Renderer::setIsShadowMappingEnable(bool isEnable)
{
    _isShadowMappingEnable = isEnable;

    if (_shadowMap != NULL)
    {
        if (isEnable)
        {
            createRenderDatasForShadowMap(_shadowMap);
        }
        else
        {
            deleteRenderDatasForShadowMap(_shadowMap);
        }
    }
}


bool Renderer::isShadowMappingEnable()
{
    return _isShadowMappingEnable;
}


void Renderer::registerShadowMap(ShadowMap* shadowMap)
{
    // todo: graphicsManager refactor
    if (_isShadowMappingEnable && _shadowMap == NULL)
    {
        _shadowMap = shadowMap;

        createRenderDatasForShadowMap(shadowMap);
    }
}


void Renderer::unregisterShadowMap(ShadowMap* shadowMap)
{
    // todo: graphicsManager refactor
    if (_shadowMap == shadowMap)
    {
        _shadowMap = NULL;

        deleteRenderDatasForShadowMap(shadowMap);
    }
}


void Renderer::setCurrentMainCamera(CameraStatic* camera)
{
    //_renderDataList[_renderDataList.size() - 1]->camera = camera;
    //_mainRenderData->camera = camera;
}


CameraStatic* Renderer::getCurrentMainCamera()
{
    return _mainRenderData->camera;
}


void Renderer::setDayNightRatio(float ratio)
{
    _dayNightRatio = ratio;
}


float Renderer::getDayNightRatio()
{
    return _dayNightRatio;
}


void Renderer::setWindowDimensions(unsigned int screenWidth, unsigned int screenHeight)
{
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    Framebuffer* defaultFramebuffer = OGLDriver::getInstance().getDefaultFramebuffer();
    defaultFramebuffer->setViewport(UintRect(0, 0, _screenWidth, _screenHeight));

	recreateAllFramebuffers();
}


glm::uvec2 Renderer::getWindowDimensions()
{
    return glm::uvec2(_screenWidth, _screenHeight);
}


void Renderer::toogleRenderAABBFlag()
{
    _renderObjectsAAABB = !_renderObjectsAAABB;
}


void Renderer::toogleRenderOBBFlag()
{
    _renderObjectsOBB = !_renderObjectsOBB;
}


bool Renderer::getAABBFlag()
{
    return _renderObjectsAAABB;
}


bool Renderer::getOBBFlag()
{
    return _renderObjectsOBB;
}


VBO* Renderer::getQuadVbo()
{
	return _quadVBO;
}


void Renderer::bakeStaticShadows()
{
	if (!_isShadowMappingEnable)
	{
		return;
	}

	LOG_INFO("Static shadows baking: start" + Strings::toString(_renderDataListForStaticShadowmapping[0]->renderList.size()));
	Timer timer;
	timer.start();

	prepareRenderDataForStaticShadowmaps();

	for (int i = 0; i < _renderDataListForStaticShadowmapping.size(); ++i)
	{
		//if (_renderDataListForStaticShadowmapping[i]->renderPass == RP_SHADOWS)
		{
			renderDepth(_renderDataListForStaticShadowmapping[i]);
		}
	}

	double time = timer.stop();
	LOG_INFO("Static shadows baking: end. Time: " + Strings::toString(time));
}


void Renderer::rebuildStaticLighting()
{
    _requiredRebuildStaticLighting = true;
}


void Renderer::renderAll()
{
    _mainRenderData->camera = _graphicsManager->getCurrentCamera();

    if (_requiredRebuildStaticLighting)
    {
        rebuildStaticLightingInternal();
    }

    prepareLightsData();
    prepareRenderData();

    for (int i = 0; i < _renderDataList.size(); ++i)
    {
        if (_renderDataList[i]->renderPass == RP_SHADOWS)
            renderDepth(_renderDataList[i]);
        else if (_renderDataList[i]->renderPass == RP_MIRROR)
            renderToMirrorTexture(_renderDataList[i]);
        else // RP_NORMAL
        {
            Framebuffer* framebufer = _renderDataList[i]->framebuffer;

            // depth renering - for decals
            _renderDataList[i]->framebuffer = _depthFramebuffer;
            renderDepth(_renderDataList[i]);

            _renderDataList[i]->framebuffer = framebufer;
            renderScene(_renderDataList[i]);
        }
    }


    glDisable(GL_DEPTH_TEST);


	for (int i = 0; i < _postProcessingEffectsStack.size(); ++i)
	{
		RTexture* input;
		if (i == 0)
            //input = _depthFramebuffer->getTexture(0);
			input = _mainRenderData->framebuffer->getTexture(0);
		else
			input = _postProcessingFramebuffers[(i - 1) % 2]->getTexture(0);

		Framebuffer * output = _postProcessingFramebuffers[i % 2];
		if (i == _postProcessingEffectsStack.size() - 1)
			output = _defaultFramebuffer;

		_postProcessingEffectsStack[i]->run(input, output);
	}


    glEnable(GL_DEPTH_TEST);
}


void Renderer::renderDepth(RenderData* renderData)
{
    renderData->framebuffer->bind();
    if (renderData->framebufferLayer >= 0)
    {
        renderData->framebuffer->bindTextureArrayLayerToRender(renderData->framebufferLayer);
    }

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    ShaderType currentShader = GUI_IMAGE_SHADER;
    RShader* shader;
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RStaticModel* model = i->model;
        ModelNodeMesh* mesh = i->mesh;
        Material* material = i->material;

        ShaderType shaderType;
        bool isAlphaTest = material->shader == ALPHA_TEST_MATERIAL || material->shader == TREE_MATERIAL || material->shader == NEW_TREE_MATERIAL || material->shader == PBR_TREE_MATERIAL || material->shader == NEW_TREE_2_MATERIAL;
        bool isAnimated = material->shader == SOLID_ANIMATED_MATERIAL || material->shader == NORMALMAPPING_ANIMATED_MATERIAL || material->shader == ALPHA_TEST_ANIMATED_MATERIAL;
        if (isAlphaTest)
            shaderType = SHADOWMAP_ALPHA_TEST_SHADER;
        else if (isAnimated)
            shaderType = SHADOWMAP_ANIMATED_SHADER;
        else
            shaderType = SHADOWMAP_SHADER;

        if (material->shader == DECAL_MATERIAL)
        {
            continue;
        }

        if (shaderType != currentShader)
        {
            shader = _shaderList[shaderType];
            shader->enable();

			{
				glEnable(GL_CULL_FACE);
			}

			if (material->shader == PBR_TREE_MATERIAL || material->shader == NEW_TREE_2_MATERIAL)
			{
				glDisable(GL_CULL_FACE);
			}

            currentShader = shaderType;
        }
        else
        {
            shader->resetTextureLocation();
        }

        glm::mat4 modelMatrix = i->object->getGlobalTransformMatrix() * i->transformMatrix;
        glm::mat4 MVP = renderData->MVMatrix * modelMatrix;
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MVP], MVP);

        mesh->vbo->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)0);

        if (isAlphaTest)
        {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 3));

            shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_ALPHA_TEXTURE], material->diffuseTexture);

			shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MATERIAL_FIX_DISAPPEARANCE_ALPHA], material->fixDisappearanceAlpha);
			shader->setUniform(_uniformsLocations[shaderType][UNIFORM_ALPHA_TEST_THRESHOLD], material->shadowmappingAlphaTestThreshold);
        }
        if (isAnimated)
        {
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, mesh->vertexSize, (void*)(sizeof(float) * 14));

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 14 + sizeof(int) * 4));

            SceneObject* sceneObject = i->object;

            const std::vector<glm::mat4>& finalMatrices = getFinalMatrices(sceneObject);

            for (int i = 0; i < MAX_BONES; ++i)
            {
                const glm::mat4& matrix = finalMatrices[i];

                shader->setUniform(shader->getUniformLocation(("finalBonesMatrices[" + Strings::toString(i) + "]").c_str()), matrix);
            }
        }

        mesh->ibo->bind();
        glDrawElements(model->getPrimitiveType(),
                       mesh->indicesCount,
                       GL_UNSIGNED_INT,
                       (void*)(mesh->firstVertex * sizeof(unsigned int)));

        glDisableVertexAttribArray(0);
        if (isAlphaTest)
        {
            glDisableVertexAttribArray(1);
        }
        if (isAnimated)
        {
            glDisableVertexAttribArray(5);
            glDisableVertexAttribArray(6);
        }
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}


void Renderer::renderToMirrorTexture(RenderData* renderData)
{
    renderData->framebuffer->bind();
    if (renderData->framebufferLayer >= 0)
    {
        renderData->framebuffer->bindTextureArrayLayerToRender(renderData->framebufferLayer);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    ShaderType currentShader = NUMBER_OF_SHADERS;
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RStaticModel* model = i->model;
        ModelNodeMesh* mesh = i->mesh;
        Material* material = i->material;

        ShaderType shaderType = _shaderListForMirrorRendering[material->shader];
        RShader* shader = _shaderList[shaderType];
        if (shaderType != currentShader)
        {
            shader->enable();

            if (currentShader == SKY_MATERIAL || currentShader == PBR_TREE_MATERIAL || currentShader == NEW_TREE_2_MATERIAL)
            {
                glEnable(GL_CULL_FACE);
            }
            else if (currentShader == MIRROR_ALPHA_TEST_MATERIAL || currentShader == MIRROR_ALPHA_TEST_ANIMATED_MATERIAL)
            {
                glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (currentShader == MIRROR_GLASS_MATERIAL)
            {
                glDisable(GL_BLEND);
            }

            if (shaderType == SKY_MATERIAL || shaderType == PBR_TREE_MATERIAL || shaderType == NEW_TREE_2_MATERIAL)
            {
                glDisable(GL_CULL_FACE);
            }
            else if (shaderType == MIRROR_ALPHA_TEST_MATERIAL || shaderType == MIRROR_ALPHA_TEST_ANIMATED_MATERIAL)
            {
                glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (shaderType == MIRROR_GLASS_MATERIAL)
            {
                glEnable(GL_BLEND);
            }

            currentShader = shaderType;
        }
        else
        {
            shader->resetTextureLocation();
        }


        if (shaderType == MIRROR_GLASS_MATERIAL)
        {
            if (material->glassTexture != NULL)
                shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_GLASS_TEXTURE], material->glassTexture);


            if (material->reflectionTexture1 == EMT_GLOBAL)
            {
                shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_TEXTURE], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_MATRIX], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_TEXTURE], static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_MATRIX], static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }

            if (material->reflectionTexture2 == EMT_GLOBAL)
            {
                shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_2_TEXTURE], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_2_MATRIX], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_2_TEXTURE], static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(_uniformsLocations[shaderType][UNIFORM_ENVIRONMENTMAP_2_MATRIX], static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }
        }


        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_DAY_NIGHT_RATIO], _dayNightRatio);


        glm::mat4 modelMatrix = i->object->getGlobalTransformMatrix() * i->transformMatrix;
        glm::mat4 normalMatrix = i->object->getGlobalNormalMatrix() * i->normalMatrix;
        glm::mat4 MVP = renderData->MVMatrix * modelMatrix;
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MVP], MVP);
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MODEL_MATRIX], modelMatrix);
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_NORMAL_MATRIX], normalMatrix);

        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MATERIAL_AMBIENT_COLOR], material->ambientColor);
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MATERIAL_DIFFUSE_COLOR], material->diffuseColor);
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MATERIAL_SPECULAR_COLOR], material->specularColor);

        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MATERIAL_TRANSPARENCY], material->transparency);
		shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MATERIAL_FIX_DISAPPEARANCE_ALPHA], material->fixDisappearanceAlpha);
		shader->setUniform(_uniformsLocations[shaderType][UNIFORM_ALPHA_TEST_THRESHOLD], material->alphaTestThreshold);

        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_MATERIAL_SPECULAR_POWER], material->shininess);
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_CAMERA_POSITION], camera->getPosition());


        mesh->vbo->bind();
        mesh->ibo->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 3));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 5));

        bool isAnimated = currentShader == MIRROR_SOLID_ANIMATED_MATRIAL || currentShader == MIRROR_ALPHA_TEST_ANIMATED_MATERIAL;
        if (isAnimated)
        {
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, mesh->vertexSize, (void*)(sizeof(float) * 14));

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 14 + sizeof(int) * 4));

            SceneObject* sceneObject = i->object;

            const std::vector<glm::mat4>& finalMatrices = getFinalMatrices(sceneObject);

            for (int i = 0; i < MAX_BONES; ++i)
            {
                const glm::mat4& matrix = finalMatrices[i];

                shader->setUniform(shader->getUniformLocation(("finalBonesMatrices[" + Strings::toString(i) + "]").c_str()), matrix);
            }
        }

        if (material->diffuseTexture != NULL)
            shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_DIFFUSE_TEXTURE], material->diffuseTexture);
        if (material->emissiveTexture != NULL)
            shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_EMISSIVE_TEXTURE], material->emissiveTexture);


        if (i->type != RET_GRASS)
        {
            glDrawElements(model->getPrimitiveType(),
                           mesh->indicesCount,
                           GL_UNSIGNED_INT,
                           (void*)(mesh->firstVertex * sizeof(unsigned int)));
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        if (isAnimated)
        {
            glDisableVertexAttribArray(5);
            glDisableVertexAttribArray(6);
        }
    }
    glEnable(GL_CULL_FACE);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_BLEND);
}


// UBO
void Renderer::renderScene(RenderData* renderData)
{
    glm::mat4 lightSpaceMatrix[ShadowMap::CASCADE_COUNT];
    if (_isShadowMappingEnable && _shadowMap != NULL)
    {
        for (int j = 0; j < _shadowMap->CASCADE_COUNT; ++j)
        {
            lightSpaceMatrix[j] = _shadowMap->getCameraForShadowMap(j)->getProjectionMatrix() * _shadowMap->getCameraForShadowMap(j)->getViewMatrix();
        }
    }

    renderData->framebuffer->bind();
    if (renderData->framebufferLayer >= 0)
    {
        renderData->framebuffer->bindTextureArrayLayerToRender(renderData->framebufferLayer);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    ShaderType currentShader = NUMBER_OF_SHADERS;
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RStaticModel* model = i->model;
        ModelNodeMesh* mesh = i->mesh;
        Material* material = i->material;

        RShader* shader = _shaderList[material->shader];
        if (material->shader != currentShader)
        {
            shader->enable();

            if (currentShader == SKY_MATERIAL || currentShader == PBR_TREE_MATERIAL || currentShader == NEW_TREE_2_MATERIAL)
            {
                glEnable(GL_CULL_FACE);
            }
            else if (currentShader == TREE_MATERIAL || currentShader == ALPHA_TEST_MATERIAL || currentShader == GRASS_MATERIAL || currentShader == NEW_TREE_MATERIAL || currentShader == ALPHA_TEST_ANIMATED_MATERIAL)
            {
                glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (currentShader == GLASS_MATERIAL)
            {
                glDisable(GL_BLEND);
            }
            else if (currentShader == WIREFRAME_MATERIAL)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else if (currentShader == DECAL_MATERIAL)
            {
                glDepthMask(GL_TRUE);
            }
            else if (currentShader == NOTEXTURE_ALWAYS_VISIBLE_MATERIAL)
            {
                glEnable(GL_DEPTH_TEST);
            }

            if (material->shader == SKY_MATERIAL || material->shader == PBR_TREE_MATERIAL || material->shader == NEW_TREE_2_MATERIAL)
            {
                glDisable(GL_CULL_FACE);
            }
            else if ((material->shader == TREE_MATERIAL || material->shader == ALPHA_TEST_MATERIAL || material->shader == GRASS_MATERIAL || material->shader == NEW_TREE_MATERIAL || material->shader == ALPHA_TEST_ANIMATED_MATERIAL) && _alphaToCoverage)
            {
                glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (material->shader == GLASS_MATERIAL)
            {
                glEnable(GL_BLEND);
            }
            else if (material->shader == WIREFRAME_MATERIAL)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else if (material->shader == DECAL_MATERIAL)
            {
                glDepthMask(GL_FALSE);
            }
            else if (material->shader == NOTEXTURE_ALWAYS_VISIBLE_MATERIAL)
            {
                glDisable(GL_DEPTH_TEST);
            }
            else if (material->shader == EDITOR_AXIS_SHADER)
            {
                glClear(GL_DEPTH_BUFFER_BIT);
            }

            currentShader = material->shader;
        }
        else
        {
            shader->resetTextureLocation();
        }


        if (material->shader == GLASS_MATERIAL || material->shader == CAR_PAINT_MATERIAL)
        {
            if (material->glassTexture != NULL)
                shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_GLASS_TEXTURE], material->glassTexture);


            if (material->reflectionTexture1 == EMT_GLOBAL)
            {
                shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_TEXTURE], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_MATRIX], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_TEXTURE], static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_MATRIX], static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }

            if (material->reflectionTexture2 == EMT_GLOBAL)
            {
                shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_2_TEXTURE], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_2_MATRIX], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_2_TEXTURE], static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(_uniformsLocations[currentShader][UNIFORM_ENVIRONMENTMAP_2_MATRIX], static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }
        }


        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_DAY_NIGHT_RATIO], _dayNightRatio);


        if (material->shader == TREE_MATERIAL)
        {
            glm::vec3 d = _graphicsManager->getWindVector();

            Component* treeComponent = i->object->getComponent(CT_TREE_COMPONENT);
            if (treeComponent != NULL)
            {
                d = static_cast<TreeComponent*>(treeComponent)->getWindVector();
            }

            shader->setUniform(_uniformsLocations[currentShader][UNIFORM_WIND_DIRECTION], d);
        }


        glm::mat4 modelMatrix = i->object->getGlobalTransformMatrix() * i->transformMatrix;
        glm::mat4 normalMatrix = i->object->getGlobalNormalMatrix() * i->normalMatrix;
        glm::mat4 MVP = renderData->MVMatrix * modelMatrix;
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MVP], MVP);
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MODEL_MATRIX], modelMatrix);
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_NORMAL_MATRIX], normalMatrix);

        glm::vec4 highlightingEmissiveColor = static_cast<float>(i->renderObject->isHighlighted()) * _graphicsManager->getObjectHighlightingColor();
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MATERIAL_AMBIENT_COLOR], material->ambientColor);
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MATERIAL_DIFFUSE_COLOR], material->diffuseColor);
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MATERIAL_SPECULAR_COLOR], material->specularColor);
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MATERIAL_EMISSIVE_COLOR], material->emissiveColor + highlightingEmissiveColor);

        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MATERIAL_TRANSPARENCY], material->transparency);
		shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MATERIAL_FIX_DISAPPEARANCE_ALPHA], material->fixDisappearanceAlpha);
		shader->setUniform(_uniformsLocations[currentShader][UNIFORM_ALPHA_TEST_THRESHOLD], material->alphaTestThreshold);

        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_MATERIAL_SPECULAR_POWER], material->shininess);
        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_CAMERA_POSITION], camera->getPosition());
        //shader->setUniform("a", 1.0f);


        if (_isShadowMappingEnable)
        {
            shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_SHADOW_MAP], _shadowMap->getShadowMapFramebuffer()->getTexture(0));

            for (int i = 0; i < ShadowMap::CASCADE_COUNT; ++i)
            {
                shader->setUniform(_uniformsLocations[currentShader][UNIFORM_LIGHT_SPACE_MATRIX_1 + i], lightSpaceMatrix[i]);
            }
        }

        mesh->vbo->bind();
        mesh->ibo->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 3));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 5));

        if (material->normalmapTexture != NULL)
        {
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 8));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 11));

            shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_NOTMALMAP_TEXTURE], material->normalmapTexture);
        }

        if (material->shader == SOLID_ANIMATED_MATERIAL || material->shader == NORMALMAPPING_ANIMATED_MATERIAL || material->shader == ALPHA_TEST_ANIMATED_MATERIAL)
        {
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, mesh->vertexSize, (void*)(sizeof(float) * 14));

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mesh->vertexSize, (void*)(sizeof(float) * 14 + sizeof(int) * 4));

            SceneObject* sceneObject = i->object;

            const std::vector<glm::mat4>& finalMatrices = getFinalMatrices(sceneObject);

            for (int i = 0; i < MAX_BONES; ++i)
            {
                const glm::mat4& matrix = finalMatrices[i];

                shader->setUniform(shader->getUniformLocation(("finalBonesMatrices[" + Strings::toString(i) + "]").c_str()), matrix);
            }
        }

        if (material->diffuseTexture != NULL)
            shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_DIFFUSE_TEXTURE], material->diffuseTexture);

		if (material->diffuseTexture != NULL)
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_ALBEDO_TEXTURE], material->diffuseTexture);
		if (material->metalicTexture != NULL)
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_METALIC_TEXTURE], material->metalicTexture);
		if (material->roughnessTexture != NULL)
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_ROUGHNESS_TEXTURE], material->roughnessTexture);
		if (material->aoTexture != NULL)
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_AO_TEXTURE], material->aoTexture);
		if (material->emissiveTexture != NULL)
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_EMISSIVE_TEXTURE], material->emissiveTexture);

		if (material->shader == PBR_MATERIAL || material->shader == PBR_TREE_MATERIAL)
		{
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_IRRADIANCE_MAP], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getIrradianceMap());
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_SPECULAR_IRRADIANCE_MAP], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getSpecularIrradianceMap());
			shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_BRDF_LUT], _brdfLutTexture);
		}
		if (material->shader == SKY_MATERIAL)
		{
			//shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_DIFFUSE_TEXTURE], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getIrradianceMap());
			//shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_DIFFUSE_TEXTURE], _graphicsManager->getGlobalEnvironmentCaptureComponent()->getSpecularIrradianceMap());
		}
        if (material->shader == DECAL_MATERIAL)
        {
            glm::mat4 viewProjMatrixInv = glm::inverse(_mainRenderData->camera->getProjectionMatrix() * _mainRenderData->camera->getViewMatrix());
            glm::mat4 modelMatrixInv = glm::inverse(modelMatrix);

            shader->bindTexture(shader->getUniformLocation("depthMap"), _depthFramebuffer->getTexture(0));
            shader->setUniform(shader->getUniformLocation("viewProjMatrixInv"), viewProjMatrixInv);
            shader->setUniform(shader->getUniformLocation("modelMatrixInv"), modelMatrixInv);
        }

		shader->setUniform(_uniformsLocations[currentShader][UNIFORM_COLOR_1], color1);
		shader->setUniform(_uniformsLocations[currentShader][UNIFORM_COLOR_2], color2);
		shader->setUniform(_uniformsLocations[currentShader][UNIFORM_COLOR_3], color3);
		shader->setUniform(_uniformsLocations[currentShader][UNIFORM_COLOR_4], color4);

		shader->setUniform(_uniformsLocations[currentShader][UNIFORM_GRASS_WIND_TIMER], _graphicsManager->getWindTimer());

        shader->setUniform(_uniformsLocations[currentShader][UNIFORM_OBJECT_ID], i->object->getId());

        if (_renderObjectIdsForPicking && !i->renderObject->isRenderObjectId())
        {
            glColorMaski(2, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        }

        if (i->type != RET_GRASS)
        {
            glDrawElements(model->getPrimitiveType(),
                           mesh->indicesCount,
                           GL_UNSIGNED_INT,
                           (void*)(mesh->firstVertex * sizeof(unsigned int)));
        }
        else
        {
            shader->setUniform(_uniformsLocations[currentShader][UNIFORM_VP], renderData->MVMatrix);

            glDisable(GL_CULL_FACE);

			Grass* grass = static_cast<Grass*>(i->renderObject);
			float renderingDistance = grass->getRenderingDistance();

            Frustum frustum(glm::perspective(camera->getViewAngle(), (float)camera->getWindowWidth() / (float)camera->getWindowHeight(),
                                     camera->getNearValue(), renderingDistance) * camera->getViewMatrix());
            AABB* aabb = frustum.getAABB();
            glm::vec3 min = aabb->getMinCoords();
            min = glm::vec3((float)((int)(min.x / 0.5)) * 0.5, (float)min.y, (float)((int)(min.z / 0.5)) * 0.5);
            glm::vec3 max = aabb->getMaxCoords();
            max = glm::vec3((int)max.x, (int)max.y, (int)max.z);
            float width = (max.x - min.x) / 0.5;
            float height = (max.z - min.z) / 0.5;
            float a = width * height;

            shader->setUniform(_uniformsLocations[currentShader][UNIFORM_GRASS_COLOR], grass->getGrassColor());
            shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_HEIGHTMAP], grass->getTerrainHeightmap());
            shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_GRASS_DENSITY], grass->getGrassDensityTexture());
            shader->setUniform(_uniformsLocations[currentShader][UNIFORM_GRASS_MIN], min);
            shader->setUniform(_uniformsLocations[currentShader][UNIFORM_GRASS_WIDTH], (int)width);

			std::vector<RTexture2D*>& grassTextures = grass->getAdditionalRandomGrassTextures();
			std::vector<float>& grassTexturesScale = grass->getAdditionalRandomGrassTexturesScale();
			for (int i = 0; i < grassTextures.size(); ++i)
			{
				shader->bindTexture(_uniformsLocations[currentShader][UNIFORM_GRASS_TEXTURE_1 + i], grassTextures[i]);
				shader->setUniform(_uniformsLocations[currentShader][UNIFORM_GRASS_TEXTURE_SCALE_1 + i], grassTexturesScale[i]);
			}
			shader->setUniform(_uniformsLocations[currentShader][UNIFORM_GRASS_TEXTURES_COUNT], (int)grassTextures.size());

            glDrawElementsInstanced(model->getPrimitiveType(),
                                mesh->indicesCount,
                                GL_UNSIGNED_INT,
                                (void*)(mesh->firstVertex * sizeof(unsigned int)),
                                (int)a);

            glEnable(GL_CULL_FACE);
        }

        if (_renderObjectIdsForPicking && !i->renderObject->isRenderObjectId())
        {
            glColorMaski(2, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        if (material->normalmapTexture != 0)
        {
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);
        }
        if (material->shader == SOLID_ANIMATED_MATERIAL || material->shader == NORMALMAPPING_ANIMATED_MATERIAL || material->shader == ALPHA_TEST_ANIMATED_MATERIAL)
        {
            glDisableVertexAttribArray(5);
            glDisableVertexAttribArray(6);
        }
    }
    glEnable(GL_CULL_FACE);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    // -----------------DEBUG----------------------------------------
    #ifdef DRAW_AABB
    RShader* shader = _shaderList[DEBUG_SHADER];
    shader->enable();
    glPointSize(50);
    glLineWidth(2);
    _aabbVbo->bind();

    if (_renderObjectsAAABB)
    {
        for (std::vector<RenderObject*>::iterator i = _renderObjectsInCurrentFrame.begin(); i != _renderObjectsInCurrentFrame.end(); ++i)
        {
            RenderObject* renderObject = *i;

            glm::vec3 min = renderObject->getAABB()->getMinCoords();
            glm::vec3 max = renderObject->getAABB()->getMaxCoords();

            glm::vec3 vertices[8];
            vertices[0] = glm::vec3(min.x, min.y, min.z);
            vertices[1] = glm::vec3(min.x, min.y, max.z);
            vertices[2] = glm::vec3(min.x, max.y, min.z);
            vertices[3] = glm::vec3(min.x, max.y, max.z);
            vertices[4] = glm::vec3(max.x, min.y, min.z);
            vertices[5] = glm::vec3(max.x, min.y, max.z);
            vertices[6] = glm::vec3(max.x, max.y, min.z);
            vertices[7] = glm::vec3(max.x, max.y, max.z);

            shader->setUniform(_uniformsLocations[DEBUG_SHADER][UNIFORM_MVP], _mainRenderData->camera->getProjectionMatrix() * _mainRenderData->camera->getViewMatrix());
            shader->setUniform(_uniformsLocations[DEBUG_SHADER][UNIFORM_MATERIAL_EMISSIVE_COLOR], glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));

            for (int i = 0; i < 8; ++i)
                shader->setUniform(_uniformsLocations[DEBUG_SHADER][UNIFORM_DEBUG_VERTEX_INDEX_1 + i], vertices[i]);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glDrawArrays(GL_LINES, 0, 24);

            glDisableVertexAttribArray(0);
        }
    }
    if (_renderObjectsOBB)
    {
        for (std::vector<RenderObject*>::iterator i = _renderObjectsInCurrentFrame.begin(); i != _renderObjectsInCurrentFrame.end(); ++i)
        {
            RenderObject* renderObject = *i;

            glm::vec3 min = renderObject->getModel()->getAABB()->getMinCoords();
            glm::vec3 max = renderObject->getModel()->getAABB()->getMaxCoords();

            glm::vec3 vertices[8];
            vertices[0] = glm::vec3(min.x, min.y, min.z);
            vertices[1] = glm::vec3(min.x, min.y, max.z);
            vertices[2] = glm::vec3(min.x, max.y, min.z);
            vertices[3] = glm::vec3(min.x, max.y, max.z);
            vertices[4] = glm::vec3(max.x, min.y, min.z);
            vertices[5] = glm::vec3(max.x, min.y, max.z);
            vertices[6] = glm::vec3(max.x, max.y, min.z);
            vertices[7] = glm::vec3(max.x, max.y, max.z);

            shader->setUniform(_uniformsLocations[DEBUG_SHADER][UNIFORM_MVP], _mainRenderData->camera->getProjectionMatrix() * _mainRenderData->camera->getViewMatrix() * renderObject->getSceneObject()->getGlobalTransformMatrix());
            shader->setUniform(_uniformsLocations[DEBUG_SHADER][UNIFORM_MATERIAL_EMISSIVE_COLOR], glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));

            for (int i = 0; i < 8; ++i)
                shader->setUniform(_uniformsLocations[DEBUG_SHADER][UNIFORM_DEBUG_VERTEX_INDEX_1 + i], vertices[i]);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glDrawArrays(GL_LINES, 0, 24);

            glDisableVertexAttribArray(0);
        }
    }
    _renderObjectsInCurrentFrame.clear();
    #endif
}


/*void Renderer::renderGUI(GUIRenderList* renderList);//std::list<GUIObject*>* GUIObjectsList)
{
    glDisable(GL_DEPTH_TEST);

    RShader* shader = _shaderList[GUI_SHADER];
    shader->enable();

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight, 1.0f, -1.5f);
*/
    /*for (std::list<GUIObject*>::iterator i = GUIObjectsList->begin(); i != GUIObjectsList->end(); ++i)
    {
        GUIObject* object = *i;

        shader->setUniform("VerticesTransformMatrix", projectionMatrix * object->getVerticesTransformMatrix());
        shader->setUniform("TexCoordTransformMatrix", object->getTexCoordTransformMatrix());

        object->getVBO()->bind();


        RTexture* texture = static_cast<Image*>(object)->getTexture();      // Dodac rozroznianie typow obiektow
        shader->bindTexture2D("Texture", texture);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableVertexAttribArray(0);
    }*/


    /*shader->setUniform("VerticesTransformMatrix", projectionMatrix);
    shader->setUniform("TexCoordTransformMatrix", glm::mat4(1.0f));

    _font->_vbo->bind();

    //RTexture* texture = ResourceManager::getInstance().loadTexture("crate.jpg");
    RTexture* texture = _font->_characterTextures[static_cast<int>('g')];      // Dodac rozroznianie typow obiektow
    shader->bindTexture2D("Texture", texture);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)(sizeof(float) * 3));

    glDrawArrays(GL_TRIANGLE_STRIP, static_cast<int>('g') * 4, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);*/


    /*Label* label = new Label(_font);
    label->setText("Hello world");
    label->setPosition(100, 600);
    //label->setRotation(0.25 * 3.14);
    label->setScale(1, 1);

    for (int i = 0; i < label->getQuantumOfVBO(); ++i)
    {

        shader->setUniform("VerticesTransformMatrix", projectionMatrix * label->getVerticesTransformMatrix(i));
        shader->setUniform("TexCoordTransformMatrix", glm::mat4(1.0f));

        label->getVBO(i)->bind();

        //RTexture* texture = ResourceManager::getInstance().loadTexture("crate.jpg");
        RTexture* texture = _font->_characterTextures[static_cast<int>(label->getText()[i])];      // Dodac rozroznianie typow obiektow
        shader->bindTexture2D("Texture", texture);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)(sizeof(float) * 3));

        glDrawArrays(GL_TRIANGLE_STRIP, static_cast<int>(label->getText()[i]) * 4, 4);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }


    delete GUIObjectsList;
    glEnable(GL_DEPTH_TEST);
}*/


void Renderer::renderGUI(GUIRenderList* renderList)//std::list<GUIObject*>* GUIObjectsList)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight, 1.0f, -1.5f);

    for (GUIRenderList::iterator i = renderList->begin(); i != renderList->end(); ++i)
    {
		ShaderType shaderType = i->getShaderType();
        RShader* shader = _shaderList[shaderType];
        shader->enable();

        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_GUI_VERTICES_TRANSFORM_MATRIX], projectionMatrix * i->getVerticesTransformMatrix());
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_GUI_TEXCOORDS_TRANSFORM_MATRIX], i->getTexCoordTransformMatrix());
        shader->setUniform(_uniformsLocations[shaderType][UNIFORM_GUI_COLOR], i->getColor());

        for (const auto& uniform : i->getCustomUniforms())
        {
            // todo: refactor
            GLint location = shader->getUniformLocation(uniform.first.c_str());
            shader->setUniform(location, uniform.second);
        }

        i->getVBO()->bind();

        if (i->getTexture() != nullptr)
        {
            shader->bindTexture(_uniformsLocations[shaderType][UNIFORM_DIFFUSE_TEXTURE], i->getTexture());
        }

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)(sizeof(float) * 3));

        glDrawArrays(GL_TRIANGLE_STRIP, i->getFirstVertex(), 4);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }


    /*shader->setUniform("VerticesTransformMatrix", projectionMatrix);
    shader->setUniform("TexCoordTransformMatrix", glm::mat4(1.0f));

    _font->_vbo->bind();

    //RTexture* texture = ResourceManager::getInstance().loadTexture("crate.jpg");
    RTexture* texture = _font->_characterTextures[static_cast<int>('g')];      // Dodac rozroznianie typow obiektow
    shader->bindTexture2D("Texture", texture);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)(sizeof(float) * 3));

    glDrawArrays(GL_TRIANGLE_STRIP, static_cast<int>('g') * 4, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);*/


    /*Label* label = new Label(_font);
    label->setText("Hello world");
    label->setPosition(100, 600);
    //label->setRotation(0.25 * 3.14);
    label->setScale(1, 1);

    for (int i = 0; i < label->getQuantumOfVBO(); ++i)
    {

        shader->setUniform("VerticesTransformMatrix", projectionMatrix * label->getVerticesTransformMatrix(i));
        shader->setUniform("TexCoordTransformMatrix", glm::mat4(1.0f));

        label->getVBO(i)->bind();

        //RTexture* texture = ResourceManager::getInstance().loadTexture("crate.jpg");
        RTexture* texture = _font->_characterTextures[static_cast<int>(label->getText()[i])];      // Dodac rozroznianie typow obiektow
        shader->bindTexture2D("Texture", texture);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GUIVertex), (void*)(sizeof(float) * 3));

        glDrawArrays(GL_TRIANGLE_STRIP, static_cast<int>(label->getText()[i]) * 4, 4);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }*/


    delete renderList;
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}


unsigned int Renderer::pickObject(int x, int y)
{
    if (!_pickingComputeShader)
    {
        LOG_WARNING("Graphics object picking is disabled.");
        return 0;
    }

    RTexture* texture = _mainRenderData->framebuffer->getTexture(2);

    unsigned int id = 0;

#ifdef OBJECT_PICKING_GET_TEX_IMAGE
    {
        Timer timer;
        timer.start();

        glBindTexture(GL_TEXTURE_2D, texture->getID());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, _objectsIdsTextureData);

        id = _objectsIdsTextureData[y * texture->getSize().x + x];

        double pickingTime = timer.stop();
        LOG_DEBUG(LOG_VARIABLE(pickingTime));
    }
#endif // OBJECT_PICKING_GET_TEX_IMAGE

#ifdef OBJECT_PICKING_STORAGE_BUFFER
    {
        Timer timer;
        timer.start();

        PickingShaderStorageBufferData shaderData;
        shaderData.x = static_cast<float>(x) / static_cast<float>(_screenWidth);
        shaderData.y = static_cast<float>(y) / static_cast<float>(_screenHeight);

        _pickingComputeShader->enable();
        _pickingComputeShader->bindTexture(_pickingComputeShader->getUniformLocation("textureObjectsIds"), texture);
        _pickingComputeShader->setShaderStorageBlockBinding("ShaderData", 0);
        _pickingSSBO->bindBufferBase(0);
        _pickingSSBO->setData(shaderData);

        _pickingComputeShader->runComputeShader(1, 1, 1);

        _pickingSSBO->getBufferData(shaderData);

        id = shaderData.selectedObject;

        double pickingTime = timer.stop();
        LOG_DEBUG(LOG_VARIABLE(pickingTime));
    }
#endif // OBJECT_PICKING_STORAGE_BUFFER

    LOG_DEBUG("Selected object " + LOG_VARIABLE(id));

    return id;
}
