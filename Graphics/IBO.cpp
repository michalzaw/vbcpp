#include "IBO.h"

#include <cstddef>


IBO::IBO(unsigned int size, GLenum usage)
    : _iboId(0), _bufferSize(size), _indicesCount(0)
{
    glGenBuffers(1, &_iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, usage);
}


IBO::~IBO()
{
    if (_iboId != 0)
        glDeleteBuffers(1, &_iboId);
}


unsigned int IBO::getBufferSize()
{
    return _bufferSize;
}


unsigned int IBO::getIndicesCount()
{
    return _indicesCount;
}


/*bool IBO::create(unsigned int size)
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
}*/


/*bool IBO::create(unsigned int* indices, unsigned int indicesCount)
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
}*/


unsigned int IBO::addIndices(unsigned int* indices, unsigned int indicesCount)
{
    if (_iboId != 0 && ((_indicesCount + indicesCount) * sizeof(unsigned int)) <= _bufferSize)
    {
        bind();
        unsigned int offset = _indicesCount * sizeof(unsigned int);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, indicesCount* sizeof(unsigned int), indices);

        _indicesCount += indicesCount;

        return offset;
    }

    return -1;
}


unsigned int IBO::updateIndices(unsigned int* indices, unsigned int indicesCount, unsigned int offset)
{
	if (_iboId != 0 && ((offset + indicesCount) * sizeof(unsigned int)) <= _bufferSize)
	{
		bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, indicesCount * sizeof(unsigned int), indices);

		if (indicesCount + offset > _indicesCount)
		{
			_indicesCount = indicesCount + offset;
		}

		return offset;
	}

	return -1;
}
