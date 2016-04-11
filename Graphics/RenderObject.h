#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED


#include <vector>

#include "Model.h"
#include "Transform.h"


class RenderObject
{
    private:
        Model* _model;
        Transform _transform;

        std::string _name;
        bool _isActive;

    public:
        RenderObject(Model* model = NULL, std::string name = "RenderObject");
        ~RenderObject();

        void SetModel(Model* model);
        void SetTransform(Transform& transform);
        void SetIsActive(bool isActive);
        Model* GetModel();
        Transform* GetTransform();
        std::string GetName();
        bool IsActive();

};


#endif // RENDEROBJECT_H_INCLUDED
