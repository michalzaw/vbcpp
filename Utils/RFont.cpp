#include "RFont.h"


RFont::RFont(std::string path)
    : Resource(RT_FONT_RESOURCE, path),
    _characterTexture(NULL), _vbo(NULL)
{

}


RFont::~RFont()
{
    if (_characterTexture != NULL)
        delete _characterTexture;

    if (_vbo != NULL)
        delete _vbo;
}


const CharacterInfo& RFont::getCharacterInfo(char character)
{
    return _characterInfos[character];
}


RTexture2D* RFont::getTexture()
{
    return _characterTexture;
}


VBO* RFont::getVBO()
{
    return _vbo;
}
