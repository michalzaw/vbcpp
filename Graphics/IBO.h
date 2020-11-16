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
        IBO(unsigned int size, GLenum usage = GL_STATIC_DRAW);
        ~IBO();

        unsigned int getBufferSize();
        unsigned int getIndicesCount();

        //bool create(unsigned int size);
        //bool create(unsigned int* indices, unsigned int indicesCount);

        unsigned int addIndices(unsigned int* indices, unsigned int indicesCount);
		unsigned int updateIndices(unsigned int* indices, unsigned int indicesCount, unsigned int offset = 0);

        inline void bind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
        }

};


#endif // BUFFEROBJECT_H_INCLUDED
