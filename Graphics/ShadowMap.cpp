#include "ShadowMap.h"

#include "GraphicsManager.h"

#include "../Scene/SceneManager.h"


ShadowMap::ShadowMap(SceneObject* lightObject)
{
    _lightObject = lightObject;
}


ShadowMap::~ShadowMap()
{
    destroy();
}


void ShadowMap::updateTextureSizeFromConfig()
{
    int size = GameConfig::getInstance().shadowmapSize;

    _shadowmapSize = size;
}


void ShadowMap::create()
{
    updateTextureSizeFromConfig();

    _shadowMapFramebuffer = OGLDriver::getInstance().createFramebuffer();
    _shadowMapFramebuffer->addTextureArray(TF_DEPTH_COMPONENT, _shadowmapSize, _shadowmapSize, CASCADE_COUNT);
    _shadowMapFramebuffer->getTexture(0)->setParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    _shadowMapFramebuffer->getTexture(0)->setParameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    OGLDriver::getInstance().registerFramebufferForInitialization(_shadowMapFramebuffer);

    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        // todo: graphicsManager refactor
        _cameraForShadowMap[i] = _lightObject->getSceneManager()->getGraphicsManager()->addCameraStatic(CPT_ORTHOGRAPHIC);
        _cameraForShadowMap[i]->setOrthoProjectionParams(-256.0f, 256.0f, -256.0f, 256.0f, -256.0f, 256.0f);

        _lightObject->addComponent(_cameraForShadowMap[i]);
    }
}


void ShadowMap::destroy()
{
    OGLDriver::getInstance().deleteFramebuffer(_shadowMapFramebuffer);

    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        if (_cameraForShadowMap[i] != NULL)
        {
            _lightObject->removeComponent(_cameraForShadowMap[i]);
        }
    }
}


Framebuffer* ShadowMap::getShadowMapFramebuffer()
{
    return _shadowMapFramebuffer;
}


CameraStatic* ShadowMap::getCameraForShadowMap(int index)
{
    return _cameraForShadowMap[index];
}
