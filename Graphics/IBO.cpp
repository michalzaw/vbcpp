#include "IBO.h"


IBO::IBO()
    : _iboId(0), _bufferSize(0), _indicesCount(0)
{

}


IBO::~IBO()
{
    if (_iboId != 0)
        glDeleteBuffers(1, &_iboId);
}


unsigned int IBO::GetBufferSize()
{
    return _bufferSize;
}


unsigned int IBO::GetIndicesCount()
{
    return _indicesCount;
}


bool IBO::Create(unsigned int size)
{
    if (_iboId == 0)
    {
        _bufferSize = size;

        glGenBuffers(1, &_iboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

        return true;
    }

    return false;
}


bool IBO::Create(unsigned int* indices, unsigned int indicesCount)
{
    if (_iboId == 0)
    {
        _indicesCount = indicesCount;
        _bufferSize = _indicesCount * sizeof(unsigned int);

        glGenBuffers(1, &_iboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _bufferSize, indices, GL_STATIC_DRAW);

        return true;
    }

    return false;
}


unsigned int IBO::AddIndices(unsigned int* indices, unsigned int indicesCount)
{
    if (_iboId != 0 && ((_indicesCount + indicesCount) * sizeof(unsigned int)) <= _bufferSize)
    {
        Bind();
        unsigned int offset = _indicesCount * sizeof(unsigned int);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, indicesCount* sizeof(unsigned int), indices);

        _indicesCount += indicesCount;

        return offset;
    }

    return -1;
}
