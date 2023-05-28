#include "MirrorImage.h"

#include "../../GUI/GUIManager.h"

#include "../../Graphics/Renderer.h"


MirrorImage::MirrorImage(GUIManager* gui, RTexture* texture)
	: _gui(gui),
	_mirrorOriginalTexture(texture)
{
	_mirrorImageFramebuffer = OGLDriver::getInstance().createFramebuffer();
	_mirrorImageFramebuffer->addTexture(TF_RGBA, _mirrorOriginalTexture->getSize().x, _mirrorOriginalTexture->getSize().y);
	_mirrorImageFramebuffer->getTexture()->setClampMode(TCM_REPEAT);

	OGLDriver::getInstance().registerFramebufferForInitialization(_mirrorImageFramebuffer);

	_mirrorImage = _gui->addImage(_mirrorImageFramebuffer->getTexture());
	_mirrorImage->setInvertX(true);
	_mirrorImage->setInvertY(false);
}


void MirrorImage::setMirrorOriginalTexture(RTexture* texture)
{
	_mirrorOriginalTexture = texture;
}


void MirrorImage::setPosition(const glm::vec2& position)
{
	_mirrorImage->setPosition(position);
}


void MirrorImage::setIsActive(bool isActive)
{
	_mirrorImage->setIsActive(isActive);
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
		postProcessingToneMappingEffect->run(_mirrorOriginalTexture, _mirrorImageFramebuffer);
	}
}
