#ifndef OGLDRIVER_H_INCLUDED
#define OGLDRIVER_H_INCLUDED


#include <vector>

#include <GLEW/glew.h>

#include "Shader.h"
#include "LoadShader.h"
#include "VAO.h"


class OGLDriver
{
    private:
        std::vector<Shader*> _shaderList;

        VAO* _defaultVAO;

        GLuint VertexArrayID;

    public:
        OGLDriver();
        ~OGLDriver();

        bool Initialize();

        Shader* GetShader(ShaderType type);

};


#endif // OGLDRIVER_H_INCLUDED
