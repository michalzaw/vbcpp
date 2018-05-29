#ifndef RENDERDATA_H_INCLUDED
#define RENDERDATA_H_INCLUDED


#include <list>

#include "Model.h"
#include "CameraStatic.hpp"
#include "Light.h"
#include "Transform.h"
#include "RenderObject.h"
#include "../Utils/RModel.h"
#include "../Scene/SceneObject.h"


enum RenderElementType
{
    RET_SINGLE,
    RET_GRASS
};


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
        RenderElementType _type;

        RModel*     _model;
        Mesh*       _mesh;
        TransformMatrices _matrices;

        float       _distanceFromCamera;

        SceneObject* _object; /* !!!!!!!!!! */
        RenderObject* _renderObject;

    public:
        RenderListElement(RenderElementType type, RModel* model, Mesh* mesh, TransformMatrices matrices, float distance, SceneObject* object, RenderObject* renderObject);
        ~RenderListElement();

        inline RenderElementType getType()
        { return _type; }

        inline RModel* getModel()
        { return _model; }

        inline Mesh* getMesh()
        { return _mesh; }

        inline TransformMatrices& getTransformMatrices()
        { return _matrices; }

        inline float getDistanceFromCamera()
        { return _distanceFromCamera; }

        inline SceneObject* getObject()
        { return _object; }

        inline RenderObject* getRenderObject()
        { return _renderObject; }

};


struct RenderData
{
    std::list<RenderListElement> renderList;
    CameraStatic* camera;
    std::list<Light*> lights;

};


#endif // RENDERDATA_H_INCLUDED
