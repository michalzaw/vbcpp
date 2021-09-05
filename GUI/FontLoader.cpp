#include "FontLoader.h"

#include "GUIObject.h"

#include "../Utils/Strings.h"


FontLoader::FontLoader()
{
    FT_Init_FreeType(&library);

    face = NULL;
    font = NULL;
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


    _textureWidth += font->_characterInfos[index].width;
    _textureHeight = std::max(_textureHeight, font->_characterInfos[index].height);

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

    FT_Set_Pixel_Sizes(face, pixelSize, pixelSize);


    font = new RFont(createFontResourceName(fontName, pixelSize));
    font->_vbo = new VBO(1024 * sizeof(GUIVertex));

    _textureData = new unsigned char*[256];
    _textureDataSize = 0;
    _textureWidth = _textureHeight = 0;

    for (int i = 0; i < 256; ++i)
    {
        loadChar(i);
    }

    // Create texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    _textureWidth = next_p2(_textureWidth);
    _textureHeight = next_p2(_textureHeight);
    font->_characterTexture = new RTexture2D(TF_DEPTH_COMPONENT, glm::uvec2(_textureWidth, _textureHeight));
    font->_characterTexture->setFiltering(TFM_LINEAR, TFM_LINEAR);
    font->_characterTexture->setClampMode(TCM_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

    // Add data to texture and create vbo
    int w = 0;
    for (int i = 0; i < 128; ++i)
    {
        font->_characterTexture->setTexSubImage(_textureData[i], w, 0, font->_characterInfos[i].width, font->_characterInfos[i].height);

        GUIVertex vertices[4];
        vertices[0].position = glm::vec3(0.0f, static_cast<float>(-font->_characterInfos[i].advY + font->_characterInfos[i].height), 0.0f);
        vertices[1].position = glm::vec3(0.0f, static_cast<float>(-font->_characterInfos[i].advY), 0.0f);
        vertices[2].position = glm::vec3(static_cast<float>(font->_characterInfos[i].width), static_cast<float>(-font->_characterInfos[i].advY + font->_characterInfos[i].height), 0.0f);
        vertices[3].position = glm::vec3(static_cast<float>(font->_characterInfos[i].width), static_cast<float>(-font->_characterInfos[i].advY), 0.0f);

        vertices[0].texCoord = glm::vec2(static_cast<float>(w) / _textureWidth, static_cast<float>(font->_characterInfos[i].height) / _textureHeight);
        vertices[1].texCoord = glm::vec2(static_cast<float>(w) / _textureWidth, 0.0f);
        vertices[2].texCoord = glm::vec2(static_cast<float>(w + font->_characterInfos[i].width) / _textureWidth, static_cast<float>(font->_characterInfos[i].height) / _textureHeight);
        vertices[3].texCoord = glm::vec2(static_cast<float>(w + font->_characterInfos[i].width) / _textureWidth, 0.0f);

        font->_vbo->addVertexData(vertices, 4);

        w += font->_characterInfos[i].width;
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
