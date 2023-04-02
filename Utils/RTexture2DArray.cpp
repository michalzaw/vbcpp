#include "RTexture2DArray.h"


RTexture2DArray::RTexture2DArray(TextureFormat internalFormat, const glm::uvec2& size, unsigned int arraySize, bool isMultisample/* = false*/, int samplesCount/* = 0*/)
    : RTexture("", isMultisample ? TT_2D_MULTISAMPLE_ARRAY : TT_2D_ARRAY, internalFormat, size, false, false),
    _arraySize(arraySize)
{
    LOG_INFO("RTexture2DArray - Konstruktor: " + _path);


    glGenTextures(1, &_texID);
    bind();

    if (_textureType == TT_2D_MULTISAMPLE_ARRAY)
        glTexImage3DMultisample(_textureType, samplesCount, _internalFormat, _size.x, _size.y, arraySize, GL_TRUE);
    else
        glTexImage3D(_textureType, 0, _internalFormat, _size.x, _size.y, _arraySize, 0, _format, GL_UNSIGNED_BYTE, nullptr);
}


RTexture2DArray::~RTexture2DArray()
{

}
