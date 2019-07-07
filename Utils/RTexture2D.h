#ifndef RTEXTURE2D_H_INCLUDED
#define RTEXTURE2D_H_INCLUDED


#include "RTexture.h"


class RTexture2D : public RTexture
{
    private:

    public:
        RTexture2D(string path, unsigned char* data, TextureFormat internalFormat, glm::uvec2 size);
        RTexture2D(TextureFormat internalFormat, glm::uvec2 size, bool isMultisample = false, int samplesCount = 0);
        virtual ~RTexture2D();

        void setTexSubImage(unsigned char* data, int offsetX, int offsetY, int width, int height);

        static RTexture2D* createWhiteTexture(string path, glm::uvec2 size);

};


#endif // RTEXTURE2D_H_INCLUDED
