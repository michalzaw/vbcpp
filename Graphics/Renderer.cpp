#include "Renderer.h"

#include "../Utils/Helpers.hpp"


static std::unique_ptr<Renderer> rendererInstance;


Renderer::Renderer()
    : _isInitialized(false),
    _screenWidth(0), _screenHeight(0),
    _alphaToCoverage(true), _exposure(0.05f),
    _msaaAntialiasing(false), _msaaAntialiasingLevel(8),
    _bloom(false),
    _isShadowMappingEnable(false), _shadowMap(NULL),
    _mainRenderData(NULL),
    _renderObjectsAAABB(true), _renderObjectsOBB(false)
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


void Renderer::prepareLightsData()
{
    GraphicsManager& graphicsManager = GraphicsManager::getInstance();


    // Aktualizacja UBO swiatel
    int d = 0;
    int p = 0;
    int s = 0;
    for (std::list<Light*>::iterator i = graphicsManager._lights.begin(); i != graphicsManager._lights.end(); ++i)
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
    if (_isShadowMappingEnable)
    {
        float cascadeEnd[4] = {0.1, 25.0f, 100.0f, 500.0f };

        for (int i = 0; i < 3; ++i)
        {
            CameraStatic* cameraForShadowMap = _shadowMap->getCameraForShadowMap(i);
            CameraStatic* currentCamera = graphicsManager.getCurrentCamera();

            Frustum frustum(glm::perspective(currentCamera->getViewAngle(), (float)currentCamera->getWindowWidth() / (float)currentCamera->getWindowHeight(),
                                             cascadeEnd[i], cascadeEnd[i + 1]) * currentCamera->getViewMatrix());

            frustum.applyTransform(cameraForShadowMap->getViewMatrix());
            AABB* frustumAabb = frustum.getAABB();
            glm::vec3 min = frustumAabb->getMinCoords();
            glm::vec3 max = frustumAabb->getMaxCoords();

            cameraForShadowMap->setOrthoProjectionParams(min.x, max.x, min.y, max.y, -max.z - 100.0f, -min.z);
        }
    }
}


void Renderer::addStaticModelNodeToRenderList(ModelNode* modelNode, RenderListElement& tempRenderElement, std::list<RenderListElement>& renderList, RenderPass renderPass, glm::mat4 parentTransform, glm::mat4 parentNormalMatrix)
{
    glm::mat4 t = parentTransform * modelNode->getTransformMatrix();
    glm::mat4 n = parentNormalMatrix * modelNode->getNormalMatrix();
    tempRenderElement.transformMatrix = t;
    tempRenderElement.normalMatrix = n;

    for (int k = 0; k < modelNode->getMeshesCount(); ++k)
    {
        tempRenderElement.mesh = modelNode->getMesh(k);
        tempRenderElement.material = tempRenderElement.renderObject->getMaterial(tempRenderElement.mesh->materialIndex);
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
        addStaticModelNodeToRenderList(modelNode->getChildren()[i], tempRenderElement, renderList, renderPass, t, n);
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
        tempRenderElement.material = tempRenderElement.renderObject->getMaterial(tempRenderElement.mesh->materialIndex);
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
    GraphicsManager& graphicsManager = GraphicsManager::getInstance();


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
    for (int i = 0; i < graphicsManager._mirrorComponents.size(); ++i)
    {
        Frustum frustum(_mainRenderData->camera->getProjectionMatrix() * _mainRenderData->camera->getViewMatrix());
        if (isPointInFrustum(frustum, graphicsManager._mirrorComponents[i]->getPosition()))
        {
            graphicsManager._mirrorComponents[i]->calculateReflectionVectorAndRotateCamera(graphicsManager.getCurrentCamera()->getPosition());
            RenderData* renderData = new RenderData;
            renderData->camera = graphicsManager._mirrorComponents[i];
            renderData->framebuffer = graphicsManager._mirrorComponents[i]->getFramebuffer();
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

    for (std::list<RenderObject*>::iterator i = graphicsManager._renderObjects.begin(); i != graphicsManager._renderObjects.end(); ++i)
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
                tempRenderElement.model = object->getModel();
                tempRenderElement.object = object->getSceneObject();
                tempRenderElement.renderObject = object;

                ModelNode* modelNode = tempRenderElement.renderObject->getModelRootNode();
                addStaticModelNodeToRenderList(modelNode, tempRenderElement, _renderDataList[j]->renderList, renderPass);
            }
        }
#ifdef DRAW_AABB
        _renderObjectsInCurrentFrame.push_back(*i);
#endif // DRAW_AABB
    }

    for (std::list<Grass*>::iterator i = graphicsManager._grassComponents.begin(); i != graphicsManager._grassComponents.end(); ++i)
    {
        RenderObject* object = *i;

        tempRenderElement.type = RET_GRASS;
        tempRenderElement.model = object->getModel();
        tempRenderElement.object = object->getSceneObject();
        tempRenderElement.renderObject = object;

        ModelNode* modelNode = tempRenderElement.renderObject->getModelRootNode();
        addGrassStaticModelNodeToRenderList(modelNode, tempRenderElement, _renderDataList[_renderDataList.size() - 1]->renderList);
    }

    _renderDataList[_renderDataList.size() - 1]->renderList.sort(compareByShader);
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
    for (int i = 0; i < shadowMap->CASCADE_COUNT; ++i)
    {
        RenderData* renderData = new RenderData;
        renderData->camera = shadowMap->getCameraForShadowMap(i);
        renderData->framebuffer = shadowMap->getShadowMap(i);
        renderData->renderPass = RP_SHADOWS;
        _renderDataListForShadowmapping.insert(_renderDataListForShadowmapping.begin(), renderData);
    }
}


