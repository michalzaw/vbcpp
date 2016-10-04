#ifndef UBO_H_INCLUDED
#define UBO_H_INCLUDED


#include "RShader.h"


class UBO
{
    private:
        GLuint _uboId;

        unsigned int _bufferSize;

    public:
        UBO(unsigned int size)
            : _bufferSize(size)
        {
            glGenBuffers(1, &_uboId);
            glBindBuffer(GL_UNIFORM_BUFFER, _uboId);
            glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        }

        ~UBO()
        {
            if (_uboId != 0)
                glDeleteBuffers(1, &_uboId);
        }

        unsigned int getBufferSize()
        {
            return _bufferSize;
        }

		inline void setUniform(int offset, int Value)
		{
		    bind();

		    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &Value);
        }
		inline void setUniform(int offset, float Value)
		{
		    bind();

		    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &Value);
        }

        inline void bind()
        {
            glBindBuffer(GL_UNIFORM_BUFFER, _uboId);
        }

        inline void bindBufferBase(unsigned int location)
        {
            glBindBufferBase(GL_UNIFORM_BUFFER, location, _uboId);
        }

};


#endif // UBO_H_INCLUDED
