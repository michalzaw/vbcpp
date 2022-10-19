#include "ProgressBar.h"


ProgressBar::ProgressBar(bool indeterminate)
    : _isInitialized(false),
    _indeterminate(indeterminate), _speed(1.0f), _type(0), _value(0.0f)
{
    createVBO();

    setScale(50.0f, 50.0f);

    _isInitialized = true;
}


ProgressBar::~ProgressBar()
{
    for (std::vector<VBO*>::iterator i = _vbos.begin(); i != _vbos.end(); ++i)
    {
        OGLDriver::getInstance().deleteVBO(*i);
    }
}


void ProgressBar::createVBO()
{
    if (_isInitialized)
        return;

    VBO* vbo = OGLDriver::getInstance().createVBO(12800);

    GUIVertex vertices[4];
    vertices[0].position = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[1].position = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[2].position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertices[3].position = glm::vec3(1.0f, 0.0f, 0.0f);

    vertices[0].texCoord = glm::vec2(0.0f, 1.0f);
    vertices[1].texCoord = glm::vec2(0.0f, 0.0f);
    vertices[2].texCoord = glm::vec2(1.0f, 1.0f);
    vertices[3].texCoord = glm::vec2(1.0f, 0.0f);

    vbo->addVertexData(vertices, 4);

    _vbos.push_back(vbo);
}


void ProgressBar::calculateVerticesTransformMatrix()
{
    _verticesTransformMatrix = glm::translate(glm::vec3(_position.x, _position.y, _position.z)) *
                               glm::rotate(_rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                               glm::scale(glm::vec3(_scale, 1.0f));
}


void ProgressBar::setSize(float size)
{
    setScale(size, size);
}


float ProgressBar::getSize()
{
    return getScale().x;
}


void ProgressBar::setIndeterminate(bool indeterminate)
{
    _indeterminate = indeterminate;
}


bool ProgressBar::isIndeterminate()
{
    return _indeterminate;
}


void ProgressBar::setSpeed(float speed)
{
    _speed = speed;
}


float ProgressBar::getSpeed()
{
    return _speed;
}


void ProgressBar::setType(int type)
{
    _type = type;
}


int ProgressBar::getType()
{
    return _type;
}


void ProgressBar::setValue(float value)
{
    _value = value;
}


float ProgressBar::getValue()
{
    return _value;
}


void ProgressBar::update(float deltaTime)
{
    if (_indeterminate)
    {
        _value += deltaTime * _speed;
    }
}


glm::mat4& ProgressBar::getVerticesTransformMatrix(unsigned int vboIndex)
{
    //  if (vboIndex != 0)
  //        return glm::mat4(1.0f);


    if (!_verticesTransformMatrixIsCalculated)
    {
        calculateVerticesTransformMatrix();

        _verticesTransformMatrixIsCalculated = true;
    }

    return _verticesTransformMatrix;
}


void ProgressBar::addDataToRenderList(GUIRenderList* renderList)
{
    std::vector<std::pair<std::string, float>> customUniforms;
    customUniforms.push_back(std::make_pair("progressBarValue", _value));
    customUniforms.push_back(std::make_pair("progressBarType", static_cast<float>(_type)));

    renderList->push_back(GUIRenderListElement(_vbos[0], 0,
                          getVerticesTransformMatrix(), getTexCoordTransformMatrix(),
                          _color, nullptr,
                          GUI_PROGRESS_BAR_SHADER,
                          customUniforms));
}
