#ifndef SHADOWMAP_H_INCLUDED
#define SHADOWMAP_H_INCLUDED


#include "OGLDriver.h"
#include "Framebuffer.h"
#include "CameraStatic.hpp"

#include "../Game/GameConfig.h"


class ShadowMap
{
    public:
        static const int CASCADE_COUNT = 2;

    private:
        int _shadowmapSize;

        CameraStatic*   _cameraForShadowMap[CASCADE_COUNT];

        Framebuffer*    _shadowMapFramebuffer;

        SceneObject* _lightObject;

        void updateTextureSizeFromConfig();

    public:
        ShadowMap(SceneObject* lightObject);
        ~ShadowMap();

        void create();
        void destroy();

        Framebuffer* getShadowMapFramebuffer();
        CameraStatic* getCameraForShadowMap(int index);

};


#endif // SHADOWMAP_H_INCLUDED
