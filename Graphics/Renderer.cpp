#include "Renderer.h"


Renderer::Renderer(unsigned int screenWidth, unsigned int screenHeight/* OGLDriver* driver */)
//    : _OGLDriver(driver)
    : _screenWidth(screenWidth), _screenHeight(screenHeight)
{
    std::vector<std::string> defines;
    // Load shaders
    // SOLID_MATERIAL
    defines.push_back("SOLID");
	RShader* shdr1 = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);
    _shaderList.push_back(shdr1);

    // NOTEXTURE_MATERIAL
    RShader* shdr2 = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader_notexture.frag", defines);
    _shaderList.push_back(shdr2);

    // NORMALMAPPING_MATERIAL
    defines.clear();
    defines.push_back("NORMALMAPPING");
    RShader* shader = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);
    _shaderList.push_back(shader);

    // ALPHA_TEST_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    RShader* alphaTestShader = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);
    _shaderList.push_back(alphaTestShader);

    // TRANSPARENCY_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("TRANSPARENCY");
    RShader* transparencyShader = ResourceManager::getInstance().loadShader("Shaders/shader.vert", "Shaders/shader.frag", defines);
    _shaderList.push_back(transparencyShader);

    // TREE_MATERIAL
    defines.clear();
    defines.push_back("SOLID");
    defines.push_back("ALPHA_TEST");
    RShader* treeShader = ResourceManager::getInstance().loadShader("Shaders/tree.vert", "Shaders/shader.frag", defines);
    _shaderList.push_back(treeShader);

    // GUI_IMAGE_SHADER
    RShader* guishader = ResourceManager::getInstance().loadShader("Shaders/GUIshader.vert", "Shaders/GUIshader.frag");
    _shaderList.push_back(guishader);

    // GUI_LABEL_SHADER
    RShader* guishader2 = ResourceManager::getInstance().loadShader("Shaders/GUIshader.vert", "Shaders/LabelShader.frag");
    _shaderList.push_back(guishader2);

    // SKY_MATERIAL
    RShader* skyshader = ResourceManager::getInstance().loadShader("Shaders/sky.vert", "Shaders/sky.frag");
    _shaderList.push_back(skyshader);


    // Create UBO for lights
    _lightUBO = OGLDriver::getInstance().createUBO(4096);
    _lightUBO->bindBufferBase(0);

    _shaderList[SOLID_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);
    _shaderList[NORMALMAPPING_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);
}

Renderer::~Renderer()
{
    OGLDriver::getInstance().deleteUBO(_lightUBO);
}

/*void Renderer::Render(RenderData* renderData)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CCamera* camera = renderData->_camera;

    for (int i = 0; i < renderData->_renderList.size(); ++i)
    {
        RenderObject* renderObject = renderData->_renderList[i].GetRenderObject();
        unsigned int meshIndex = renderData->_renderList[i].GetMeshIndex();

        Shader* shader = renderObject->GetModel()->GetMesh(meshIndex).material.shader;
        shader->Enable();

        glm::mat4 MVP = camera->GetMatrices().GetViewProjectionMatrix() * renderObject->GetTransform().GetTransformMatrix();
        shader->SetUniform("MVP", MVP);

        renderObject->GetModel()->GetVBO()->Bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

        shader->BindTexture2D("Texture", renderObject->GetModel()->GetMesh(meshIndex).material.diffuseTexture);

        shader->SetUniform("Transparency", renderObject->GetModel()->GetMesh(meshIndex).material.transparency);

        glDrawArrays(renderObject->GetModel()->GetPrimitiveType(),
            renderObject->GetModel()->GetMesh(meshIndex).firstVertex,
            renderObject->GetModel()->GetMesh(meshIndex).quantumOfVertice);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

    delete renderData;
}*/


// z oœwietleniem
/*void Renderer::Render(RenderData* renderData)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CCamera* camera = renderData->_camera;

    for (int i = 0; i < renderData->_renderList.size(); ++i)
    {
        RenderObject* renderObject = renderData->_renderList[i].GetRenderObject();
        unsigned int meshIndex = renderData->_renderList[i].GetMeshIndex();

        Shader* shader = renderObject->GetModel()->GetMesh(meshIndex).material.shader;
        shader->Enable();

        //if (renderObject->GetModel()->GetMesh(meshIndex).material.name != "Material #305")
            //std::cout << "\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n";

        glm::mat4 MVP = camera->GetMatrices().GetViewProjectionMatrix() * renderObject->GetTransform().GetTransformMatrix();
        shader->SetUniform("MVP", MVP);
        shader->SetUniform("ModelMatrix", renderObject->GetTransform().GetTransformMatrix());
        shader->SetUniform("NormalMatrix", renderObject->GetTransform().GetNormalMatrix());

        renderObject->GetModel()->GetVBO()->Bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));

        shader->SetUniform("Light.Color", renderData->_light->GetColor());
        shader->SetUniform("Light.AmbientIntensity", renderData->_light->GetAmbientIntensity());
        shader->SetUniform("Light.DiffuseIntensity", renderData->_light->GetDiffiseIntenisty());
        shader->SetUniform("Light.Direction", renderData->_light->GetDirection());

        shader->BindTexture2D("Texture", renderObject->GetModel()->GetMesh(meshIndex).material.diffuseTexture);

        shader->SetUniform("Transparency", renderObject->GetModel()->GetMesh(meshIndex).material.transparency);

        shader->SetUniform("SpecularPower", renderObject->GetModel()->GetMesh(meshIndex).material.shininess);
        shader->SetUniform("CameraPosition", renderData->_camera->GetPosition());

        glDrawArrays(renderObject->GetModel()->GetPrimitiveType(),
            renderObject->GetModel()->GetMesh(meshIndex).firstVertex,
            renderObject->GetModel()->GetMesh(meshIndex).quantumOfVertice);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    delete renderData;
}*/


