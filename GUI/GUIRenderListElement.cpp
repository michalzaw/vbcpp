#include "GUIRenderListElement.h"


GUIRenderListElement::GUIRenderListElement(VBO* vbo, unsigned int firstVertex,
                                           glm::mat4 verticesTransformMatrix, glm::mat4 texCoordTransformMatrix,
                                           glm::vec4 color, RTexture* texture,
                                           ShaderType shaderType)
    : _vbo(vbo), _firstVertex(firstVertex),
    _verticesTransformMatrix(verticesTransformMatrix), _texCoordTransformMatrix(texCoordTransformMatrix),
    _color(color), _texture(texture),
    _shaderType(shaderType)
{

}


GUIRenderListElement::~GUIRenderListElement()
{

}
