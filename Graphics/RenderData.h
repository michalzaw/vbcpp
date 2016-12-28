#ifndef RENDERDATA_H_INCLUDED
#define RENDERDATA_H_INCLUDED


#include <list>

#include "Model.h"
#include "CameraStatic.hpp"
#include "Light.h"
#include "Transform.h"
#include "../Utils/RModel.h"


struct TransformMatrices
{
    glm::mat4& transformMatrix;
    glm::mat4& normalMatrix;

    TransformMatrices(glm::mat4& transform, glm::mat4& normal)
        : transformMatrix(transform), normalMatrix(normal)
    {

    }

};


class RenderListElement
{
    private:
        RModel*     _model;
        Mesh*       _mesh;
        TransformMatrices _matrices;

        float       _distanceFromCamera;

    public:
        RenderListElement(RModel* model, Mesh* mesh, TransformMatrices matrices, float distance);
        ~RenderListElement();

        inline RModel* getModel()
        { return _model; }

        inline Mesh* getMesh()
        { return _mesh; }

        inline TransformMatrices& getTransformMatrices()
        { return _matrices; }

        inline float getDistanceFromCamera()
        { return _distanceFromCamera; }

};


struct RenderData
{
    std::list<RenderListElement> renderList;
    CameraStatic* camera;
    std::list<Light*> lights;

};


#endif // RENDERDATA_H_INCLUDED
