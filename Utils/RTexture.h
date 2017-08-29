#ifndef RTEXTURE_H_INCLUDED
#define RTEXTURE_H_INCLUDED

#include <GLEW/glew.h>

#include <glm/glm.hpp>

#include "Resource.h"


enum TextureType
{
    TT_1D   = GL_TEXTURE_1D,
    TT_2D   = GL_TEXTURE_2D,
    TT_3D   = GL_TEXTURE_3D,
    TT_CUBE = GL_TEXTURE_CUBE_MAP,
    TT_RECT = GL_TEXTURE_RECTANGLE

};


enum TextureFormat
{
    TF_RGB              = GL_RGB,
    TF_RGBA             = GL_RGBA,
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
        RTexture(std::string path, TextureType type, TextureFormat format, glm::uvec2 size);
        virtual ~RTexture();

        //GLuint getID() { return _texID; }

        void setFiltering(TextureFilterMode minFilter, TextureFilterMode magFilter);
        void setClampMode(TextureClampMode mode);

        void setAnisotropyFiltering(bool isEnable, float anisotropy = 1.0f);

        glm::uvec2 getSize();

        inline void bind()
        {
            glBindTexture(_type, _texID);
        }

    protected:
        GLuint      _texID;
        TextureType _type;
        TextureFormat _format;
        glm::uvec2  _size;

        bool        _isGenerateMipmap;

        void generateMipmap();

};

#endif // RTEXTURE_H_INCLUDED
