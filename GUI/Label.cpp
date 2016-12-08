#include "Label.h"


Label::Label(RFont* font)
    : _font(font),
    _text(""), _textSize(0)
{
    _color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


Label::~Label()
{

}


bool Label::buildTextData()
{
    if (_font == NULL)
        return false;

    float x = 0;
    float y = 0;

    for (int i = 0; i < _text.size(); ++i)
    {
        int charIndex = static_cast<int>(_text[i]);

        x += _font->getCharacterInfo(charIndex).bearingX;

        //_charsTransforms.push_back(glm::translate(glm::vec3(x, y, 0.0f)));
        _charsOffsets.push_back(glm::vec3(x, y, 0.0f));

        x += _font->getCharacterInfo(charIndex).advX - _font->getCharacterInfo(charIndex).bearingX;


        ++_textSize;
    }

    _charsTransforms.resize(_charsOffsets.size());
    _charsTransformsIs.resize(_charsOffsets.size(), false);

    return true;
}


void Label::clearTextData()
{
    _charsOffsets.clear();
    _charsTransforms.clear();
    _charsTransformsIs.clear();

    _textSize = 0;
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

    for (int i = 0; i < _textSize; ++i)
    {
        int charIndex = static_cast<int>(_text[i]);
        renderList->push_back(GUIRenderListElement(vbo, charIndex * 4,
                                                   getVerticesTransformMatrix(i), getTexCoordTransformMatrix(i),
                                                   _color, _font->getTexture(),//s[charIndex],
                                                   GUI_LABEL_SHADER));
    }
}


/*glm::mat4& getTexCoordTransformMatrix(unsigned int vboIndex)
{

}*/
