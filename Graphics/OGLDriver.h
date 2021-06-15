#ifndef OGLDRIVER_H_INCLUDED
#define OGLDRIVER_H_INCLUDED


#include <vector>

#include <GL/glew.h>

//#include "RShader.h"
#include "LoadShader.h"
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "UBO.h"
#include "Framebuffer.h"

#include <memory>


class OGLDriver
{
    friend class VAO;
    friend class VBO;
    friend class IBO;
    friend class UBO;

    public:
        //std::vector<RShader*> _shaderList;

        static const int VBO_SIZE = 12582912;       // 12MB = 12 * 1024 * 1024B

        std::vector<VAO*> _vaoList;
        std::vector<VBO*> _vboList;
        std::vector<IBO*> _iboList;
        std::vector<UBO*> _uboList;
        std::vector<Framebuffer*> _framebufferList;

        VAO* _currentVAO;
        VBO* _currentVBO;
        IBO* _currentIBO;
        UBO* _currentUBO;

        Framebuffer* _defaultFramebuffer;
        std::vector<Framebuffer*> _uninitializedFramebuffers;

		float _maxAnisotropy;

        OGLDriver();

    public:

        ~OGLDriver();

        static OGLDriver& getInstance();

        bool initialize();

        VAO* createVAO();
        VBO* createVBO(unsigned int size, GLenum usage = GL_STATIC_DRAW);
        IBO* createIBO(unsigned int size, GLenum usage = GL_STATIC_DRAW);
        UBO* createUBO(unsigned int size);
        Framebuffer* createFramebuffer();

        void deleteVAO(VAO* vao);
        void deleteVBO(VBO* vbo);
        void deleteIBO(IBO* ibo);
        void deleteUBO(UBO* ubo);
        void deleteFramebuffer(Framebuffer* framebuffer);

        VAO* getCurrentVAO();
        VBO* getCurrentVBO();
        IBO* getCurrentIBO();
        UBO* getCurrentUBO();

        Framebuffer* getDefaultFramebuffer();

		float getMaxAnisotropy();

        std::vector<VBO*>* vbos;
        std::vector<IBO*>* ibos;

        int getVboIndexForVertices(ShaderType shaderType, unsigned int verticesCoutnt, unsigned int vertexSize)
        {
            int vectorSize = vbos[shaderType].size();
            if (vectorSize == 0 ||
                (vbos[shaderType][vectorSize - 1]->getQuantumOfVertices() + verticesCoutnt) * vbos[shaderType][vectorSize - 1]->getVertexSize() > vbos[shaderType][vectorSize - 1]->getBufferSize())
            {
                int verticesSize = verticesCoutnt * vertexSize;
                vbos[shaderType].push_back(createVBO(verticesSize <= VBO_SIZE ? VBO_SIZE : verticesSize));
            }

            return vbos[shaderType].size() - 1;
        }

        int getIboIndexForIndices(ShaderType shaderType, unsigned int indicesCoutnt)
        {
            int vectorSize = ibos[shaderType].size();
            if (vectorSize == 0 ||
                (ibos[shaderType][vectorSize - 1]->getIndicesCount() + indicesCoutnt) * sizeof(unsigned int) > ibos[shaderType][vectorSize - 1]->getBufferSize())
            {
                int indicesSize = indicesCoutnt * sizeof(unsigned int);
                ibos[shaderType].push_back(createIBO(indicesSize <= VBO_SIZE ? VBO_SIZE : indicesSize));
            }

            return ibos[shaderType].size() - 1;
        }

        void update();

};


#endif // OGLDRIVER_H_INCLUDED
