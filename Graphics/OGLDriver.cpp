#include "OGLDriver.h"


static std::unique_ptr<OGLDriver> drvInstance;

OGLDriver::OGLDriver()
{
    //_defaultVAO = NULL;

    vbos = new std::vector<VBO*>[13];
    ibos = new std::vector<IBO*>[13];
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

    for (int i = 0; i < _framebufferList.size(); ++i)
    {
        delete _framebufferList[i];
    }

    delete _defaultFramebuffer;

    delete[] vbos;
    delete[] ibos;

}


OGLDriver& OGLDriver::getInstance()
{
    if( !drvInstance )
        drvInstance = std::unique_ptr<OGLDriver>(new OGLDriver);

    return* drvInstance;
}


bool OGLDriver::initialize()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        return false;

    if (createVAO() == NULL)
        return false;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);

    _defaultFramebuffer = new Framebuffer();
    _defaultFramebuffer->_fboBuffs.push_back(GL_BACK_LEFT);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    return true;
}


VAO* OGLDriver::createVAO()
{
    VAO* vao = new VAO;

    if (vao != NULL)
    {
        _vaoList.push_back(vao);
    }

    return vao;
}


VBO* OGLDriver::createVBO(unsigned int size)
{
    VBO* vbo = new VBO(size);

    if (vbo != NULL)
    {
        _vboList.push_back(vbo);
    }

    return vbo;
}


IBO* OGLDriver::createIBO(unsigned int size)
{
    IBO* ibo = new IBO(size);

    if (ibo != NULL)
    {
        _iboList.push_back(ibo);
    }

    return ibo;
}


UBO* OGLDriver::createUBO(unsigned int size)
{
    UBO* ubo = new UBO(size);

    if (ubo != NULL)
    {
        _uboList.push_back(ubo);
    }

    return ubo;
}


Framebuffer* OGLDriver::createFramebuffer()
{
    Framebuffer* framebuffer = new Framebuffer;
    framebuffer->init();

    _framebufferList.push_back(framebuffer);

    return framebuffer;
}


void OGLDriver::deleteVAO(VAO* vao)
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


void OGLDriver::deleteVBO(VBO* vbo)
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


void OGLDriver::deleteIBO(IBO* ibo)
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


void OGLDriver::deleteUBO(UBO* ubo)
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


void OGLDriver::deleteFramebuffer(Framebuffer* framebuffer)
{
    for (int i = 0; i < _framebufferList.size(); ++i)
    {
        if (_framebufferList[i] == framebuffer)
        {
            delete _framebufferList[i];
            _framebufferList.erase(_framebufferList.begin() + i);

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


Framebuffer* OGLDriver::getDefaultFramebuffer()
{
    return _defaultFramebuffer;
}
