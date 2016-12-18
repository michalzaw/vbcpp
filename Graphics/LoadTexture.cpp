#include "LoadTexture.h"


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


RTexture2D* loadTexture(const char* fileName, bool mipmaping)
{
    int width, height;
    unsigned char* image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

    RTexture2D* texture = new RTexture2D(fileName, image, TF_RGBA, glm::uvec2(width, height));

    std::cout << "Loading texture: " << fileName << std::endl;
    std::cout << "SOIL result: " << SOIL_last_result() << std::endl;

    if (mipmaping)
    {
        texture->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
    }
    else
    {
        texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    }

    texture->setClampMode(TCM_REPEAT);


    SOIL_free_image_data(image);


    return texture;
}


RTextureCubeMap* loadTextureCubeMap(const char** filesNames, bool mipmaping)
{
    std::cout << "Loading cube map" << std::endl;

    unsigned char* cubeMapFaces[6];
    int width, height;

    for (int i = 0; i < 6; ++i)
    {
        cubeMapFaces[i] = SOIL_load_image(filesNames[i], &width, &height, 0, SOIL_LOAD_RGBA);

        std::cout << "Loading texture: " << filesNames[i] << std::endl;
        std::cout << "SOIL result: " << SOIL_last_result() << std::endl;
    }

    if (width != height)
        return NULL;

    RTextureCubeMap* texture = new RTextureCubeMap("", cubeMapFaces, TF_RGBA, width);

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
        SOIL_free_image_data(cubeMapFaces[i]);
    }


    return texture;
}

GLuint loadTextureRect(const char* fileName)
{
    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);


    int width, height;
    unsigned char* image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

    std::cout << "Loading texture: " << fileName << std::endl;
    std::cout << "SOIL result: " << SOIL_last_result() << std::endl;

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


    SOIL_free_image_data(image);


    return texId;
}
