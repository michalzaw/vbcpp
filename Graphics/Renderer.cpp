#include "Renderer.h"


static std::unique_ptr<Renderer> rendererInstance;


Renderer::Renderer()
    : _isInitialized(false),
    _screenWidth(0), _screenHeight(0),
    _isShadowMappingEnable(false), _shadowMap(NULL)
{

}

Renderer::~Renderer()
{
    OGLDriver::getInstance().deleteUBO(_lightUBO);
}


Renderer& Renderer::getInstance()
{
    if ( !rendererInstance )
        rendererInstance = std::unique_ptr<Renderer>(new Renderer);

    return *rendererInstance;
}


bool compareByShader(const RenderListElement& a, const RenderListElement& b)
{
    return a.mesh->material.shader < b.mesh->material.shader;
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


void Renderer::prepareRenderData()
{
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
}


void Renderer::createRenderDatasForShadowMap(ShadowMap* shadowMap)
{
    for (int i = 0; i < shadowMap->CASCADE_COUNT; ++i)
    {
        RenderData* renderData = new RenderData;
        renderData->camera = shadowMap->getCameraForShadowMap(i);
        renderData->framebuffer = shadowMap->getShadowMap(i);
        _renderDataList.insert(_renderDataList.begin(), renderData);
    }
}


void Renderer::deleteRenderDatasForShadowMap(ShadowMap* shadowMap)
{
    for (int i = 0; i < shadowMap->CASCADE_COUNT; ++i)
    {
        for (std::vector<RenderData*>::iterator j = _renderDataList.begin(); j != _renderDataList.end(); ++j)
        {
            if (shadowMap->getShadowMap(i) == (*j)->framebuffer)
            {
                RenderData* renderData = (*j);
                _renderDataList.erase(j);
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

    Framebuffer* defaultFramebuffer = OGLDriver::getInstance().getDefaultFramebuffer();
    defaultFramebuffer->setViewport(UintRect(0, 0, _screenWidth, _screenHeight));


    RenderData* mainRenderData = new RenderData;
    mainRenderData->camera = GraphicsManager::getInstance().getCurrentCamera();
    mainRenderData->framebuffer = defaultFramebuffer;
    _renderDataList.push_back(mainRenderData);


    _shaderList.resize(NUMBER_OF_SHADERS);
    std::vector<std::string> defines;

    // Load shaders
    // SOLID_MATERIAL
    defines.push_back("SOLID");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
	_shaderList[SOLID_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);



    defines.push_back("CAR_PAINT");
	_shaderList[CAR_PAINT] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);


    // NOTEXTURE_MATERIAL
    _shaderList[NOTEXTURE_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader_notexture.frag", defines);

    // NORMALMAPPING_MATERIAL
    defines.clear();
    defines.push_back("NORMALMAPPING");
    if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    _shaderList[NORMALMAPPING_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);

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
    //defines.push_back("SOLID");
    //defines.push_back("TRANSPARENCY");
    //if (_isShadowMappingEnable) defines.push_back("SHADOWMAPPING");
    _shaderList[GLASS_MATERIAL] = ResourceManager::getInstance().loadShader("Shaders/glass.vert", "Shaders/glass.frag");

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


    // Create UBO for lights
    _lightUBO = OGLDriver::getInstance().createUBO(4096);
    _lightUBO->bindBufferBase(0);

    _shaderList[SOLID_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);
    _shaderList[NORMALMAPPING_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);


    _isInitialized = true;
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
    _renderDataList[_renderDataList.size() - 1]->camera = camera;
}


void Renderer::renderAll()
{
    prepareLightsData();
    prepareRenderData();

    for (int i = 0; i < _renderDataList.size() - 1; ++i)
    {
        renderDepth(_renderDataList[i]);
    }

    renderScene(_renderDataList[_renderDataList.size() - 1]);
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

        ShaderType shaderType;
        bool isAlphaTest = mesh->material.shader == ALPHA_TEST_MATERIAL || mesh->material.shader == TREE_MATERIAL;
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

        glm::mat4 MVP = renderData->MVMatrix * i->object->getGlobalTransformMatrix();
        shader->setUniform(UNIFORM_MVP, MVP);

        mesh->vbo->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        if (isAlphaTest)
        {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

            shader->bindTexture(UNIFORM_ALPHA_TEXTURE, mesh->material.diffuseTexture);
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

    ShaderType currentShader;
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RStaticModel* model = i->model;
        StaticModelMesh* mesh = i->mesh;

        RShader* shader = _shaderList[mesh->material.shader];
        if (mesh->material.shader != currentShader)
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

            if (mesh->material.shader == SKY_MATERIAL)
            {
                glDisable(GL_CULL_FACE);
            }
            else if (mesh->material.shader == TREE_MATERIAL || mesh->material.shader == ALPHA_TEST_MATERIAL || mesh->material.shader == GRASS_MATERIAL)
            {
                glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            }
            else if (mesh->material.shader == GLASS_MATERIAL)
            {
                glEnable(GL_BLEND);
                shader->bindTexture(UNIFORM_DIFFUSE_TEXTURE, envMap);
            }

            currentShader = mesh->material.shader;
        }
        else
        {
            shader->resetTextureLocation();
        }


                shader->bindTexture(ENV, envMap);



        if (mesh->material.shader == TREE_MATERIAL)
        {
            glm::vec3 d = GraphicsManager::getInstance().getWindVector();

            Component* treeComponent = i->object->getComponent(CT_TREE_COMPONENT);
            if (treeComponent != NULL)
            {
                d = static_cast<TreeComponent*>(treeComponent)->getWindVector();
            }

            shader->setUniform(UNIFORM_WIND_DIRECTION, d);
        }


        glm::mat4 MVP = renderData->MVMatrix * i->object->getGlobalTransformMatrix();
        shader->setUniform(UNIFORM_MVP, MVP);
        shader->setUniform(UNIFORM_MODEL_MATRIX, i->object->getGlobalTransformMatrix());
        shader->setUniform(UNIFORM_NORMAL_MATRIX, i->object->getGlobalNormalMatrix());

        shader->setUniform(UNIFORM_MATERIAL_AMBIENT_COLOR, mesh->material.ambientColor);
        shader->setUniform(UNIFORM_MATERIAL_DIFFUSE_COLOR, mesh->material.diffuseColor);
        shader->setUniform(UNIFORM_MATERIAL_SPECULAR_COLOR, mesh->material.specularColor);

        shader->setUniform(UNIFORM_MATERIAL_TRANSPARENCY, mesh->material.transparency);

        shader->setUniform(UNIFORM_MATERIAL_SPECULAR_POWER, mesh->material.shininess);
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

        if (mesh->material.normalmapTexture != NULL)
        {
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 11));

            shader->bindTexture(UNIFORM_NOTMALMAP_TEXTURE, mesh->material.normalmapTexture);
        }

        if (mesh->material.diffuseTexture != NULL)
            shader->bindTexture(UNIFORM_DIFFUSE_TEXTURE, mesh->material.diffuseTexture);



        if (i->type != RET_GRASS)
        {
            glDrawElements(model->getPrimitiveType(),
                           mesh->indicesCount,
                           GL_UNSIGNED_INT,
                           (void*)(mesh->firstVertex * sizeof(unsigned int)));
        }
        else
        {
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
        if (mesh->material.normalmapTexture != 0)
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
    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RModel* model = i->getModel();
        Mesh* mesh = i->getMesh();
        glPointSize(50);
        glLineWidth(3);
        RShader* shader = _shaderList[DEBUG_SHADER];
        shader->enable();

        glm::mat4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix() * i->getTransformMatrices().transformMatrix;
        shader->setUniform("MVP", MVP);

        model->getAabbVbo()->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        model->getAabbIbo()->bind();
        glDrawElements(GL_LINES,
                       24,
                       GL_UNSIGNED_INT,
                       (void*)0);

        glDisableVertexAttribArray(0);
    }
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
