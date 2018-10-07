#include "Framebuffer.h"


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


void Framebuffer::addDepthRenderbuffer(unsigned int width, unsigned int height)
{
    bind();

    glGenRenderbuffers(1, &_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);
}


void Framebuffer::addTexture(TextureFormat format, unsigned int width, unsigned int height)
{
    bind();

    RTexture2D* texture = new RTexture2D(format, glm::uvec2(width, height));
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

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->_texID, 0);

    _textures.push_back(texture);

    if (_fboBuffs.size() > 0)
    {
        glDrawBuffers(_fboBuffs.size(), &_fboBuffs[0]);
    }
    else
    {
        glDrawBuffer(GL_NONE);
    }
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
