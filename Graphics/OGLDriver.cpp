#include "OGLDriver.h"


OGLDriver::OGLDriver()
{
    _defaultVAO = NULL;
}


OGLDriver::~OGLDriver()
{
    for (int i = 0; i < _shaderList.size(); ++i)
    {
        delete _shaderList[i];
    }

    //glDeleteVertexArrays(1, &VertexArrayID);

    delete _defaultVAO;
}


bool OGLDriver::Initialize()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        return false;

    /*
   	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
    */

    _defaultVAO = new VAO;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);


    //Shader* shader = new Shader(LoadShader("shader.vert", "shader.frag"));
    Shader* shader = new Shader(LoadShader("DirLight.vert", "DirLight.frag"));

    _shaderList.push_back(shader);
}


Shader* OGLDriver::GetShader(ShaderType type)
{
    return _shaderList[type];
}
