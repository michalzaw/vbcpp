#include "GUIRenderListElement.h"


GUIRenderListElement::GUIRenderListElement(VBO* vbo, unsigned int firstVertex,
                                           glm::mat4 verticesTransformMatrix, glm::mat4 texCoordTransformMatrix,
                                           glm::vec4 color, RTexture* texture,
                                           ShaderType shaderType,
                                           const std::vector<std::pair<std::string, float>>& customUniforms)
    : _vbo(vbo), _firstVertex(firstVertex),
    _verticesTransformMatrix(verticesTransformMatrix), _texCoordTransformMatrix(texCoordTransformMatrix),
    _color(color), _texture(texture),
    _shaderType(shaderType),
    _customUniforms(customUniforms)
{

}


GUIRenderListElement::~GUIRenderListElement()
{

}
