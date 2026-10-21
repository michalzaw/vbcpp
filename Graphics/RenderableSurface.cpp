#include "RenderableSurface.h"

#include "Renderer.h"


RenderableSurface::RenderableSurface(int width, int height)
	: Component(CT_RENDERABLE_SURFACE),
	_width(width), _height(height),
	_backgroundColor(1.0f, 1.0f, 1.0f, 1.0f),
	_renderTexture(nullptr)
{
	_guiManager = new GUIManager;

	recreateFramebuffer();
}


RenderableSurface::~RenderableSurface()
{
	delete _guiManager;
}


void RenderableSurface::recreateFramebuffer()
{
	if (_renderTexture != nullptr)
	{
		OGLDriver::getInstance().deleteFramebuffer(_renderTexture);
	}

	_renderTexture = OGLDriver::getInstance().createFramebuffer();
	_renderTexture->addTexture(TF_RGBA, _width, _height);
	_renderTexture->setTextureFiltering(0, TFM_TRILINEAR, TFM_LINEAR);
	OGLDriver::getInstance().registerFramebufferForInitialization(_renderTexture);
}


Image* RenderableSurface::addImage(RTexture* texture)
{
	return _guiManager->addImage(texture);
}


Label* RenderableSurface::addLabel(RFont* font, std::string text/* = ""*/)
{
	return _guiManager->addLabel(font, text);
}


void RenderableSurface::removeObject(GUIObject* object)
{
	_guiManager->removeObject(object);
}


void RenderableSurface::removeAllObjects()
{
	_guiManager->removeAllObjects();
}


int RenderableSurface::getWidth()
{
	return _width;
}


int RenderableSurface::getHeight()
{
	return _height;
}


void RenderableSurface::setWidth(int width, bool recreateFramebuffer/* = false*/)
{
	_width = width;

	if (recreateFramebuffer)
	{
		this->recreateFramebuffer();
	}
}


void RenderableSurface::setHeight(int height, bool recreateFramebuffer/* = false*/)
{
	_height = height;

	if (recreateFramebuffer)
	{
		this->recreateFramebuffer();
	}
}


void RenderableSurface::setBackgroundColor(const glm::vec4& backgroundColor)
{
	_backgroundColor = backgroundColor;
}


const glm::vec4& RenderableSurface::getBackgroundColor()
{
	return _backgroundColor;
}


RTexture* RenderableSurface::getRenderedTexture()
{
	return _renderTexture->getTexture();
}


void RenderableSurface::update(float deltaTime)
{
	_renderTexture->bind();

	glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer::getInstance().renderGUI(_guiManager->getGUIRenderList(), _renderTexture, true);

	OGLDriver::getInstance().getDefaultFramebuffer()->bind();

	_renderTexture->getTexture(0)->generateMipmap();
}
