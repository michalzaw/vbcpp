#include "Renderer.h"


Renderer::Renderer(OGLDriver* driver)
    : _OGLDriver(driver)
{
    //_shader = LoadShader("shader.vert", "shader.frag");
    //glUseProgram(_shader);
}

Renderer::~Renderer()
{

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
void Renderer::Render(RenderData* renderData)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CameraStatic* camera = renderData->camera;

    for (std::list<RenderListElement>::iterator i = renderData->renderList.begin(); i != renderData->renderList.end(); ++i)
    {
        Model* model = i->GetModel();
        Mesh* mesh = i->GetMesh();

        Shader* shader = mesh->material._shader;
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

        shader->SetUniform("Light.Color", renderData->light->GetColor());
        shader->SetUniform("Light.AmbientIntensity", renderData->light->GetAmbientIntensity());
        shader->SetUniform("Light.DiffuseIntensity", renderData->light->GetDiffiseIntenisty());
        shader->SetUniform("Light.Direction", renderData->light->GetDirection());

        //shader->BindTexture2D("Texture", mesh->material.diffuseTexture);
        shader->BindTexture2D("Texture", mesh->material.getDiffTex()->getID());
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
