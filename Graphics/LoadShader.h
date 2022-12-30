#ifndef LOADSHADER_H_INCLUDED
#define LOADSHADER_H_INCLUDED


#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <GL/glew.h>


//GLuint loadShader(const char* VertexShaderFileName, const char* FragmentShaderFileName);


class ShaderLoader
{
    private:
        enum ShaderType
        {
            ST_VERTEX_SHADER    = GL_VERTEX_SHADER,
            ST_GEOMETRY_SHADER  = GL_GEOMETRY_SHADER,
            ST_FRAGMENT_SHADER  = GL_FRAGMENT_SHADER,
            ST_COMPUTE_SHADER   = GL_COMPUTE_SHADER

        };

        static std::string replaceConstatnsInLine(std::string line, const std::unordered_map<std::string, std::string>& constants);

        static bool loadShaderCode(const char* fileName, std::string& code, const std::vector<std::string>& defines,
                                   const std::unordered_map<std::string, std::string>& constants);
        static GLuint compileShader(ShaderType type, std::string& code);
        static GLuint linkProgram(std::vector<GLuint> shadersIds);

    public:
        static GLuint loadShader(const char* VertexShaderFileName, const char* FragmentShaderFileName, const std::vector<std::string>& defines = {},
                                 const std::unordered_map<std::string, std::string>& constants = {});

        static GLuint loadComputeShader(const char* shaderFileName, const std::vector<std::string>& defines = {},
                                        const std::unordered_map<std::string, std::string>& constants = {});

};


#endif // LOADSHADER_H_INCLUDED
