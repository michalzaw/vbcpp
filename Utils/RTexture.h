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


enum TextureFilterMode
{
    TFM_NEAREST = GL_NEAREST,
    TFM_LINEAR  = GL_LINEAR,
    TFM_MIPMAP  = GL_LINEAR_MIPMAP_LINEAR,

};


enum TextureClampMode
{
    TCM_REPEAT          = GL_REPEAT,
    TCM_CLAMP           = GL_CLAMP,
    TCM_CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
    TCM_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER

};


class RTexture : virtual public Resource
{
    public:
        RTexture(string path, GLuint id, TextureType type, glm::uvec2 size);
        virtual ~RTexture();

        GLuint getID() { return _texID; }

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
        glm::uvec2  _size;

};

#endif // RTEXTURE_H_INCLUDED