void Renderer::deleteRenderDatasForShadowMap(ShadowMap* shadowMap)
{
    for (int i = 0; i < shadowMap->CASCADE_COUNT; ++i)
    {
        for (std::vector<RenderData*>::iterator j = _renderDataListForShadowmapping.begin(); j != _renderDataListForShadowmapping.end(); ++j)
        {
            if (shadowMap->getShadowMap(i) == (*j)->framebuffer)
            {
                RenderData* renderData = (*j);
                _renderDataListForShadowmapping.erase(j);
                delete renderData;

                break;
            }
        }
    }
}


void Renderer::init(unsigned int screenWidth, unsigned int screenHeight)
{
    if (_isInitialized)
        return;


    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    _defaultFramebuffer = OGLDriver::getInstance().getDefaultFramebuffer();
    _defaultFramebuffer->setViewport(UintRect(0, 0, _screenWidth, _screenHeight));

    Framebuffer* framebuffer = OGLDriver::getInstance().createFramebuffer();
    framebuffer->addDepthRenderbuffer(_screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
    framebuffer->addTexture(TF_RGBA_32F, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
    framebuffer->addTexture(TF_RGBA_32F, _screenWidth, _screenHeight, _msaaAntialiasing, _msaaAntialiasingLevel);
    framebuffer->getTexture(0)->setFiltering(TFM_NEAREST, TFM_NEAREST);
    framebuffer->getTexture(1)->setFiltering(TFM_NEAREST, TFM_NEAREST);
    framebuffer->setViewport(UintRect(0, 0, _screenWidth, _screenHeight));

	for (int i = 0; i < 2; ++i)
	{
		_postProcessingFramebuffers[i] = OGLDriver::getInstance().createFramebuffer();
		_postProcessingFramebuffers[i]->addTexture(TF_RGBA_32F, screenWidth, _screenHeight, false);
		_postProcessingFramebuffers[i]->getTexture(0)->setFiltering(TFM_LINEAR, TFM_LINEAR);
		_postProcessingFramebuffers[i]->setViewport(UintRect(0, 0, _screenWidth, _screenHeight));
	}

    /*for (int i = 0; i < 2; ++i)
    {
        _blurFramebuffers[i] = OGLDriver::getInstance().createFramebuffer();
        _blurFramebuffers[i]->addTexture(TF_RGBA_32F, screenWidth / 10.0f, _screenHeight / 10.0f, false);
        _blurFramebuffers[i]->getTexture(0)->setFiltering(TFM_LINEAR, TFM_LINEAR);
        _blurFramebuffers[i]->setViewport(UintRect(0, 0, _screenWidth / 10.0f, _screenHeight / 10.0f));
    }*/


    _mainRenderData = new RenderData;
    _mainRenderData->camera = GraphicsManager::getInstance().getCurrentCamera();
    _mainRenderData->framebuffer = framebuffer;
    _mainRenderData->renderPass = RP_NORMAL;




	std::vector<std::string> defines;
	std::unordered_map<std::string, std::string> constants;


	// msaa
	if (_msaaAntialiasing)
	{
		constants["samplesCount"] = toString(_msaaAntialiasingLevel);
		PostProcessingEffect* postProcessingMsaa = new PostProcessingEffect(PPT_MSAA, _quadVBO, ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingMsaa.frag", defines, constants));
		//_postProcessingEffectsStack.push_back(postProcessingMsaa);
		addPostProcessingEffect(postProcessingMsaa);
	}

	// bloom
	if (_bloom)
	{
		PostProcessingBloom* postProcessingBloom = new PostProcessingBloom(_quadVBO, ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingBloom.frag"), _screenWidth, _screenHeight, _mainRenderData->framebuffer->getTexture(1), _msaaAntialiasing, _msaaAntialiasingLevel);
		addPostProcessingEffect(postProcessingBloom);
	}

	// tone mapping
	PostProcessingToneMapping* postProcessingToneMapping = new PostProcessingToneMapping(_quadVBO, ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingToneMapping.frag"));
	postProcessingToneMapping->setExposure(_exposure);
	//_postProcessingEffectsStack.push_back(postProcessingToneMapping);
	addPostProcessingEffect(postProcessingToneMapping);


    _shaderList.resize(NUMBER_OF_SHADERS);

    // Load shaders
    // SOLID_MATERIAL
    defines.push_back("SOLID");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
	_shaderList[SOLID_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // NOTEXTURE_MATERIAL
    _shaderList[NOTEXTURE_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader_notexture.frag", defines);

    // NORMALMAPPING_MATERIAL
    defines.clear();
    defines.push_back("NORMALMAPPING");
    defines.push_back("SOLID");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    _shaderList[NORMALMAPPING_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // CAR_PAINT_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("CAR_PAINT");
    defines.push_back("REFLECTION");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
	_shaderList[CAR_PAINT_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // MIRROR_MATERIAL
	_shaderList[MIRROR_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/mirror.frag");

    // ALPHA_TEST_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    _shaderList[ALPHA_TEST_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // TREE_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    _shaderList[TREE_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/tree.vert", "Shaders/shader.frag", defines);

    // GRASS_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    defines.push_back("GRASS");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    _shaderList[GRASS_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/grass.vert", "Shaders/shader.frag", defines);

    // SKY_MATERIAL
    _shaderList[SKY_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/sky.vert", "Shaders/sky.frag");

    // GLASS_MATERIAL
    defines.clear();
    defines.push_back("GLASS");
    defines.push_back("REFLECTION");
    //defines.push_back("TRANSPARENCY");
    //if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    _shaderList[GLASS_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

    // GUI_IMAGE_SHADER
    _shaderList[GUI_IMAGE_SHADER] = ResourceManager::getInstance().loadShader("Shaders/GUIshader.vert", "Shaders/GUIshader.frag");

    // GUI_LABEL_SHADER
    _shaderList[GUI_LABEL_SHADER] = ResourceManager::getInstance().loadShader("Shaders/GUIshader.vert", "Shaders/LabelShader.frag");

    // DEBUG_SHADER
    _shaderList[DEBUG_SHADER] = ResourceManager::getInstance().loadShader("Shaders/debug.vert", "Shaders/debug.frag");

    // SHADOWMAP_SHADER
    _shaderList[SHADOWMAP_SHADER] = ResourceManager::getInstance().loadShader("Shaders/shadowmap.vert", "Shaders/shadowmap.frag");

    // SHADOWMAP_ALPHA_TEST_SHADER
    defines.clear();
    defines.push_back("ALPHA_TEST");
    _shaderList[SHADOWMAP_ALPHA_TEST_SHADER] = ResourceManager::getInstance().loadShader("Shaders/shadowmap.vert", "Shaders/shadowmap.frag", defines);

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

    // BLUR_SHADER_MSAA
    defines.clear();
    defines.push_back(_msaaAntialiasing ? "MULTISAMPLE" : "NOT_MULTISAMPLE");
	constants.clear();
    constants["samplesCount"] = toString(_msaaAntialiasingLevel);
    _shaderList[BLUR_SHADER_MSAA] = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/blur.frag", defines, constants);

    // BLUR_SHADER
    defines.clear();
    defines.push_back("NOT_MULTISAMPLE");
    constants.clear();
    constants["samplesCount"] = toString(_msaaAntialiasingLevel);
    _shaderList[BLUR_SHADER] = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/blur.frag", defines, constants);

    // QUAD_SHADER
    defines.clear();
    defines.push_back(_msaaAntialiasing ? "MULTISAMPLE" : "NOT_MULTISAMPLE");
    constants.clear();
    constants["samplesCount"] = toString(_msaaAntialiasingLevel);
    _shaderList[QUAD_SHADER] = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/quad.frag", defines, constants);

    // EDITOR_AXIS_SHADER
    _shaderList[EDITOR_AXIS_SHADER] = _shaderList[SOLID_MATERIAL];

    _shaderListForMirrorRendering.resize(NUMBER_OF_SHADERS);
    _shaderListForMirrorRendering[SOLID_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[NOTEXTURE_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[NORMALMAPPING_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[CAR_PAINT_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[MIRROR_MATERIAL] = MIRROR_SOLID_MATERIAL;
    _shaderListForMirrorRendering[ALPHA_TEST_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[TREE_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[GRASS_MATERIAL] = MIRROR_ALPHA_TEST_MATERIAL;
    _shaderListForMirrorRendering[SKY_MATERIAL] = SKY_MATERIAL;
    _shaderListForMirrorRendering[GLASS_MATERIAL] = MIRROR_GLASS_MATERIAL;


    // Create UBO for lights
    _lightUBO = OGLDriver::getInstance().createUBO(4096);
    _lightUBO->bindBufferBase(0);

    _shaderList[SOLID_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);
    _shaderList[NORMALMAPPING_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);


    _isInitialized = true;
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
			PostProcessingBloom* postProcessingBloom = new PostProcessingBloom(_quadVBO, ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/postProcessingBloom.frag"), _screenWidth, _screenHeight, _mainRenderData->framebuffer->getTexture(1), _msaaAntialiasing, _msaaAntialiasingLevel);
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
    if (_isShadowMappingEnable && _shadowMap == NULL)
    {
        _shadowMap = shadowMap;

        createRenderDatasForShadowMap(shadowMap);
    }
}


void Renderer::unregisterShadowMap(ShadowMap* shadowMap)
{
    if (_shadowMap == shadowMap)
    {
        _shadowMap = NULL;

        deleteRenderDatasForShadowMap(shadowMap);
    }
}


void Renderer::setCurrentMainCamera(CameraStatic* camera)
{
    //_renderDataList[_renderDataList.size() - 1]->camera = camera;
    _mainRenderData->camera = camera;
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


void Renderer::renderAll()
{
    prepareLightsData();
    prepareRenderData();

    for (int i = 0; i < _renderDataList.size(); ++i)
    {
        if (_renderDataList[i]->renderPass == RP_SHADOWS)
            renderDepth(_renderDataList[i]);
        else if (_renderDataList[i]->renderPass == RP_MIRROR)
            renderToMirrorTexture(_renderDataList[i]);
        else // RP_NORMAL
            renderScene(_renderDataList[i]);
    }


    glDisable(GL_DEPTH_TEST);







    // blur
    /*if (_bloom)
    {
        bool isHorizontal = true;
        bool isFirstIteration = true;
        int amount = 10;
        ShaderType shaderType;
        for (unsigned int i = 0; i < amount; i++)
        {
            _blurFramebuffers[isHorizontal]->bind();
            glClear(GL_COLOR_BUFFER_BIT);

            if (isFirstIteration)
                shaderType = BLUR_SHADER_MSAA;
            else
                shaderType = BLUR_SHADER;

            _shaderList[shaderType]->enable();

            _shaderList[shaderType]->setUniform(UNIFORM_BLUR_IS_HORIZONTAL, isHorizontal);

            RTexture* texture = isFirstIteration ? _mainRenderData->framebuffer->getTexture(1) : _blurFramebuffers[!isHorizontal]->getTexture(0);
            _shaderList[shaderType]->bindTexture(UNIFORM_DIFFUSE_TEXTURE, texture);


            _quadVBO->bind();

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(0);


            isHorizontal = !isHorizontal;
            if (isFirstIteration)
                isFirstIteration = false;
        }
    }



    // render quad on screen
    _defaultFramebuffer->bind();
	//_postProcessingFramebuffers[1]->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _quadVBO->bind();

    _shaderList[QUAD_SHADER]->enable();
    _shaderList[QUAD_SHADER]->bindTexture(UNIFORM_DIFFUSE_TEXTURE, t == 0 ? _mainRenderData->framebuffer->getTexture(0) : _blurFramebuffers[0]->getTexture(0));
    _shaderList[QUAD_SHADER]->bindTexture(UNIFORM_BLOOM_TEXTURE, _blurFramebuffers[0]->getTexture(0));
    _shaderList[QUAD_SHADER]->setUniform(UNIFORM_TONEMAPPING_EXPOSURE, _exposure);
    _shaderList[QUAD_SHADER]->setUniform(UNIFORM_BLOOM_RATIO, _bloom ? 1.0f : 0.0f);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);*/



	for (int i = 0; i < _postProcessingEffectsStack.size(); ++i)
	{
		RTexture* input;
		if (i == 0)
			input = _mainRenderData->framebuffer->getTexture(0);
			//input = _postProcessingFramebuffers[1]->getTexture(0);
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
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    ShaderType currentShader = GUI_IMAGE_SHADER;
    RShader* shader;
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RStaticModel* model = i->model;
        StaticModelMesh* mesh = i->mesh;
        Material* material = i->material;

        ShaderType shaderType;
        bool isAlphaTest = material->shader == ALPHA_TEST_MATERIAL || material->shader == TREE_MATERIAL;
        if (isAlphaTest)
            shaderType = SHADOWMAP_ALPHA_TEST_SHADER;
        else
            shaderType = SHADOWMAP_SHADER;

        if (shaderType != currentShader)
        {
            shader = _shaderList[shaderType];
            shader->enable();
            currentShader = shaderType;
        }
        else
        {
            shader->resetTextureLocation();
        }

        glm::mat4 modelMatrix = i->object->getGlobalTransformMatrix() * i->transformMatrix;
        glm::mat4 MVP = renderData->MVMatrix * modelMatrix;
        shader->setUniform(UNIFORM_MVP, MVP);

        mesh->vbo->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        if (isAlphaTest)
        {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

            shader->bindTexture(UNIFORM_ALPHA_TEXTURE, material->diffuseTexture);
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
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}


void Renderer::renderToMirrorTexture(RenderData* renderData)
{
    renderData->framebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    ShaderType currentShader = NUMBER_OF_SHADERS;
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RStaticModel* model = i->model;
        StaticModelMesh* mesh = i->mesh;
        Material* material = i->material;

        ShaderType shaderType = _shaderListForMirrorRendering[material->shader];
        RShader* shader = _shaderList[shaderType];
        if (shaderType != currentShader)
        {
            shader->enable();

            if (currentShader == SKY_MATERIAL)
            {
                glEnable(GL_CULL_FACE);
            }
            else if (currentShader == MIRROR_ALPHA_TEST_MATERIAL)
            {
                glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (currentShader == MIRROR_GLASS_MATERIAL)
            {
                glDisable(GL_BLEND);
            }

            if (shaderType == SKY_MATERIAL)
            {
                glDisable(GL_CULL_FACE);
            }
            else if (currentShader == MIRROR_ALPHA_TEST_MATERIAL)
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
                shader->bindTexture(UNIFORM_GLASS_TEXTURE, material->glassTexture);


            if (material->reflectionTexture1 == EMT_GLOBAL)
            {
                shader->bindTexture(UNIFORM_ENVIRONMENTMAP_TEXTURE, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(UNIFORM_ENVIRONMENTMAP_MATRIX, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(UNIFORM_ENVIRONMENTMAP_TEXTURE, static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(UNIFORM_ENVIRONMENTMAP_MATRIX, static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }

            if (material->reflectionTexture2 == EMT_GLOBAL)
            {
                shader->bindTexture(UNIFORM_ENVIRONMENTMAP_2_TEXTURE, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(UNIFORM_ENVIRONMENTMAP_2_MATRIX, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(UNIFORM_ENVIRONMENTMAP_2_TEXTURE, static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(UNIFORM_ENVIRONMENTMAP_2_MATRIX, static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }
        }


        shader->setUniform(UNIFORM_DAY_NIGHT_RATIO, _dayNightRatio);


        glm::mat4 modelMatrix = i->object->getGlobalTransformMatrix() * i->transformMatrix;
        glm::mat4 normalMatrix = i->object->getGlobalNormalMatrix() * i->normalMatrix;
        glm::mat4 MVP = renderData->MVMatrix * modelMatrix;
        shader->setUniform(UNIFORM_MVP, MVP);
        shader->setUniform(UNIFORM_MODEL_MATRIX, modelMatrix);
        shader->setUniform(UNIFORM_NORMAL_MATRIX, normalMatrix);

        shader->setUniform(UNIFORM_MATERIAL_AMBIENT_COLOR, material->ambientColor);
        shader->setUniform(UNIFORM_MATERIAL_DIFFUSE_COLOR, material->diffuseColor);
        shader->setUniform(UNIFORM_MATERIAL_SPECULAR_COLOR, material->specularColor);

        shader->setUniform(UNIFORM_MATERIAL_TRANSPARENCY, material->transparency);

        shader->setUniform(UNIFORM_MATERIAL_SPECULAR_POWER, material->shininess);
        shader->setUniform(UNIFORM_CAMERA_POSITION, camera->getPosition());


        mesh->vbo->bind();
        mesh->ibo->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));

        if (material->diffuseTexture != NULL)
            shader->bindTexture(UNIFORM_DIFFUSE_TEXTURE, material->diffuseTexture);


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
    }
    glEnable(GL_CULL_FACE);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_BLEND);
}


// UBO
void Renderer::renderScene(RenderData* renderData)
{
    glm::mat4 lightSpaceMatrix[3];
    if (_isShadowMappingEnable)
    {
        for (int j = 0; j < 3; ++j)
        {
            lightSpaceMatrix[j] = _shadowMap->getCameraForShadowMap(j)->getProjectionMatrix() * _shadowMap->getCameraForShadowMap(j)->getViewMatrix();
        }
    }

    renderData->framebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    ShaderType currentShader = NUMBER_OF_SHADERS;
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RStaticModel* model = i->model;
        StaticModelMesh* mesh = i->mesh;
        Material* material = i->material;

        RShader* shader = _shaderList[material->shader];
        if (material->shader != currentShader)
        {
            shader->enable();

            if (currentShader == SKY_MATERIAL)
            {
                glEnable(GL_CULL_FACE);
            }
            else if (currentShader == TREE_MATERIAL || currentShader == ALPHA_TEST_MATERIAL || currentShader == GRASS_MATERIAL)
            {
                glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (currentShader == GLASS_MATERIAL)
            {
                glDisable(GL_BLEND);
            }

            if (material->shader == SKY_MATERIAL)
            {
                glDisable(GL_CULL_FACE);
            }
            else if ((material->shader == TREE_MATERIAL || material->shader == ALPHA_TEST_MATERIAL || material->shader == GRASS_MATERIAL) && _alphaToCoverage)
            {
                glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (material->shader == GLASS_MATERIAL)
            {
                glEnable(GL_BLEND);
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
                shader->bindTexture(UNIFORM_GLASS_TEXTURE, material->glassTexture);


            if (material->reflectionTexture1 == EMT_GLOBAL)
            {
                shader->bindTexture(UNIFORM_ENVIRONMENTMAP_TEXTURE, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(UNIFORM_ENVIRONMENTMAP_MATRIX, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(UNIFORM_ENVIRONMENTMAP_TEXTURE, static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(UNIFORM_ENVIRONMENTMAP_MATRIX, static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }

            if (material->reflectionTexture2 == EMT_GLOBAL)
            {
                shader->bindTexture(UNIFORM_ENVIRONMENTMAP_2_TEXTURE, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getEnvironmentMap());
                shader->setUniform(UNIFORM_ENVIRONMENTMAP_2_MATRIX, GraphicsManager::getInstance().getGlobalEnvironmentCaptureComponent()->getRotationMatrix());
            }
            else
            {
                Component* c = i->object->getComponent(CT_ENVIRONMENT_CAPTURE_COMPONENT);
                if (c != NULL)
                {
                    shader->bindTexture(UNIFORM_ENVIRONMENTMAP_2_TEXTURE, static_cast<EnvironmentCaptureComponent*>(c)->getEnvironmentMap());
                    shader->setUniform(UNIFORM_ENVIRONMENTMAP_2_MATRIX, static_cast<EnvironmentCaptureComponent*>(c)->getRotationMatrix());
                }
            }
        }


        shader->setUniform(UNIFORM_DAY_NIGHT_RATIO, _dayNightRatio);


        if (material->shader == TREE_MATERIAL)
        {
            glm::vec3 d = GraphicsManager::getInstance().getWindVector();

            Component* treeComponent = i->object->getComponent(CT_TREE_COMPONENT);
            if (treeComponent != NULL)
            {
                d = static_cast<TreeComponent*>(treeComponent)->getWindVector();
            }

            shader->setUniform(UNIFORM_WIND_DIRECTION, d);
        }


        glm::mat4 modelMatrix = i->object->getGlobalTransformMatrix() * i->transformMatrix;
        glm::mat4 normalMatrix = i->object->getGlobalNormalMatrix() * i->normalMatrix;
        glm::mat4 MVP = renderData->MVMatrix * modelMatrix;
        shader->setUniform(UNIFORM_MVP, MVP);
        shader->setUniform(UNIFORM_MODEL_MATRIX, modelMatrix);
        shader->setUniform(UNIFORM_NORMAL_MATRIX, normalMatrix);

        shader->setUniform(UNIFORM_MATERIAL_AMBIENT_COLOR, material->ambientColor);
        shader->setUniform(UNIFORM_MATERIAL_DIFFUSE_COLOR, material->diffuseColor);
        shader->setUniform(UNIFORM_MATERIAL_SPECULAR_COLOR, material->specularColor);
        shader->setUniform(UNIFORM_MATERIAL_EMISSIVE_COLOR, material->emissiveColor);

        shader->setUniform(UNIFORM_MATERIAL_TRANSPARENCY, material->transparency);

        shader->setUniform(UNIFORM_MATERIAL_SPECULAR_POWER, material->shininess);
        shader->setUniform(UNIFORM_CAMERA_POSITION, camera->getPosition());
        //shader->setUniform("a", 1.0f);


        if (_isShadowMappingEnable)
        {
            shader->setUniform(UNIFORM_LIGHT_SPACE_MATRIX_1, lightSpaceMatrix[0]);
            shader->bindTexture(UNIFORM_SHADOW_MAP_1, _shadowMap->getShadowMap(0)->getTexture(0));
            shader->setUniform(UNIFORM_LIGHT_SPACE_MATRIX_2, lightSpaceMatrix[1]);
            shader->bindTexture(UNIFORM_SHADOW_MAP_2, _shadowMap->getShadowMap(1)->getTexture(0));
            shader->setUniform(UNIFORM_LIGHT_SPACE_MATRIX_3, lightSpaceMatrix[2]);
            shader->bindTexture(UNIFORM_SHADOW_MAP_3, _shadowMap->getShadowMap(2)->getTexture(0));
        }

        mesh->vbo->bind();
        mesh->ibo->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));

        if (material->normalmapTexture != NULL)
        {
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 11));

            shader->bindTexture(UNIFORM_NOTMALMAP_TEXTURE, material->normalmapTexture);
        }

        if (material->diffuseTexture != NULL)
            shader->bindTexture(UNIFORM_DIFFUSE_TEXTURE, material->diffuseTexture);



        if (i->type != RET_GRASS)
        {
            glDrawElements(model->getPrimitiveType(),
                           mesh->indicesCount,
                           GL_UNSIGNED_INT,
                           (void*)(mesh->firstVertex * sizeof(unsigned int)));
        }
        else
        {
            shader->setUniform(UNIFORM_VP, renderData->MVMatrix);

            glDisable(GL_CULL_FACE);

            Frustum frustum(glm::perspective(camera->getViewAngle(), (float)camera->getWindowWidth() / (float)camera->getWindowHeight(),
                                     camera->getNearValue(), 30.0f) * camera->getViewMatrix());
            AABB* aabb = frustum.getAABB();
            glm::vec3 min = aabb->getMinCoords();
            min = glm::vec3((float)((int)(min.x / 0.5)) * 0.5, (float)min.y, (float)((int)(min.z / 0.5)) * 0.5);
            glm::vec3 max = aabb->getMaxCoords();
            max = glm::vec3((int)max.x, (int)max.y, (int)max.z);
            float width = (max.x - min.x) / 0.5;
            float height = (max.z - min.z) / 0.5;
            float a = width * height;

            Grass* grass = static_cast<Grass*>(i->renderObject);
            shader->setUniform(UNIFORM_GRASS_COLOR, grass->getGrassColor());
            shader->bindTexture(UNIFORM_HEIGHTMAP, grass->getTerrainHeightmap());
            shader->bindTexture(UNIFORM_GRASS_DENSITY, grass->getGrassDensityTexture());
            shader->setUniform(UNIFORM_GRASS_MIN, min);
            shader->setUniform(UNIFORM_GRASS_WIDTH, (int)width);

            glDrawElementsInstanced(model->getPrimitiveType(),
                                mesh->indicesCount,
                                GL_UNSIGNED_INT,
                                (void*)(mesh->firstVertex * sizeof(unsigned int)),
                                (int)a);

            glEnable(GL_CULL_FACE);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        if (material->normalmapTexture != 0)
        {
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);
        }
    }
    glEnable(GL_CULL_FACE);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_BLEND);

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

            shader->setUniform(UNIFORM_MVP, _mainRenderData->camera->getProjectionMatrix() * _mainRenderData->camera->getViewMatrix());
            shader->setUniform(UNIFORM_MATERIAL_EMISSIVE_COLOR, glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));

            for (int i = 0; i < 8; ++i)
                shader->setUniform((UniformName)(UNIFORM_DEBUG_VERTEX_INDEX_1 + i), vertices[i]);

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

            shader->setUniform(UNIFORM_MVP, _mainRenderData->camera->getProjectionMatrix() * _mainRenderData->camera->getViewMatrix() * renderObject->getSceneObject()->getGlobalTransformMatrix());
            shader->setUniform(UNIFORM_MATERIAL_EMISSIVE_COLOR, glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));

            for (int i = 0; i < 8; ++i)
                shader->setUniform((UniformName)(UNIFORM_DEBUG_VERTEX_INDEX_1 + i), vertices[i]);

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
        RShader* shader = _shaderList[i->getShaderType()];
        shader->enable();

        shader->setUniform(UNIFORM_GUI_VERTICES_TRANSFORM_MATRIX, projectionMatrix * i->getVerticesTransformMatrix());
        shader->setUniform(UNIFORM_GUI_TEXCOORDS_TRANSFORM_MATRIX, i->getTexCoordTransformMatrix());
        shader->setUniform(UNIFORM_GUI_COLOR, i->getColor());

        i->getVBO()->bind();


        shader->bindTexture(UNIFORM_DIFFUSE_TEXTURE, i->getTexture());

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
