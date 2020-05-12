#ifndef VBO_H_INCLUDED
#define VBO_H_INCLUDED


#include <GL/glew.h>

#include "Vertex.h"


class VBO
{
    private:
        GLuint _vboId;

        unsigned short _vertexSize;
        unsigned int _bufferSize;
        unsigned int _quantumOfVertices;

    public:
        VBO(unsigned int size);
        ~VBO();

        unsigned int getVertexSize();
        unsigned int getBufferSize();
        unsigned int getQuantumOfVertices();

        //bool create(unsigned int size);

        /*template <typename VertexType>
        bool Create(VertexType* vertexData, unsigned int quantumOfVertices)
        {
            if (_vboId == 0)
            {
                _vertexSize = sizeof(VertexType);
                _quantumOfVertices = quantumOfVertices;
                _bufferSize = _quantumOfVertices * _vertexSize;

                glGenBuffers(1, &_vboId);
                glBindBuffer(GL_ARRAY_BUFFER, _vboId);
                glBufferData(GL_ARRAY_BUFFER, _bufferSize, vertexData, GL_STATIC_DRAW);

                return true;
            }

            return false;
        }*/

        template <typename VertexType>
        // Return new data offset in buffer or -1
        unsigned int addVertexData(VertexType* vertices, unsigned int quantumOfVertices)
        {
            if (_vertexSize == 0)
                _vertexSize = sizeof(VertexType);

            else if (sizeof(VertexType) != _vertexSize)
                return -1;

            if (_vboId != 0 && ((_quantumOfVertices + quantumOfVertices) * _vertexSize) <= _bufferSize)
            {
                bind();
                unsigned int offset = _quantumOfVertices * _vertexSize;
                glBufferSubData(GL_ARRAY_BUFFER, offset, quantumOfVertices* _vertexSize, vertices);

                _quantumOfVertices += quantumOfVertices;

                return offset;
            }

            return -1;
        }

		template <typename VertexType>
		unsigned int updateVertexData(VertexType* vertices, unsigned int quantumOfVertices, unsigned int offset = 0)
		{
			if (sizeof(VertexType) != _vertexSize)
				return -1;

			if (_vboId != 0 && ((offset + quantumOfVertices) * _vertexSize) <= _bufferSize)
			{
				bind();
				glBufferSubData(GL_ARRAY_BUFFER, offset, quantumOfVertices * _vertexSize, vertices);

				if (quantumOfVertices + offset > _quantumOfVertices)
				{
					_quantumOfVertices = quantumOfVertices + offset;
				}

				return offset;
			}

			return -1;
		}

        inline void bind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, _vboId);
        }

};


#endif // VBO_H_INCLUDED
