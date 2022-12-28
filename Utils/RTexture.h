#ifndef RTEXTURE_H_INCLUDED
#define RTEXTURE_H_INCLUDED

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Resource.h"


enum TextureType
{
    TT_1D   = GL_TEXTURE_1D,
    TT_2D   = GL_TEXTURE_2D,
    TT_3D   = GL_TEXTURE_3D,
    TT_CUBE = GL_TEXTURE_CUBE_MAP,
    TT_RECT = GL_TEXTURE_RECTANGLE,

    TT_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE

};


enum TextureFormat
{
    TF_RGB              = GL_RGB,
    TF_RGBA             = GL_RGBA,
    TF_RGB_16F          = GL_RGB16F,
    TF_RGB_32F          = GL_RGB32F,
    TF_RGBA_16F         = GL_RGBA16F,
    TF_RGBA_32F         = GL_RGBA32F,
    TF_RED_32           = GL_R32UI,
    TF_RED_INTEGER      = GL_RED_INTEGER,
    TF_DEPTH_COMPONENT  = GL_DEPTH_COMPONENT

};


enum TextureFilterMode
{
    TFM_NEAREST     = GL_NEAREST,
    TFM_LINEAR      = GL_LINEAR,
    TFM_TRILINEAR   = GL_LINEAR_MIPMAP_LINEAR,

};


enum TextureClampMode
{
    TCM_REPEAT          = GL_REPEAT,
    TCM_CLAMP           = GL_CLAMP,
    TCM_CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
    TCM_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER

};


class RTexture : public Resource
{
    friend class Framebuffer;

    public:
        //RTexture(string path, GLuint id, TextureType type, glm::uvec2 size);
        //RTexture(string path, unsigned char* data, TextureType type, TextureFormat format, glm::uvec2 size);
        //RTexture(TextureType type, TextureFormat format, glm::uvec2 size);
        RTexture(std::string path, TextureType type, TextureFormat internalFormat, glm::uvec2 size, bool fromFile, bool compressed = false);
        virtual ~RTexture();

        GLuint getID() { return _texID; }

        void setFiltering(TextureFilterMode minFilter, TextureFilterMode magFilter);
        void setClampMode(TextureClampMode mode);

        void setAnisotropyFiltering(bool isEnable, float anisotropy = 1.0f);

        void setParameter(GLenum name, GLint value);
        void setParameter(GLenum name, GLfloat value);

        glm::uvec2 getSize();

        TextureType getTextureType();
		TextureFormat getInternalFormat();

		bool isCompressed();
		bool isLoadedFromFile();

        inline void bind()
        {
            glBindTexture(_textureType, _texID);
        }

		void generateMipmap();

    protected:
        GLuint      _texID;
        TextureType _textureType;
        TextureFormat _internalFormat;
        TextureFormat _format;
        glm::uvec2  _size;
		bool _compressed;
		bool _fromFile;

        bool        _isGenerateMipmap;

};

#endif // RTEXTURE_H_INCLUDED
