#ifndef LOADSHADER_H_INCLUDED
#define LOADSHADER_H_INCLUDED


#include <vector>
#include <fstream>
#include <iostream>

#include <GL/glew.h>


GLuint LoadShader(const char* VertexShaderFileName, const char* FragmentShaderFileName);


#endif // LOADSHADER_H_INCLUDED
