#ifndef LOADSHADER_H_INCLUDED
#define LOADSHADER_H_INCLUDED


#include <vector>
#include <fstream>
#include <iostream>

#include <GLEW/glew.h>


//GLuint loadShader(const char* VertexShaderFileName, const char* FragmentShaderFileName);


class ShaderLoader
{
    private:
        enum ShaderType
        {
            ST_VERTEX_SHADER    = GL_VERTEX_SHADER,
            ST_GEOMETRY_SHADER  = GL_GEOMETRY_SHADER,
            ST_FRAGMENT_SHADER  = GL_FRAGMENT_SHADER

        };

        static bool loadShaderCode(const char* fileName, std::string& code, const std::vector<std::string>& defines);
        static GLuint compileShader(ShaderType type, std::string& code);
        static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

    public:
        static GLuint loadShader(const char* VertexShaderFileName, const char* FragmentShaderFileName, const std::vector<std::string>& defines = std::vector<std::string>());

};


#endif // LOADSHADER_H_INCLUDED
