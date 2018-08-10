#include "VBO.h"


VBO::VBO(unsigned int size)
    : _vboId(0), _vertexSize(0), _bufferSize(size), _quantumOfVertices(0)
{
    glGenBuffers(1, &_vboId);
    glBindBuffer(GL_ARRAY_BUFFER, _vboId);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
}


VBO::~VBO()
{
    if (_vboId != 0)
        glDeleteBuffers(1, &_vboId);
}


unsigned int VBO::getVertexSize()
{
    return _vertexSize;
}


unsigned int VBO::getBufferSize()
{
    return _bufferSize;
}


unsigned int VBO::getQuantumOfVertices()
{
    return _quantumOfVertices;
}


/*bool VBO::create(unsigned int size)
{
    if (_vboId == 0)
    {
        _bufferSize = size;

        glGenBuffers(1, &_vboId);
        glBindBuffer(GL_ARRAY_BUFFER, _vboId);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

        return true;
    }

    return false;
}*/
