#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED


#include <vector>

#include <GL/glew.h>

#include "Rect.h"

#include "../Utils/RTexture2D.h"
#include "../Utils/RTextureCubeMap.h"


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

        void checkFramebufferStatus();

    public:
        Framebuffer();
        ~Framebuffer();

        void init();
        void bind();
		void bindCubeMapFaceToRender(CubeMapFace face, int textureIndex = 0);

        void addDepthRenderbuffer(unsigned int width, unsigned int height, bool multisample = false, int samplesCount = 0);
        void addTexture(TextureFormat format, unsigned int width, unsigned int height, bool multisample = false, int samplesCount = 0);
		void addCubeMapTexture(TextureFormat format, unsigned int size);

        int getTexturesCount();
        RTexture* getTexture(int index = 0);

        void setViewport(const UintRect& viewport);
        UintRect& getViewport();

};


#endif // FRAMEBUFFER_H_INCLUDED
