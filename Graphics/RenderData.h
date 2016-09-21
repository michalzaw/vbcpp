#ifndef RENDERDATA_H_INCLUDED
#define RENDERDATA_H_INCLUDED


#include <list>

#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "../Utils/RModel.h"

#include "CameraStatic.hpp"

class RenderListElement
{
    private:
        //Model* _model;
        RModel* _model;
        Mesh* _mesh;
        Transform* _transform;

        float _distanceFromCamera;

    public:
        RenderListElement(RModel* model, Mesh* mesh, Transform* transform, float distance);
        ~RenderListElement();

        inline RModel* GetModel()
        { return _model; }

        inline Mesh* GetMesh()
        { return _mesh; }

        inline Transform* GetTransform()
        { return _transform; }

        inline float GetDistanceFromCamera()
        { return _distanceFromCamera; }

};


struct RenderData
{
    std::list<RenderListElement> renderList;
    CameraStatic* camera;
    std::list<Light*> lights;

};


#endif // RENDERDATA_H_INCLUDED
