#ifndef IBO_H_INCLUDED
#define IBO_H_INCLUDED


#include <GL/glew.h>


class IBO
{
    private:
        GLuint _iboId;

        unsigned int _bufferSize;
        unsigned int _indicesCount;

    public:
        IBO();
        ~IBO();

        unsigned int GetBufferSize();
        unsigned int GetIndicesCount();

        bool Create(unsigned int size);
        bool Create(unsigned int* indices, unsigned int indicesCount);

        unsigned int AddIndices(unsigned int* indices, unsigned int indicesCount);

        inline void Bind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
        }

};


#endif // BUFFEROBJECT_H_INCLUDED
