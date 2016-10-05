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

        VAO* _currentVAO;
        VBO* _currentVBO;
        IBO* _currentIBO;
        UBO* _currentUBO;

        OGLDriver();

    public:

        ~OGLDriver();

        static OGLDriver& getInstance();

        bool Initialize();

        VAO* CreateVAO();
        VBO* CreateVBO(unsigned int size);
        IBO* CreateIBO(unsigned int size);
        UBO* CreateUBO(unsigned int size);

        void DeleteVAO(VAO* vao);
        void DeleteVBO(VBO* vbo);
        void DeleteIBO(IBO* ibo);
        void DeleteUBO(UBO* ubo);

        VAO* getCurrentVAO();
        VBO* getCurrentVBO();
        IBO* getCurrentIBO();
        UBO* getCurrentUBO();

};


#endif // OGLDRIVER_H_INCLUDED
