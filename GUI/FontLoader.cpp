#include "FontLoader.h"

#include "GUIObject.h"

#include "../Utils/Logger.h"
#include "../Utils/Strings.h"


FontLoader::FontLoader()
    : _face(nullptr), _font(nullptr)
{
    FT_Init_FreeType(&_library);
}


FontLoader::~FontLoader()
{
    FT_Done_FreeType(_library);
}


void FontLoader::loadChar(unsigned long charCode, unsigned int glyphIndex)
{
    FT_Load_Glyph(_face, glyphIndex, FT_LOAD_DEFAULT);
    FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);

    FT_Bitmap* bitmap = &_face->glyph->bitmap;

    int width = bitmap->width;
    int height = bitmap->rows;

    unsigned char* textureData = new unsigned char[width * height];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            textureData[i * width + j] = bitmap->buffer[(height - i - 1) * width + j];
        }
    }
    _textureData.push_back(textureData);

    if (_currentCharLineWidth + width + CHAR_MARGIN > MAX_TEXTURE_WIDTH)
    {
        _currentLineY += _linesHeights[_currentLineIndex] + CHAR_MARGIN;
        ++_currentLineIndex;
        _linesHeights.push_back(0);

        _textureWidth = std::max(_textureWidth, _currentCharLineWidth);

        _currentCharLineWidth = 0;
    }

    _font->_characterInfos[charCode].advX = _face->glyph->advance.x >> 6;
    _font->_characterInfos[charCode].advY = (_face->glyph->metrics.height - _face->glyph->metrics.horiBearingY) >> 6;
    _font->_characterInfos[charCode].bearingX = _face->glyph->metrics.horiBearingX >> 6;
    _font->_characterInfos[charCode].bearingY = _face->glyph->metrics.horiBearingY >> 6;
    _font->_characterInfos[charCode].width = _face->glyph->metrics.width >> 6;
    _font->_characterInfos[charCode].height = _face->glyph->metrics.height >> 6;

    _font->_characterInfos[charCode].x = _currentCharLineWidth;
    _font->_characterInfos[charCode].y = _currentLineY;

    _currentCharLineWidth += _font->_characterInfos[charCode].width + CHAR_MARGIN;
    _linesHeights[_currentLineIndex] = std::max(_linesHeights[_currentLineIndex], _font->_characterInfos[charCode].height);
}


void FontLoader::calculateTextureHeight()
{
    for (int i = 0; i < _linesHeights.size(); ++i)
    {
        _textureHeight += _linesHeights[i];
    }
}


void FontLoader::releaseTextureData()
{
    for (int i = 0; i < _textureData.size(); ++i)
    {
        delete[] _textureData[i];
    }
}


RFont* FontLoader::loadFont(const std::string& fontName, int pixelSize)
{
    Logger::info("Loading font: " + fontName);

    if (pixelSize <= 0)
    {
        Logger::error("Invalid pixelSize");
        return nullptr;
    }

    bool result = FT_New_Face(_library, fontName.c_str(), 0, &_face);
    if (result)
    {
        Logger::error("Cannot load font: " + fontName);
        return nullptr;
    }

    FT_Select_Charmap(_face, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(_face, pixelSize, pixelSize);

    _textureWidth = _textureHeight = 0;
    _currentCharLineWidth = 0;
    _currentLineIndex = 0;
    _currentLineY = 0;
    _linesHeights.push_back(0);

    _font = new RFont(createFontResourceName(fontName.c_str(), pixelSize), pixelSize);

    int charsCount = 0;
    unsigned int glyphIndex;
    unsigned long charCode = FT_Get_First_Char(_face, &glyphIndex);
    while (glyphIndex != 0)
    {
        loadChar(charCode, glyphIndex);

        charCode = FT_Get_Next_Char(_face, charCode, &glyphIndex);
        ++charsCount;
    }
    Logger::info("Loaded " + toString(charsCount) + " chars");

    calculateTextureHeight();

    // Create texture
    GLint initialUnpackAlignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &initialUnpackAlignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    _textureWidth = next_p2(_textureWidth);
    _textureHeight = next_p2(_textureHeight);

    _font->_characterTexture = new RTexture2D(TF_DEPTH_COMPONENT, glm::uvec2(_textureWidth, _textureHeight));
    _font->_characterTexture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    _font->_characterTexture->setClampMode(TCM_CLAMP_TO_EDGE);

    _font->_vbo = new VBO(charsCount * 4 * sizeof(GUIVertex));

    charCode = FT_Get_First_Char(_face, &glyphIndex);
    int index = 0;
    while (glyphIndex != 0)
    {
        _font->_characterTexture->setTexSubImage(_textureData[index], _font->_characterInfos[charCode].x, _font->_characterInfos[charCode].y, _font->_characterInfos[charCode].width, _font->_characterInfos[charCode].height);

        GUIVertex vertices[4];
        vertices[0].position = glm::vec3(0.0f, static_cast<float>(-_font->_characterInfos[charCode].advY + _font->_characterInfos[charCode].height), 0.0f);
        vertices[1].position = glm::vec3(0.0f, static_cast<float>(-_font->_characterInfos[charCode].advY), 0.0f);
        vertices[2].position = glm::vec3(static_cast<float>(_font->_characterInfos[charCode].width), static_cast<float>(-_font->_characterInfos[charCode].advY + _font->_characterInfos[charCode].height), 0.0f);
        vertices[3].position = glm::vec3(static_cast<float>(_font->_characterInfos[charCode].width), static_cast<float>(-_font->_characterInfos[charCode].advY), 0.0f);

        vertices[0].texCoord = glm::vec2(static_cast<float>(_font->_characterInfos[charCode].x) / _textureWidth, static_cast<float>(_font->_characterInfos[charCode].y + _font->_characterInfos[charCode].height) / _textureHeight);
        vertices[1].texCoord = glm::vec2(static_cast<float>(_font->_characterInfos[charCode].x) / _textureWidth, static_cast<float>(_font->_characterInfos[charCode].y) / _textureHeight);
        vertices[2].texCoord = glm::vec2(static_cast<float>(_font->_characterInfos[charCode].x + _font->_characterInfos[charCode].width) / _textureWidth, static_cast<float>(_font->_characterInfos[charCode].y + _font->_characterInfos[charCode].height) / _textureHeight);
        vertices[3].texCoord = glm::vec2(static_cast<float>(_font->_characterInfos[charCode].x + _font->_characterInfos[charCode].width) / _textureWidth, static_cast<float>(_font->_characterInfos[charCode].y) / _textureHeight);

        _font->_vbo->addVertexData(vertices, 4);

        _font->_characterInfos[charCode].vboOffset = index * 4;

        charCode = FT_Get_Next_Char(_face, charCode, &glyphIndex);
        ++index;
    }


    FT_Done_Face(_face);

    _face = nullptr;


    releaseTextureData();


    glPixelStorei(GL_UNPACK_ALIGNMENT, initialUnpackAlignment);


    return _font;
}


std::string FontLoader::createFontResourceName(const char* fontName, int pixelSize)
{
    return std::string(fontName) + ";" + toString(pixelSize);
}
