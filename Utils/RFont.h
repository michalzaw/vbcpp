#ifndef RFONT_H_INCLUDED
#define RFONT_H_INCLUDED


#include <freetype/ft2build.h>
#include FT_FREETYPE_H
//#include <freetype/freetype.h>

#include "../Graphics/VBO.h"

#include "Resource.h"
#include "RTexture2D.h"


struct CharacterInfo
{
    int advX;
    int advY;
    int bearingX;
    int bearingY;
    int width;
    int height;

};


class RFont : virtual public Resource
{
    friend class FontLoader;

    private:
        CharacterInfo _characterInfos[256];
        RTexture2D* _characterTexture;

        VBO* _vbo;

    public:
        RFont(std::string path);
        virtual ~RFont();

        const CharacterInfo& getCharacterInfo(char character);
        RTexture2D* getTexture();
        VBO* getVBO();

};


#endif // RFONT_H_INCLUDED
