#ifndef LOADFONT_H_INCLUDED
#define LOADFONT_H_INCLUDED


#include "../Utils/RFont.h"

#include "../Utils/Helpers.hpp"


class FontLoader
{
    private:
        FT_Library _library;
        FT_Face _face;
        RFont* _font;

        std::vector<unsigned char*> _textureData;
        int _textureWidth;
        int _textureHeight;

        int _currentCharLineWidth;
        int _currentLineIndex;
        int _currentLineY;
        std::vector<int> _linesHeights;

        void loadChar(unsigned long charCode, unsigned int glyphIndex);
        void calculateTextureHeight();
        void releaseTextureData();

    public:
        FontLoader();
        ~FontLoader();

        RFont* loadFont(const std::string& fontName, int pixelSize = 32);

        static std::string createFontResourceName(const char* fontName, int pixelSize);
};


#endif // LOADFONT_H_INCLUDED
