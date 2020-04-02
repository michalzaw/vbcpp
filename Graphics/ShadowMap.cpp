#include "ShadowMap.h"

#include "GraphicsManager.h"


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
	int staticSize = GameConfig::getInstance().staticShadowmapSize;

    _shadowmapSize[0] = size;
	_shadowmapSize[1] = staticSize;//size / 2;
	//_shadowmapSize[2] = size / 4;
}


void ShadowMap::create()
{
    updateTextureSizeFromConfig();

    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        _shadowMap[i] = OGLDriver::getInstance().createFramebuffer();
        _shadowMap[i]->addTexture(TF_DEPTH_COMPONENT, _shadowmapSize[i], _shadowmapSize[i]);
        _shadowMap[i]->getTexture(0)->setParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        _shadowMap[i]->getTexture(0)->setParameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        _shadowMap[i]->setViewport(UintRect(0, 0, _shadowmapSize[i], _shadowmapSize[i]));

        _cameraForShadowMap[i] = GraphicsManager::getInstance().addCameraStatic(CPT_ORTHOGRAPHIC);
        _cameraForShadowMap[i]->setOrthoProjectionParams(-256.0f, 256.0f, -256.0f, 256.0f, -256.0f, 256.0f);

        _lightObject->addComponent(_cameraForShadowMap[i]);
    }
}


void ShadowMap::destroy()
{
    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        if (_shadowMap[i] != NULL)
        {
            OGLDriver::getInstance().deleteFramebuffer(_shadowMap[i]);
        }
        if (_cameraForShadowMap[i] != NULL)
        {
            _lightObject->removeComponent(_cameraForShadowMap[i]);
        }
    }
}


Framebuffer* ShadowMap::getShadowMap(int index)
{
    return _shadowMap[index];
}


CameraStatic* ShadowMap::getCameraForShadowMap(int index)
{
    return _cameraForShadowMap[index];
}
