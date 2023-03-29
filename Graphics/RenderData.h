#ifndef RENDERDATA_H_INCLUDED
#define RENDERDATA_H_INCLUDED


#include <list>

#include "Model.h"
#include "CameraStatic.hpp"
#include "Light.h"
#include "Transform.h"
#include "RenderObject.h"
#include "../Utils/RStaticModel.h"
#include "../Scene/SceneObject.h"


enum RenderElementType
{
    RET_SINGLE,
    RET_GRASS
};


enum RenderPass
{
    RP_SHADOWS,
    RP_MIRROR,
    RP_NORMAL
};


struct RenderListElement
{
    RenderElementType   type;

    RStaticModel*       model;
    ModelNodeMesh*      mesh;
    Material*           material;

    float               distanceFromCamera;

    SceneObject*        object;
    RenderObject*       renderObject;

    glm::mat4           transformMatrix;
    glm::mat4           normalMatrix;

    RenderListElement();

};


struct RenderData
{
    std::list<RenderListElement> renderList;
    CameraStatic* camera;
    Framebuffer* framebuffer;
    int framebufferLayer;
    glm::mat4 MVMatrix;
    RenderPass renderPass;

    RenderData()
        : camera(nullptr), framebuffer(nullptr), framebufferLayer(-1), MVMatrix(1.0f), renderPass(RP_NORMAL)
    {}

};


#endif // RENDERDATA_H_INCLUDED
