#include "OGLDriver.h"


OGLDriver::OGLDriver()
{
    //_defaultVAO = NULL;
}


OGLDriver::~OGLDriver()
{
    for (int i = 0; i < _shaderList.size(); ++i)
    {
        delete _shaderList[i];
    }

    for (int i = 0; i < _vaoList.size(); ++i)
    {
        delete _vaoList[i];
    }

    //glDeleteVertexArrays(1, &VertexArrayID);

    //delete _defaultVAO;
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

    if (CreateVAO() == NULL)
        return false;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);


    //Shader* shader = new Shader(LoadShader("shader.vert", "shader.frag"));
    Shader* shader = new Shader(LoadShader("DirLight.vert", "DirLight.frag"));
    _shaderList.push_back(shader);

    shader = new Shader(LoadShader("DirLight.vert", "DirLight_notexture.frag"));
    _shaderList.push_back(shader);

    shader = new Shader(LoadShader("normalmapping.vert", "normalmapping.frag"));
    _shaderList.push_back(shader);
}


Shader* OGLDriver::GetShader(ShaderType type)
{
    return _shaderList[type];
}


VAO* OGLDriver::CreateVAO()
{
    VAO* vao = new VAO;

    if (vao != NULL)
    {
        _vaoList.push_back(vao);
    }

    return vao;
}


VBO* OGLDriver::CreateVBO(unsigned int size)
{
    VBO* vbo = new VBO(size);

    if (vbo != NULL)
    {
        _vboList.push_back(vbo);
    }

    return vbo;
}


IBO* OGLDriver::CreateIBO(unsigned int size)
{
    IBO* ibo = new IBO(size);

    if (ibo != NULL)
    {
        _iboList.push_back(ibo);
    }

    return ibo;
}


void OGLDriver::DeleteVAO(VAO* vao)
{
    for (int i = 0; i < _vaoList.size(); ++i)
    {
        if (_vaoList[i] == vao)
        {
            delete _vaoList[i];
            _vaoList.erase(_vaoList.begin() + i);

            break;
        }
    }
}


void OGLDriver::DeleteVBO(VBO* vbo)
{
    for (int i = 0; i < _vboList.size(); ++i)
    {
        if (_vboList[i] == vbo)
        {
            delete _vboList[i];
            _vboList.erase(_vboList.begin() + i);

            break;
        }
    }
}


void OGLDriver::DeleteIBO(IBO* ibo)
{
    for (int i = 0; i < _iboList.size(); ++i)
    {
        if (_iboList[i] == ibo)
        {
            delete _iboList[i];
            _iboList.erase(_iboList.begin() + i);

            break;
        }
    }
}
