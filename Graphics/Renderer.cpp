#include "Renderer.h"


Renderer::Renderer(/* OGLDriver* driver */)
//    : _OGLDriver(driver)
{
    // Load shaders
    // SOLID_MATERIAL
	RShader* shdr1 = ResourceManager::getInstance().loadShader("shader.vert", "shader.frag");
    _shaderList.push_back(shdr1);

    // NOTEXTURE_MATERIAL
    RShader* shdr2 = ResourceManager::getInstance().loadShader("DirLight.vert", "DirLight_notexture.frag");
    _shaderList.push_back(shdr2);

    // NORMALMAPPING_MATERIAL
    RShader* shader = ResourceManager::getInstance().loadShader("shader_n.vert", "shader_n.frag");
    _shaderList.push_back(shader);


    // Create UBO for lights
    _lightUBO = OGLDriver::getInstance().CreateUBO(4096);
    _lightUBO->bindBufferBase(0);

    _shaderList[SOLID_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);
    _shaderList[NORMALMAPPING_MATERIAL]->setUniformBlockBinding("LightsBlock", 0);
}

Renderer::~Renderer()
{
    OGLDriver::getInstance().DeleteUBO(_lightUBO);
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
void Renderer::Render(RenderData* renderData)
{
    // Aktualizacja UBO swiatel
    int d = 0;
    int p = 0;
    int s = 0;
    for (std::list<Light*>::iterator i = renderData->lights.begin(); i != renderData->lights.end(); ++i)
    {
        switch ((*i)->GetLightType())
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

                _lightUBO->setUniform(offsets[0],                        (*i)->GetColor().r);
                _lightUBO->setUniform(offsets[0] + sizeof(float),        (*i)->GetColor().g);
                _lightUBO->setUniform(offsets[0] + 2 * sizeof(float),    (*i)->GetColor().b);

                _lightUBO->setUniform(offsets[1], (*i)->GetAmbientIntensity());

                _lightUBO->setUniform(offsets[2], (*i)->GetDiffiseIntenisty());

                _lightUBO->setUniform(offsets[3],                        (*i)->GetDirection().x);
                _lightUBO->setUniform(offsets[3] + sizeof(float),        (*i)->GetDirection().y);
                _lightUBO->setUniform(offsets[3] + 2 * sizeof(float),    (*i)->GetDirection().z);

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

                _lightUBO->setUniform(offsets[0],                        (*i)->GetColor().r);
                _lightUBO->setUniform(offsets[0] + sizeof(float),        (*i)->GetColor().g);
                _lightUBO->setUniform(offsets[0] + 2 * sizeof(float),    (*i)->GetColor().b);

                _lightUBO->setUniform(offsets[1], (*i)->GetAmbientIntensity());

                _lightUBO->setUniform(offsets[2], (*i)->GetDiffiseIntenisty());

                _lightUBO->setUniform(offsets[3],                        (*i)->GetPosition().x);
                _lightUBO->setUniform(offsets[3] + sizeof(float),        (*i)->GetPosition().y);
                _lightUBO->setUniform(offsets[3] + 2 * sizeof(float),    (*i)->GetPosition().z);

                _lightUBO->setUniform(offsets[4], (*i)->GetAttenuation().constant);
                _lightUBO->setUniform(offsets[5], (*i)->GetAttenuation().linear);
                _lightUBO->setUniform(offsets[6], (*i)->GetAttenuation().exp);

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

                _lightUBO->setUniform(offsets[0],                        (*i)->GetColor().r);
                _lightUBO->setUniform(offsets[0] + sizeof(float),        (*i)->GetColor().g);
                _lightUBO->setUniform(offsets[0] + 2 * sizeof(float),    (*i)->GetColor().b);

                _lightUBO->setUniform(offsets[1], (*i)->GetAmbientIntensity());

                _lightUBO->setUniform(offsets[2], (*i)->GetDiffiseIntenisty());

                _lightUBO->setUniform(offsets[3],                        (*i)->GetPosition().x);
                _lightUBO->setUniform(offsets[3] + sizeof(float),        (*i)->GetPosition().y);
                _lightUBO->setUniform(offsets[3] + 2 * sizeof(float),    (*i)->GetPosition().z);

                _lightUBO->setUniform(offsets[4], (*i)->GetAttenuation().constant);
                _lightUBO->setUniform(offsets[5], (*i)->GetAttenuation().linear);
                _lightUBO->setUniform(offsets[6], (*i)->GetAttenuation().exp);

                _lightUBO->setUniform(offsets[7],                        (*i)->GetDirection().x);
                _lightUBO->setUniform(offsets[7] + sizeof(float),        (*i)->GetDirection().y);
                _lightUBO->setUniform(offsets[7] + 2 * sizeof(float),    (*i)->GetDirection().z);

                _lightUBO->setUniform(offsets[8], (*i)->GetCutoffCos());

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
        RModel* model = i->GetModel();
        Mesh* mesh = i->GetMesh();

        RShader* shader = _shaderList[mesh->material._shader];
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
}
