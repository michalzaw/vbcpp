#ifndef OGLDRIVER_H_INCLUDED
#define OGLDRIVER_H_INCLUDED


#include <vector>

#include <GLEW/glew.h>

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

    private:
        //std::vector<RShader*> _shaderList;

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

        OGLDriver();

    public:

        ~OGLDriver();

        static OGLDriver& getInstance();

        bool initialize();

        VAO* createVAO();
        VBO* createVBO(unsigned int size);
        IBO* createIBO(unsigned int size);
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

};


#endif // OGLDRIVER_H_INCLUDED
