#include "OGLDriver.h"


static std::unique_ptr<OGLDriver> drvInstance;

OGLDriver::OGLDriver()
{
    //_defaultVAO = NULL;
}


OGLDriver::~OGLDriver()
{
    for (int i = 0; i < _vaoList.size(); ++i)
    {
        delete _vaoList[i];
    }

    for (int i = 0; i < _vboList.size(); ++i)
    {
        delete _vboList[i];
    }

    for (int i = 0; i < _iboList.size(); ++i)
    {
        delete _iboList[i];
    }

    for (int i = 0; i < _uboList.size(); ++i)
    {
        delete _uboList[i];
    }

    //_shaderPtrList.clear();
}


OGLDriver& OGLDriver::getInstance()
{
    if( !drvInstance )
        drvInstance = std::unique_ptr<OGLDriver>(new OGLDriver);

    return* drvInstance;
}


bool OGLDriver::Initialize()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        return false;

    if (CreateVAO() == NULL)
        return false;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);

    return true;

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


UBO* OGLDriver::CreateUBO(unsigned int size)
{
    UBO* ubo = new UBO(size);

    if (ubo != NULL)
    {
        _uboList.push_back(ubo);
    }

    return ubo;
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


void OGLDriver::DeleteUBO(UBO* ubo)
{
    for (int i = 0; i < _uboList.size(); ++i)
    {
        if (_uboList[i] == ubo)
        {
            delete _uboList[i];
            _uboList.erase(_uboList.begin() + i);

            break;
        }
    }
}


VAO* OGLDriver::getCurrentVAO()
{
    return _currentVAO;
}


VBO* OGLDriver::getCurrentVBO()
{
    return _currentVBO;
}


IBO* OGLDriver::getCurrentIBO()
{
    return _currentIBO;
}


UBO* OGLDriver::getCurrentUBO()
{
    return _currentUBO;
}
