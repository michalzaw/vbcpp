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
        RModel*     _model;
        Mesh*       _mesh;
        Transform*  _transform;

        float       _distanceFromCamera;

    public:
        RenderListElement(RModel* model, Mesh* mesh, Transform* transform, float distance);
        ~RenderListElement();

        inline RModel* getModel()
        { return _model; }

        inline Mesh* getMesh()
        { return _mesh; }

        inline Transform* getTransform()
        { return _transform; }

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
