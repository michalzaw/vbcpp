#include "LoadTexture.h"

#include <stb_image/stb_image.h>

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


RTexture2D* loadTexture(const char* fileName, bool mipmaping, RTexture2D* oldTexture)
{
    Logger::info("Loading texture: " + std::string(fileName));

    stbi_set_flip_vertically_on_load(false);
    int width, height, chanels;
    //unsigned char* image = SOIL_load_image(fileName, &width, &height, &chanels, SOIL_LOAD_RGBA);
    unsigned char* image = stbi_load(fileName, &width, &height, &chanels, STBI_rgb_alpha);

    if(image == NULL)
    {
        Logger::error("Failed to load texture: " + std::string(fileName));
        return NULL;
    }

    RTexture2D* texture;
    if (oldTexture == NULL)
    {
        texture = new RTexture2D(fileName, image, TF_RGBA, glm::uvec2(width, height));
    }
    else
    {
        texture = oldTexture;
        texture->setTexSubImage(image, 0, 0, width, height);
    }

    if (mipmaping)
    {
        texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
    }
    else
    {
        texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    }

    texture->setClampMode(TCM_REPEAT);


    //SOIL_free_image_data(image);
    stbi_image_free(image);


    return texture;
}


RTexture2D* loadTextureHDR(const char* fileName, bool mipmaping, RTexture2D* oldTexture)
{
    Logger::info("Loading texture HDR: " + std::string(fileName));

    stbi_set_flip_vertically_on_load(true);
    int width, height, chanels;
    float *image = stbi_loadf(fileName, &width, &height, &chanels, 0);

    if(image == NULL)
    {
        Logger::error("Failed to load texture: " + std::string(fileName));
        return NULL;
    }

    RTexture2D* texture;
    if (oldTexture == NULL)
    {
        texture = new RTexture2D(fileName, image, TF_RGB_16F, glm::uvec2(width, height));
    }
    else
    {
        texture = oldTexture;
        texture->setTexSubImage(image, 0, 0, width, height);
    }

    if (mipmaping)
    {
        texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
    }
    else
    {
        texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    }

    texture->setClampMode(TCM_CLAMP_TO_EDGE);


    stbi_image_free(image);


    return texture;
}


RTextureCubeMap* loadTextureCubeMap(std::string* filesNames, const char* path, bool mipmaping, RTextureCubeMap* oldTexture)
{
    Logger::info("Loading cube map");

    stbi_set_flip_vertically_on_load(false);

    unsigned char* cubeMapFaces[6];
    int width, height, chanels;

    for (int i = 0; i < 6; ++i)
    {
        //cubeMapFaces[i] = SOIL_load_image(filesNames[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
        cubeMapFaces[i] = stbi_load(filesNames[i].c_str(), &width, &height, &chanels, STBI_rgb_alpha);

        Logger::info("Loading texture: " + filesNames[i]);
        if(cubeMapFaces[i] == NULL)
        {
            Logger::error("Failed to load texture: " + filesNames[i]);
            return NULL;
        }

        if (oldTexture != NULL)
        {
            oldTexture->setTexSubImage(cubeMapFaces[i], (CubeMapFace)(CMF_POSITIVE_X + i), 0, 0, width, height);
        }
    }

    if (width != height)
        return NULL;

    RTextureCubeMap* texture;
    if (oldTexture == NULL)
        texture = new RTextureCubeMap(path, cubeMapFaces, TF_RGBA, width);
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
        //SOIL_free_image_data(cubeMapFaces[i]);
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
