#include "LoadTexture.h"

#include <stb_image.h>

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger.h"


/*GLuint loadTexture(const char* fileName, int* width, int* height, bool mipmaping)
{
    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);


    //int width, height;
    unsigned char* image = SOIL_load_image(fileName, width, height, 0, SOIL_LOAD_RGBA);

    std::cout << "Loading texture: " << fileName << std::endl;
    std::cout << "SOIL result: " << SOIL_last_result() << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    if (mipmaping)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    SOIL_free_image_data(image);


    return texId;
}*/


const std::string HDR_FILE_EXTENSION = "hdr";


RTexture2D* loadTexture(const char* fileName, bool mipmaping, RTexture2D* oldTexture)
{
	std::string fileNameStr(fileName);
	bool hdrImage = FilesHelper::getFileExtension(fileNameStr) == HDR_FILE_EXTENSION;

    Logger::info("Loading texture: " + fileNameStr);

	if (hdrImage)
		stbi_set_flip_vertically_on_load(true);
	else
		stbi_set_flip_vertically_on_load(false);

    int width, height, chanels;
	void* image;
	if (hdrImage)
		image = stbi_loadf(fileName, &width, &height, &chanels, 0);
	else
		image = stbi_load(fileName, &width, &height, &chanels, STBI_rgb_alpha);
    

    if(image == NULL)
    {
        Logger::error("Failed to load texture: " + std::string(fileName));
        return NULL;
    }

    RTexture2D* texture;
    if (oldTexture == NULL)
    {
		if (hdrImage)
			texture = new RTexture2D(fileName, static_cast<float*>(image), chanels == 4 ? TF_RGBA_16F : TF_RGB_16F, glm::uvec2(width, height));
		else
			texture = new RTexture2D(fileName, static_cast<unsigned char*>(image), TF_RGBA, glm::uvec2(width, height));

		if (mipmaping)
		{
			texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
		}
		else
		{
			texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
		}

		if (hdrImage)
			texture->setClampMode(TCM_CLAMP_TO_EDGE);
		else
			texture->setClampMode(TCM_REPEAT);
    }
    else
    {
        texture = oldTexture;
		if (hdrImage)
			texture->setTexSubImage(static_cast<float*>(image), 0, 0, width, height);
		else
			texture->setTexSubImage(static_cast<unsigned char*>(image), 0, 0, width, height);
    }

    stbi_image_free(image);


    return texture;
}


RTextureCubeMap* loadTextureCubeMap(std::string* filesNames, const char* path, bool mipmaping, RTextureCubeMap* oldTexture)
{
	bool hdrImage = FilesHelper::getFileExtension(filesNames[0]) == HDR_FILE_EXTENSION;

    Logger::info("Loading cube map");

    stbi_set_flip_vertically_on_load(false);

    void* cubeMapFaces[6];
    int width, height, chanels;

    for (int i = 0; i < 6; ++i)
    {
		if (hdrImage)
			cubeMapFaces[i] = stbi_loadf(filesNames[i].c_str(), &width, &height, &chanels, 0);
		else
			cubeMapFaces[i] = stbi_load(filesNames[i].c_str(), &width, &height, &chanels, STBI_rgb_alpha);

        Logger::info("Loading texture: " + filesNames[i]);
        if(cubeMapFaces[i] == NULL)
        {
            Logger::error("Failed to load texture: " + filesNames[i]);
            return NULL;
        }

        if (oldTexture != NULL)
        {
			if (hdrImage)
				oldTexture->setTexSubImage((float*)cubeMapFaces[i], (CubeMapFace)(CMF_POSITIVE_X + i), 0, 0, width, height);
			else
				oldTexture->setTexSubImage((unsigned char*)cubeMapFaces[i], (CubeMapFace)(CMF_POSITIVE_X + i), 0, 0, width, height);
        }
    }

    if (width != height)
        return NULL;

    RTextureCubeMap* texture;
	if (oldTexture == NULL)
	{
		if (hdrImage)
			texture = new RTextureCubeMap(path, (float**)cubeMapFaces, chanels == 4 ? TF_RGBA_16F : TF_RGB_16F, width);
		else
			texture = new RTextureCubeMap(path, (unsigned char**)cubeMapFaces, TF_RGBA, width);
	}
    else
        texture = oldTexture;

    if (mipmaping)
    {
        texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
    }
    else
    {
        texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    }

    texture->setClampMode(TCM_CLAMP_TO_EDGE);


    for (int i = 0; i < 6; ++i)
    {
        stbi_image_free(cubeMapFaces[i]);
    }


    return texture;
}

// Unused
GLuint loadTextureRect(const char* fileName)
{
    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);


    int width, height;
    unsigned char* image;// = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

    std::cout << "Loading texture: " << fileName << std::endl;
    //std::cout << "SOIL result: " << SOIL_last_result() << std::endl;

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


    //SOIL_free_image_data(image);


    return texId;
}
