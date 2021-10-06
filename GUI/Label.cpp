#include "Label.h"

#include "../Utils/Strings.h"


Label::Label(RFont* font)
    : _font(font),
    _text(""), _width(0), _height(0),
    _maxWidth(0), _maxHeight(0)
{
    _color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


Label::~Label()
{

}


unsigned int finadLastSpace(const std::vector<unsigned int>& str, unsigned int index)
{
    for (unsigned int i = index; i >= 0; --i)
    {
        if (static_cast<unsigned int>(str[i]) == static_cast<unsigned int>(' '))
        {
            return i;
        }
    }

    return -1;
}


void Label::decodeTextToUnicode()
{
    _unicodeText.clear();
    for (unsigned int i = 0; i < _text.size();)
    {
        _unicodeText.push_back(Utf8String::getNextUtf8Char(_text, i));
    }
}


bool Label::buildTextData()
{
    if (_font == NULL)
        return false;

    _width = 0;

    float x = 0;
    float y = 0;

    for (int i = 0; i < _unicodeText.size(); ++i)
    {
        int charIndex = static_cast<int>(_unicodeText[i]);

        x += _font->getCharacterInfo(charIndex).bearingX;

        _charsOffsets.push_back(glm::vec3(x, y, 0.0f));


        if (_maxWidth > 0 && x > _maxWidth)
        {
            unsigned int lastSpaceIndex = finadLastSpace(_unicodeText, i);
            if (lastSpaceIndex > 0 && lastSpaceIndex < _charsOffsets.size() - 1)
            {
                y -= _font->getPixelSize();
                if (_maxHeight > 0 && -y > _maxHeight)
                {
                    break;
                }

                unsigned int firstCharAfterSpaceOffset = _charsOffsets[lastSpaceIndex + 1].x;
                firstCharAfterSpaceOffset -= _font->getCharacterInfo(_unicodeText[lastSpaceIndex + 1]).bearingX;

                for (unsigned int j = lastSpaceIndex + 1; j <= i; ++j)
                {
                    _charsOffsets[j].x = _charsOffsets[j].x - firstCharAfterSpaceOffset;
                    _charsOffsets[j].y = y;
                }

                x = _charsOffsets[i].x;

                if (x > _width)
                {
                    _width = x;
                }
            }
        }

        int charHeight = _font->getCharacterInfo(charIndex).bearingY;
        if (charHeight > _height)
        {
            _height = charHeight;
        }

        if (charIndex == static_cast<int>('\n'))
        {
            y -= static_cast<int>(_font->getPixelSize() * 1.3);
            if (_maxHeight > 0 && -y > _maxHeight)
            {
                break;
            }
            if (x > _width)
            {
                _width = x;
            }
            x = 0;
        }

        x += _font->getCharacterInfo(charIndex).advX - _font->getCharacterInfo(charIndex).bearingX;
    }

    _charsTransforms.resize(_charsOffsets.size());
    _charsTransformsIs.resize(_charsOffsets.size(), false);

    if (x > _width)
    {
        _width = x;
    }

    return true;
}


void Label::clearTextData()
{
    _charsOffsets.clear();
    _charsTransforms.clear();
    _charsTransformsIs.clear();
}


void Label::calculateVerticesTransformMatrix(unsigned int index)
{
    _charsTransforms[index] = glm::translate(glm::vec3(_position.x, _position.y, _position.z)) *
                              glm::rotate(_rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                              glm::scale(glm::vec3(_scale, 1.0f)) *
                              glm::translate(_charsOffsets[index]);
}


void Label::setFont(RFont* font)
{
    _font = font;

    clearTextData();
    buildTextData();
}


void Label::setText(std::string text)
{
    _text = text;
    decodeTextToUnicode();

    clearTextData();
    buildTextData();
}


void Label::setMaxWidth(unsigned int maxWidth)
{
    _maxWidth = maxWidth;

    clearTextData();
    buildTextData();
}


void Label::setMaxHeight(unsigned int maxHeight)
{
    _maxHeight = maxHeight;

    clearTextData();
    buildTextData();
}


RFont* Label::getFont()
{
    return _font;
}


std::string Label::getText()
{
    return _text;
}


unsigned int Label::getWidth()
{
    return _width * _scale.x;
}


unsigned int Label::getHeight()
{
    return _height * _scale.x;
}


unsigned int Label::getMaxWidth()
{
    return _maxWidth;
}


unsigned int Label::getMaxHeight()
{
    return _maxHeight;
}


glm::mat4& Label::getVerticesTransformMatrix(unsigned int vboIndex)
{
    if (!_charsTransformsIs[vboIndex])
    {
        calculateVerticesTransformMatrix(vboIndex);

        _charsTransformsIs[vboIndex] = true;
    }
    return _charsTransforms[vboIndex];
}


void Label::addDataToRenderList(GUIRenderList* renderList)
{
    VBO* vbo = _font->getVBO();

    for (int i = 0; i < _unicodeText.size(); ++i)
    {
        int charIndex = static_cast<int>(_unicodeText[i]);
        renderList->push_back(GUIRenderListElement(vbo, _font->getCharacterInfo(charIndex).vboOffset,
                                                   getVerticesTransformMatrix(i), getTexCoordTransformMatrix(i),
                                                   _color, _font->getTexture(),//s[charIndex],
                                                   GUI_LABEL_SHADER));
    }
}


/*glm::mat4& getTexCoordTransformMatrix(unsigned int vboIndex)
{

}*/