// nowe ³adowanie modeli, indeksy
/*void Renderer::Render(RenderData* renderData)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RModel* model = i->GetModel();
        Mesh* mesh = i->GetMesh();

        RShader* shader = mesh->material._shader;
        shader->Enable();

        //glm::mat4 MVP = camera->GetMatrices().GetViewProjectionMatrix() * i->GetTransform()->GetTransformMatrix();
        glm::mat4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix() * i->GetTransform()->GetTransformMatrix();
        shader->SetUniform("MVP", MVP);
        shader->SetUniform("ModelMatrix", i->GetTransform()->GetTransformMatrix());
        shader->SetUniform("NormalMatrix", i->GetTransform()->GetNormalMatrix());

        model->GetVBO()->Bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));

        if (mesh->material.normalmapTexture != 0)
        {
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 11));

            shader->BindTexture2D("NormalmapTexture", mesh->material.normalmapTexture);
        }

        int d = 0;
        int p = 0;
        int s = 0;
        for (std::list<Light*>::iterator i = renderData->lights.begin(); i != renderData->lights.end(); ++i)
        {
            switch ((*i)->GetLightType())
            {
                case LT_DIRECTIONAL:
                {
                    std::string lname = "DirLights[";
                    lname = lname + toString(d++) + "]";

                    shader->SetUniform((lname + ".Base.Color").c_str(), (*i)->GetColor());
                    shader->SetUniform((lname + ".Base.AmbientIntensity").c_str(), (*i)->GetAmbientIntensity());
                    shader->SetUniform((lname + ".Base.DiffuseIntensity").c_str(), (*i)->GetDiffiseIntenisty());
                    shader->SetUniform((lname + ".Direction").c_str(), (*i)->GetDirection());

                    break;
                }
                case LT_POINT:
                {
                    std::string lname = "PointLights[";
                    lname = lname + toString(p++) + "]";

                    shader->SetUniform((lname + ".Base.Color").c_str(), (*i)->GetColor());
                    shader->SetUniform((lname + ".Base.AmbientIntensity").c_str(), (*i)->GetAmbientIntensity());
                    shader->SetUniform((lname + ".Base.DiffuseIntensity").c_str(), (*i)->GetDiffiseIntenisty());
                    shader->SetUniform((lname + ".Position").c_str(), (*i)->GetPosition());
                    shader->SetUniform((lname + ".Attenuation.constant").c_str(), (*i)->GetAttenuation().constant);
                    shader->SetUniform((lname + ".Attenuation.linear").c_str(), (*i)->GetAttenuation().linear);
                    shader->SetUniform((lname + ".Attenuation.exp").c_str(), (*i)->GetAttenuation().exp);

                    break;
                }
                case LT_SPOT:
                {
                    std::string lname = "SpotLights[";
                    lname = lname + toString(s++) + "]";

                    shader->SetUniform((lname + ".Base.Base.Color").c_str(), (*i)->GetColor());
                    shader->SetUniform((lname + ".Base.Base.AmbientIntensity").c_str(), (*i)->GetAmbientIntensity());
                    shader->SetUniform((lname + ".Base.Base.DiffuseIntensity").c_str(), (*i)->GetDiffiseIntenisty());
                    shader->SetUniform((lname + ".Base.Position").c_str(), (*i)->GetPosition());
                    shader->SetUniform((lname + ".Base.Attenuation.constant").c_str(), (*i)->GetAttenuation().constant);
                    shader->SetUniform((lname + ".Base.Attenuation.linear").c_str(), (*i)->GetAttenuation().linear);
                    shader->SetUniform((lname + ".Base.Attenuation.exp").c_str(), (*i)->GetAttenuation().exp);
                    shader->SetUniform((lname + ".Direction").c_str(), (*i)->GetDirection());
                    shader->SetUniform((lname + ".CutoffCos").c_str(), (*i)->GetCutoffCos());
                }
            }

        }
        shader->SetUniform("DirCount", d);
        shader->SetUniform("PointCount", p);
        shader->SetUniform("SpotCount", s);

        //shader->BindTexture2D("Texture", mesh->material.diffuseTexture);
        shader->BindTexture2D("Texture", mesh->material.diffuseTexture);
        shader->SetUniform("matAmbient", mesh->material.ambientColor);
        shader->SetUniform("matDiffuse", mesh->material.diffuseColor);
        shader->SetUniform("matSpecular", mesh->material.specularColor);

        shader->SetUniform("Transparency", mesh->material.transparency);

        shader->SetUniform("SpecularPower", mesh->material.shininess);
        shader->SetUniform("CameraPosition", camera->getPosition());
        shader->SetUniform("a", 1.0f);

        //glDrawArrays(renderObject->GetModel()->GetPrimitiveType(),
        //    renderObject->GetModel()->GetMesh(meshIndex).firstVertex,
        //    renderObject->GetModel()->GetMesh(meshIndex).quantumOfVertice);

        model->GetIBO()->Bind();
        glDrawElements(model->GetPrimitiveType(),
                       mesh->quantumOfVertice,
                       GL_UNSIGNED_INT,
                       (void*)(mesh->firstVertex * sizeof(unsigned int)));

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        if (mesh->material.normalmapTexture != 0)
        {
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);
        }
    }


    delete renderData;
}*/


