#include "Framebuffer.h"

#include  "../Utils/Logger.h"


Framebuffer::Framebuffer()
    : _fboId(0), _depthRenderbuffer(0), _isInitialized(false)
{

}


Framebuffer::~Framebuffer()
{
    if (_depthRenderbuffer != 0)
        glDeleteRenderbuffers(1, &_depthRenderbuffer);

    for (int i = 0; i < _textures.size(); ++i)
    {
        delete _textures[i];
    }
    _textures.size();

    if (_fboId != 0)
        glDeleteFramebuffers(1, &_fboId);
}


void Framebuffer::checkFramebufferStatus()
{
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger::error("Incomplete FBO:");

        switch (status)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                Logger::error("FBO error: GL_FRAMEBUFFER_UNDEFINED");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                Logger::error("FBO error: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                Logger::error("FBO error: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                Logger::error("FBO error: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                Logger::error("FBO error: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                Logger::error("FBO error: GL_FRAMEBUFFER_UNSUPPORTED");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                Logger::error("FBO error: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                Logger::error("FBO error: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
                break;
        }
    }
}


void Framebuffer::init()
{
    if (!_isInitialized)
    {
        glGenFramebuffers(1, &_fboId);

        _isInitialized = true;
    }
}


void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fboId);

    glViewport(_viewport.position.x, _viewport.position.y, _viewport.size.x, _viewport.size.y);
}


void Framebuffer::bindCubeMapFaceToRender(CubeMapFace face, int textureIndex, int mipmapLevel)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, _textures[textureIndex]->_texID, mipmapLevel);

	glDrawBuffers(_fboBuffs.size(), &_fboBuffs[0]);
}


void Framebuffer::addDepthRenderbuffer(unsigned int width, unsigned int height, bool multisample, int samplesCount)
{
    bind();

    glGenRenderbuffers(1, &_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
    if (!multisample)
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    }
    else
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplesCount, GL_DEPTH_COMPONENT /*GL_DEPTH32F_STENCIL8*/, width, height);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);

    checkFramebufferStatus();
}


void Framebuffer::addTexture(TextureFormat format, unsigned int width, unsigned int height, bool multisample, int samplesCount)
{
    bind();

    RTexture2D* texture = new RTexture2D(format, glm::uvec2(width, height), multisample, samplesCount);
    texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    texture->setClampMode(TCM_CLAMP);

    GLenum attachment;
    if (format == TF_DEPTH_COMPONENT)
    {
        attachment = GL_DEPTH_ATTACHMENT;
    }
    else
    {
        attachment = GL_COLOR_ATTACHMENT0 + _fboBuffs.size();

        _fboBuffs.push_back(attachment);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture->getTextureType(), texture->_texID, 0);

    _textures.push_back(texture);

    if (_fboBuffs.size() > 0)
    {
        glDrawBuffers(_fboBuffs.size(), &_fboBuffs[0]);
    }
    else
    {
        glDrawBuffer(GL_NONE);
    }

    checkFramebufferStatus();
}


void Framebuffer::addCubeMapTexture(TextureFormat format, unsigned int size, bool mipmaping)
{
	bind();

	RTextureCubeMap* texture = new RTextureCubeMap(format, size);
	texture->setFiltering(mipmaping ? TFM_TRILINEAR : TFM_LINEAR, TFM_LINEAR);
	texture->setClampMode(TCM_CLAMP_TO_EDGE);

	_textures.push_back(texture);

	_fboBuffs.push_back(GL_COLOR_ATTACHMENT0);

	checkFramebufferStatus();
}


int Framebuffer::getTexturesCount()
{
    return _textures.size();
}


RTexture* Framebuffer::getTexture(int index)
{
    if (index < _textures.size())
        return _textures[index];

    return NULL;
}


void Framebuffer::setViewport(const UintRect& viewport)
{
    _viewport = viewport;

    bind();
}


UintRect& Framebuffer::getViewport()
{
    return _viewport;
}
