#ifndef OGLDRIVER_H_INCLUDED
#define OGLDRIVER_H_INCLUDED


#include <vector>

#include <GLEW/glew.h>

#include "RShader.h"
#include "LoadShader.h"
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "UBO.h"

#include <memory>


class OGLDriver
{
    private:
        std::vector<RShader*> _shaderList;

        std::vector<VAO*> _vaoList;
        std::vector<VBO*> _vboList;
        std::vector<IBO*> _iboList;
        std::vector<UBO*> _uboList;

        OGLDriver();

        //VAO* _defaultVAO;

        //GLuint VertexArrayID;

    public:

        ~OGLDriver();

        static OGLDriver& getInstance();

        bool Initialize();

        RShader* GetShader(ShaderType type);

        VAO* CreateVAO();
        VBO* CreateVBO(unsigned int size);
        IBO* CreateIBO(unsigned int size);
        UBO* CreateUBO(unsigned int size);

        void DeleteVAO(VAO* vao);
        void DeleteVBO(VBO* vbo);
        void DeleteIBO(IBO* ibo);
        void DeleteUBO(UBO* ubo);

};


#endif // OGLDRIVER_H_INCLUDED