// UBO
void Renderer::render(RenderData* renderData)
{
    // Aktualizacja UBO swiatel
    int d = 0;
    int p = 0;
    int s = 0;
    for (std::list<Light*>::iterator i = renderData->lights.begin(); i != renderData->lights.end(); ++i)
    {
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


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        RModel* model = i->getModel();
        Mesh* mesh = i->getMesh();

        RShader* shader = _shaderList[mesh->material.shader];
        shader->enable();
        if (mesh->material.shader == SKY_MATERIAL)
        {
            glDisable(GL_CULL_FACE);
            i->getTransformMatrices().transformMatrix = glm::translate(camera->getPosition());
        }
        if (mesh->material.shader == TREE_MATERIAL)
        {
            glDisable(GL_BLEND);
            //shader->setUniform("n",);
            shader->setUniform("CameraPositionWorldspace", camera->getPosition());

            glm::vec3 d = GraphicsManager::getInstance().getWindVector();

            Component* treeComponent = i->getObject()->getComponent(CT_TREE_COMPONENT);
            if (treeComponent != NULL)
            {
                d = static_cast<TreeComponent*>(treeComponent)->getWindVector();
            }

            shader->setUniform("d", d);
        }
        if (mesh->material.shader == ALPHA_TEST_MATERIAL)
        {
            glDisable(GL_BLEND);
        }

        //glm::mat4 MVP = camera->GetMatrices().GetViewProjectionMatrix() * i->GetTransform()->GetTransformMatrix();
        glm::mat4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix() * i->getTransformMatrices().transformMatrix;
        shader->setUniform("MVP", MVP);
        shader->setUniform("ModelMatrix", i->getTransformMatrices().transformMatrix);
        shader->setUniform("NormalMatrix", i->getTransformMatrices().normalMatrix);

        model->getVBO()->bind();

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

            shader->bindTexture("NormalmapTexture", mesh->material.normalmapTexture);
        }

        //shader->BindTexture2D("Texture", mesh->material.diffuseTexture);
        if (mesh->material.diffuseTexture != NULL)
            shader->bindTexture("Texture", mesh->material.diffuseTexture);
        if (mesh->material.alphaTexture != NULL)
            shader->bindTexture("AlphaTexture", mesh->material.alphaTexture);
        shader->setUniform("matAmbient", mesh->material.ambientColor);
        shader->setUniform("matDiffuse", mesh->material.diffuseColor);
        shader->setUniform("matSpecular", mesh->material.specularColor);

        shader->setUniform("Transparency", mesh->material.transparency);

        shader->setUniform("SpecularPower", mesh->material.shininess);
        shader->setUniform("CameraPosition", camera->getPosition());
        shader->setUniform("a", 1.0f);

        //glDrawArrays(renderObject->GetModel()->GetPrimitiveType(),
        //    renderObject->GetModel()->GetMesh(meshIndex).firstVertex,
        //    renderObject->GetModel()->GetMesh(meshIndex).quantumOfVertice);

        model->getIBO()->bind();
        glDrawElements(model->getPrimitiveType(),
                       mesh->quantumOfVertice,
                       GL_UNSIGNED_INT,
                       (void*)(mesh->firstVertex * sizeof(unsigned int)));

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        if (mesh->material.normalmapTexture != 0)
        {
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);
        }
        if (mesh->material.shader == SKY_MATERIAL)
            glEnable(GL_CULL_FACE);

        if (mesh->material.shader == TREE_MATERIAL || mesh->material.shader == ALPHA_TEST_MATERIAL)
        {
            glEnable(GL_BLEND);
        }
    }


    delete renderData;
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

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight, 1.0f, -1.5f);

    for (GUIRenderList::iterator i = renderList->begin(); i != renderList->end(); ++i)
    {
        RShader* shader = _shaderList[i->getShaderType()];
        shader->enable();

        shader->setUniform("VerticesTransformMatrix", projectionMatrix * i->getVerticesTransformMatrix());
        shader->setUniform("TexCoordTransformMatrix", i->getTexCoordTransformMatrix());
        shader->setUniform("color", i->getColor());

        i->getVBO()->bind();


        shader->bindTexture("Texture", i->getTexture());

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
    glEnable(GL_DEPTH_TEST);
}
