#include "VBO.h"


VBO::VBO()
    : _vboId(0), _vertexSize(0), _bufferSize(0), _quantumOfVertices(0)
{

}


VBO::~VBO()
{
    if (_vboId != 0)
        glDeleteBuffers(1, &_vboId);
}


unsigned int VBO::GetBufferSize()
{
    return _bufferSize;
}


unsigned int VBO::GetQuantumOfVertices()
{
    return _quantumOfVertices;
}


bool VBO::Create(unsigned int size)
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
}
