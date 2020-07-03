#ifndef RTEXTURECUBEMAP_H_INCLUDED
#define RTEXTURECUBEMAP_H_INCLUDED


#include "RTexture.h"


enum CubeMapFace
{
    CMF_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    CMF_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    CMF_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    CMF_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    CMF_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    CMF_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,

};


class RTextureCubeMap : public RTexture
{
    private:

    public:
        // data: pos_x, neg_x, pos_y, neg_y, pos_z, neg_z
        RTextureCubeMap(std::string path, unsigned char** data, TextureFormat internalFormat, unsigned int size, bool fromFile);
		RTextureCubeMap(std::string path, float** data, TextureFormat internalFormat, unsigned int size, bool fromFile);
        RTextureCubeMap(TextureFormat internalFormat, unsigned int size);
        virtual ~RTextureCubeMap();

        void setTexSubImage(unsigned char* data, CubeMapFace face, int offsetX, int offsetY, int width, int height);
		void setTexSubImage(float* data, CubeMapFace face, int offsetX, int offsetY, int width, int height);

};


#endif // RTEXTURECUBEMAP_H_INCLUDED
