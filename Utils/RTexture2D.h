#ifndef RTEXTURE2D_H_INCLUDED
#define RTEXTURE2D_H_INCLUDED


#include "RTexture.h"


class RTexture2D : public RTexture
{
    private:
		GLuint mapInternalFormatToCompressedInternalFormat(GLuint internalFormat);

    public:
        RTexture2D(string path, unsigned char* data, TextureFormat internalFormat, glm::uvec2 size, bool useCompression = false);
        RTexture2D(string path, float* data, TextureFormat internalFormat, glm::uvec2 size, bool useCompression = false);
        RTexture2D(TextureFormat internalFormat, glm::uvec2 size, bool isMultisample = false, int samplesCount = 0);
		RTexture2D(string path, GLuint textureId, glm::uvec2 size, TextureFilterMode minFilter, TextureFilterMode magFilter); // for dds texture
        virtual ~RTexture2D();

        void setTexSubImage(unsigned char* data, int offsetX, int offsetY, int width, int height);
        void setTexSubImage(float* data, int offsetX, int offsetY, int width, int height);

        static RTexture2D* createWhiteTexture(string path, glm::uvec2 size, bool useCompression = false);
		static RTexture2D* createOneColorTexture(string name, glm::vec4 color, bool useCompression = false);

};


#endif // RTEXTURE2D_H_INCLUDED
