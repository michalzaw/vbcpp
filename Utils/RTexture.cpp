#include "RTexture.h"

#include <iostream>

RTexture::RTexture(string path, GLuint id)
: Resource(path),
_texID(id)
{
    std::cout << "RTexture - Konstruktor: " << _path << std::endl;
}

RTexture::~RTexture()
{
    std::cout << "RTexture - Destruktor: " << _path << std::endl;
    glDeleteTextures(1, &_texID);
}


GLuint RTexture::getID()
{
    return _texID;
}
