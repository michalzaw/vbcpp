#ifndef OGLDRIVER_H_INCLUDED
#define OGLDRIVER_H_INCLUDED


#include <vector>

#include <GLEW/glew.h>

#include "Shader.h"
#include "LoadShader.h"
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"

#include <memory>

class OGLDriver
{
    private:
        //std::vector<Shader*> _shaderList;
        //std::vector<std::shared_ptr<Shader>> _shaderPtrList;
        std::vector<std::unique_ptr<Shader>> _shaderPtrList;

        std::vector<VAO*> _vaoList;
        std::vector<VBO*> _vboList;
        std::vector<IBO*> _iboList;

        //VAO* _defaultVAO;

        //GLuint VertexArrayID;

    public:
        OGLDriver();
        ~OGLDriver();

        bool Initialize();

        Shader* GetShader(ShaderType type);

        //std::shared_ptr<Shader>& GetShader(ShaderType type);

        VAO* CreateVAO();
        VBO* CreateVBO(unsigned int size);
        IBO* CreateIBO(unsigned int size);

        void DeleteVAO(VAO* vao);
        void DeleteVBO(VBO* vbo);
        void DeleteIBO(IBO* ibo);

};


#endif // OGLDRIVER_H_INCLUDED
