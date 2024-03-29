#include "OGLDriver.h"

#include "../Utils/Logger.h"
#include "../Utils/Strings.h"


static std::unique_ptr<OGLDriver> drvInstance;

OGLDriver::OGLDriver()
{
    //_defaultVAO = NULL;

    vbos = new std::vector<VBO*>[23];
    ibos = new std::vector<IBO*>[23];
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

    for (int i = 0; i < _ssboList.size(); ++i)
    {
        delete _ssboList[i];
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


void OGLDriver::debugOutputCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    {
        return;
    }

    std::string sourceStr;
    std::string typeStr;
    std::string severityStr;
    
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             sourceStr = "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           sourceStr = "Source: Other"; break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               typeStr = "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              typeStr = "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               typeStr = "Type: Other"; break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         severityStr = "HIGH"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "MEDIUM"; break;
        case GL_DEBUG_SEVERITY_LOW:          severityStr = "LOW"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "NOTIFICATION"; break;
    }

    LOG_INFO("[OpenGL][" + severityStr + " | " + sourceStr + " | " + typeStr + "](id=" + Strings::toString(id) + ") " + message);
}


bool OGLDriver::isDebugContextEnabled()
{
    int contextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);

    return contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT;
}


void OGLDriver::initializeDebugContext()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugOutputCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}


bool OGLDriver::initialize()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        return false;

    if (createVAO() == NULL)
        return false;


    if (isDebugContextEnabled())
    {
        LOG_INFO("OpenGL debug context: on");
        initializeDebugContext();
    }
    else
    {
        LOG_INFO("OpenGL debug context: off");
    }


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);

    _defaultFramebuffer = new Framebuffer();
    _defaultFramebuffer->_isCreated = true;
    _defaultFramebuffer->_fboBuffs.push_back(GL_BACK_LEFT);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	LOG_INFO("OpenGL initalized!");

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &_maxAnisotropy);
	LOG_INFO("- maxAnisotropy: " + Strings::toString(_maxAnisotropy));

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(PRIMITIVE_RESTART_INDEX);

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


VBO* OGLDriver::createVBO(unsigned int size, GLenum usage)
{
    VBO* vbo = new VBO(size, usage);

    if (vbo != NULL)
    {
        _vboList.push_back(vbo);
    }

    return vbo;
}


IBO* OGLDriver::createIBO(unsigned int size, GLenum usage)
{
    IBO* ibo = new IBO(size, usage);

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


ShaderStorageBuffer* OGLDriver::createShaderStorageBuffesr(unsigned int size)
{
    ShaderStorageBuffer* ssbo = new ShaderStorageBuffer(size);
    _ssboList.push_back(ssbo);

    return ssbo;
}


Framebuffer* OGLDriver::createFramebuffer()
{
    Framebuffer* framebuffer = new Framebuffer;

    _framebufferList.push_back(framebuffer);

    return framebuffer;
}


// inicjalizacja odbywa sie na glownym watku
void OGLDriver::registerFramebufferForInitialization(Framebuffer* framebuffer)
{
    glFinish();

    _uninitializedFramebuffersMutex.lock();
    _uninitializedFramebuffers.push_back(framebuffer);
    _uninitializedFramebuffersMutex.unlock();
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


void OGLDriver::deleteShaderStorageBuffer(ShaderStorageBuffer* ssbo)
{
    for (int i = 0; i < _ssboList.size(); ++i)
    {
        if (_ssboList[i] == ssbo)
        {
            delete _ssboList[i];
            _ssboList.erase(_ssboList.begin() + i);

            break;
        }
    }
}


void OGLDriver::deleteFramebuffer(Framebuffer* framebuffer)
{
    _uninitializedFramebuffersMutex.lock();
    for (int i = 0; i < _uninitializedFramebuffers.size(); ++i)
    {
        if (_uninitializedFramebuffers[i] == framebuffer)
        {
            _uninitializedFramebuffers.erase(_uninitializedFramebuffers.begin() + i);

            break;
        }
    }
    _uninitializedFramebuffersMutex.unlock();

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


float OGLDriver::getMaxAnisotropy()
{
	return _maxAnisotropy;
}


void OGLDriver::update()
{
    _uninitializedFramebuffersMutex.lock();
    for (Framebuffer* framebuffer : _uninitializedFramebuffers)
    {
        if (!framebuffer->_isInitialized)
        {
            framebuffer->init();
        }
    }

    _uninitializedFramebuffers.clear();

    _uninitializedFramebuffersMutex.unlock();
}
