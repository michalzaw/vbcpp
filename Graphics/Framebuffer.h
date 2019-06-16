#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED


#include <vector>

#include <GL/glew.h>

#include "Rect.h"

#include "../Utils/RTexture2D.h"


class Framebuffer
{
    friend class OGLDriver;

    private:
        GLuint _fboId;
        std::vector<RTexture*> _textures;
        GLuint _depthRenderbuffer;

        std::vector<GLenum> _fboBuffs;

        UintRect _viewport;

        bool _isInitialized;

    public:
        Framebuffer();
        ~Framebuffer();

        void init();
        void bind();

        void addDepthRenderbuffer(unsigned int width, unsigned int height);
        void addTexture(TextureFormat format, unsigned int width, unsigned int height);

        int getTexturesCount();
        RTexture* getTexture(int index = 0);

        void setViewport(const UintRect& viewport);
        UintRect& getViewport();

};


#endif // FRAMEBUFFER_H_INCLUDED
