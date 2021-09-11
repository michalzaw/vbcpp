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

        int _charLinesHeights[256];
        int _currentCharLineWidth;

        void loadChar(int index);
        void releaseTextureData();

    public:
        FontLoader();
        ~FontLoader();

        RFont* loadFont(const char* fontName, int pixelSize = 32);

        static std::string createFontResourceName(const char* fontName, int pixelSize);
};


#endif // LOADFONT_H_INCLUDED
