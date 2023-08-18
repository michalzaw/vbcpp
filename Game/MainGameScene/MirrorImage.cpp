#include "MirrorImage.h"

#include "../../GUI/GUIManager.h"

#include "../../Graphics/Renderer.h"


MirrorImage::MirrorImage(GUIManager* gui, MirrorComponent* mirrorComponent)
	: _gui(gui),
	_mirrorComponent(mirrorComponent)
{
	RTexture* mirrorComponentTexture = _mirrorComponent->getFramebuffer()->getTexture();

	_mirrorImageFramebuffer = OGLDriver::getInstance().createFramebuffer();
	_mirrorImageFramebuffer->addTexture(TF_RGBA, mirrorComponentTexture->getSize().x, mirrorComponentTexture->getSize().y);
	_mirrorImageFramebuffer->getTexture()->setClampMode(TCM_REPEAT);

	OGLDriver::getInstance().registerFramebufferForInitialization(_mirrorImageFramebuffer);

	_mirrorImage = _gui->addImage(_mirrorImageFramebuffer->getTexture());
	_mirrorImage->setInvertX(true);
	_mirrorImage->setInvertY(false);
}


void MirrorImage::setMirrorComponent(MirrorComponent* mirrorComponent)
{
	_mirrorComponent = mirrorComponent;
}


Image* MirrorImage::getImage()
{
	return _mirrorImage;
}


void MirrorImage::setIsActive(bool isActive)
{
	_mirrorImage->setIsActive(isActive);

	_mirrorComponent->setRefreshAlways(isActive);
}


bool MirrorImage::isActive()
{
	return _mirrorImage->isActive();
}


void MirrorImage::update()
{
	PostProcessingEffect* postProcessingToneMappingEffect = Renderer::getInstance().findEffect(PPT_TONE_MAPPING);
	if (postProcessingToneMappingEffect != nullptr)
	{
		postProcessingToneMappingEffect->run(_mirrorComponent->getFramebuffer()->getTexture(), _mirrorImageFramebuffer);
	}
}
