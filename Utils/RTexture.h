#ifndef RTEXTURE_H_INCLUDED
#define RTEXTURE_H_INCLUDED

#include <GLEW/glew.h>

#include "Resource.h"

class RTexture : virtual public Resource
{
    public:
        RTexture(string path, GLuint id);
        virtual ~RTexture();

        GLuint getID();

    protected:
        GLuint  _texID;
};

#endif // RTEXTURE_H_INCLUDED
