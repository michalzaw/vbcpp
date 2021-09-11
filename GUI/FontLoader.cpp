#include "FontLoader.h"

#include "GUIObject.h"

#include "../Utils/Strings.h"


FontLoader::FontLoader()
{
    FT_Init_FreeType(&library);

    face = NULL;
    font = NULL;

    for (int i = 0; i < 255; ++i)
    {
        _charLinesHeights[i] = 0;
    }
}


FontLoader::~FontLoader()
{
    FT_Done_FreeType(library);
}


void FontLoader::loadChar(int index)
{
    FT_Load_Glyph(face, FT_Get_Char_Index(face, index), FT_LOAD_DEFAULT);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

    FT_Bitmap* bitmap = &face->glyph->bitmap;

    int textureLine = index / 256;

    int width = bitmap->width;
    int height = bitmap->rows;

    _textureData[index] = new unsigned char[width * height];
    ++_textureDataSize;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            _textureData[index][i * width + j] = bitmap->buffer[(height - i - 1) * width + j];
        }
    }

    font->_characterInfos[index].advX = face->glyph->advance.x >> 6;
    font->_characterInfos[index].advY = (face->glyph->metrics.height - face->glyph->metrics.horiBearingY) >> 6;
    font->_characterInfos[index].bearingX = face->glyph->metrics.horiBearingX >> 6;
    font->_characterInfos[index].bearingY = face->glyph->metrics.horiBearingY >> 6;
    font->_characterInfos[index].width = face->glyph->metrics.width >> 6;
    font->_characterInfos[index].height = face->glyph->metrics.height >> 6;

    _currentCharLineWidth += font->_characterInfos[index].width + 2;
    _charLinesHeights[textureLine] = std::max(_charLinesHeights[textureLine], font->_characterInfos[index].height);
}


void FontLoader::releaseTextureData()
{
    for (int i = 0; i < _textureDataSize; ++i)
    {
        delete[] _textureData[i];
    }

    delete[] _textureData;
}


RFont* FontLoader::loadFont(const char* fontName, int pixelSize)
{
    bool result = FT_New_Face(library, fontName, 0, &face);
    if (result)
    {
        return NULL;
    }

    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(face, pixelSize, pixelSize);


    FT_UInt glyphIndex;
    FT_ULong charCode = FT_Get_First_Char(face, &glyphIndex);

    int charsCount = 0;
    while (glyphIndex != 0)
    {
        // TODO: FT_Get_Next_Char can return same glyphIndex for different charCode
        //charCodes[glyphIndex] = charCode;

        charCode = FT_Get_Next_Char(face, charCode, &glyphIndex);
        ++charsCount;
    }

    std::cout << "\n\nCount: " << charsCount << std::endl;


    font = new RFont(createFontResourceName(fontName, pixelSize), pixelSize);
    font->_vbo = new VBO(256 * 256 * 4 * sizeof(GUIVertex));

    _textureData = new unsigned char*[256 * 256];
    _textureDataSize = 0;
    _textureWidth = _textureHeight = 0;
    _currentCharLineWidth = 0;

    for (int i = 0; i < 256 * 256; ++i)
    {
        loadChar(i);

        if (i > 0 && i % 256 == 0)
        {
            _textureWidth = std::max(_currentCharLineWidth, _textureWidth);
            _currentCharLineWidth = 0;
        }
    }

    for (int i = 0; i < 255; ++i)
    {
        _textureHeight += _charLinesHeights[i];
        //std::cout << _charLinesHeights[i] << std::endl;
    }
    // Create texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    _textureWidth = next_p2(_textureWidth);
    _textureHeight = next_p2(_textureHeight);
    font->_characterTexture = new RTexture2D(TF_DEPTH_COMPONENT, glm::uvec2(_textureWidth, _textureHeight));
    font->_characterTexture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    font->_characterTexture->setClampMode(TCM_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
    //std::cout << "\n\n\n\n widtd: " << _textureWidth << "\n height: " << _textureHeight << std::endl;

    // Add data to texture and create vbo
    int w = 0;
    int h = 0;
    for (int i = 0; i < 256 * 256; ++i)
    {
        if (i > 0 && i % 256 == 0)
        {
            w = 0;
            h += _charLinesHeights[i / 256 - 1] + 2;
        }

        font->_characterTexture->setTexSubImage(_textureData[i], w, h, font->_characterInfos[i].width, font->_characterInfos[i].height);

        GUIVertex vertices[4];
        vertices[0].position = glm::vec3(0.0f, static_cast<float>(-font->_characterInfos[i].advY + font->_characterInfos[i].height), 0.0f);
        vertices[1].position = glm::vec3(0.0f, static_cast<float>(-font->_characterInfos[i].advY), 0.0f);
        vertices[2].position = glm::vec3(static_cast<float>(font->_characterInfos[i].width), static_cast<float>(-font->_characterInfos[i].advY + font->_characterInfos[i].height), 0.0f);
        vertices[3].position = glm::vec3(static_cast<float>(font->_characterInfos[i].width), static_cast<float>(-font->_characterInfos[i].advY), 0.0f);

        vertices[0].texCoord = glm::vec2(static_cast<float>(w) / _textureWidth, static_cast<float>(h + font->_characterInfos[i].height) / _textureHeight);
        vertices[1].texCoord = glm::vec2(static_cast<float>(w) / _textureWidth, static_cast<float>(h) / _textureHeight);
        vertices[2].texCoord = glm::vec2(static_cast<float>(w + font->_characterInfos[i].width) / _textureWidth, static_cast<float>(h + font->_characterInfos[i].height) / _textureHeight);
        vertices[3].texCoord = glm::vec2(static_cast<float>(w + font->_characterInfos[i].width) / _textureWidth, static_cast<float>(h) / _textureHeight);

        font->_vbo->addVertexData(vertices, 4);

        w += font->_characterInfos[i].width + 2;
    }


    FT_Done_Face(face);

    face = NULL;


    releaseTextureData();


    return font;
}


std::string FontLoader::createFontResourceName(const char* fontName, int pixelSize)
{
    return std::string(fontName) + ";" + toString(pixelSize);
}
