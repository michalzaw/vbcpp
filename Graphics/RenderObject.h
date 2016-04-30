#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>
#include <cstdio>

#include "Model.h"
#include "Transform.h"

#include "../Physics/PhysicalBody.hpp"


class RenderObject
{
    private:
        Model* _model;
        Transform _transform;
        PhysicalBody* _physicalBody;

        std::string _name;
        bool _isActive;

    public:
        RenderObject(Model* model = NULL, std::string name = "RenderObject");
        ~RenderObject();

        void SetPhysicalBody(PhysicalBody* body);
        void Update();
        void SetModel(Model* model);
        void SetTransform(Transform& transform);
        void SetIsActive(bool isActive);
        Model* GetModel();
        Transform* GetTransform();
        std::string GetName();
        bool IsActive();

};


#endif // RENDEROBJECT_H_INCLUDED
