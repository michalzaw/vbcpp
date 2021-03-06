#ifndef LOADFONT_H_INCLUDED
#define LOADFONT_H_INCLUDED


#include "../Utils/RFont.h"

#include "../Utils/Helpers.hpp"


class FontLoader
{
    private:
        FT_Library library;
        FT_Face face;
        RFont* font;

        unsigned char** _textureData;
        unsigned int _textureDataSize;
        int _textureWidth;
        int _textureHeight;

        void loadChar(int index);
        void releaseTextureData();

    public:
        FontLoader();
        ~FontLoader();

        RFont* loadFont(const char* fontName, int pixelSize = 32);
};


#endif // LOADFONT_H_INCLUDED
