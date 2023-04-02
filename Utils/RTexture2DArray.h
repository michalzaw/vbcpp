#ifndef RTEXTURE2DARRAY_H_INCLUDED
#define RTEXTURE2DARRAY_H_INCLUDED


#include "RTexture.h"


class RTexture2DArray : public RTexture
{
    private:
        unsigned int _arraySize;

    public:
        RTexture2DArray(TextureFormat internalFormat, const glm::uvec2& size, unsigned int arraySize, bool isMultisample = false, int samplesCount = 0);
        virtual ~RTexture2DArray();

};


#endif // RTEXTURE2DARRAY_H_INCLUDED
