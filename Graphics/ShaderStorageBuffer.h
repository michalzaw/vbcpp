#ifndef SHADERSTORAGEBUFFER_H_INCLUDED
#define SHADERSTORAGEBUFFER_H_INCLUDED


#include <GL/glew.h>


class ShaderStorageBuffer final
{
    private:
        GLuint _bufferId;

        unsigned int _bufferSize;

    public:
        ShaderStorageBuffer(unsigned int size)
            : _bufferSize(size)
        {
            glGenBuffers(1, &_bufferId);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _bufferId);
            glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY);
        }

        ~ShaderStorageBuffer()
        {
            if (_bufferId != 0)
            {
                glDeleteBuffers(1, &_bufferId);
            }
        }

        inline void bind()
        {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _bufferId);
        }

        inline void bindBufferBase(unsigned int location)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, _bufferId);
        }

        template <typename DataType>
        bool setData(DataType& data)
        {
            if (sizeof(DataType) != _bufferSize)
            {
                return false;
            }

            bind();
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(DataType), &data);

            return true;
        }

};


#endif // SHADERSTORAGEBUFFER_H_INCLUDED
