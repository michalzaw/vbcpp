#include "Window.h"

#include <cstdio>

Window::Window()
: _width(1024), _height(768), _xPos(100), _yPos(100), _title("New GLFW window")
{
    glfwSetErrorCallback(errorCallback);
}

Window::~Window()
{
    glfwTerminate();
}


bool Window::createWindow(int w = 1024, int h = 768, int posx = 100, int posy = 100)
{
    _width = w;
    _height = h;
    _xPos = posx;
    _yPos = posy;

    if ( !glfwInit() )
    {
        fprintf( stderr, "VIDEO: Failed to initialize GLFW!\n");
        //exit(EXIT_FAILURE);
        return false;
    }

    // Set OpenGL characteristics
    glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	// Create window with given size and title
	_win = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

	if (!_win)
        return false;

    // Make window active, set position on the screen and title
	glfwMakeContextCurrent(_win);
	glfwSetWindowPos(_win, _xPos, _yPos);

    if (!initGLEW())
        return false;


    return true;
}

// Set window title
void Window::setWindowTitle(string title)
{
    _title = title;
    glfwSetWindowTitle(_win, _title.c_str());
}


// Resize window
void Window::setWindowSize(int w, int h)
{
    _width = w; _height = h;
    glfwSetWindowSize(_win, _width, _height);
}


// Swap buffers
void Window::swapBuffers()
{
    glfwSwapBuffers(_win);
}


// Update window event poll
void Window::updateEvents()
{
    glfwPollEvents();
    //glfwWaitEvents();
}


// Check if window is still opened
bool Window::isOpened()
{
    return !glfwWindowShouldClose(_win);
}

void Window::setCloseFlag()
{
    glfwSetWindowShouldClose(_win, GL_TRUE);
}


// PRIVATE STUFF

// GLFW error callback function
void Window::errorCallback(int error, const char* description)
{
    fputs(description, stderr);
}


// GLEW initialization
bool Window::initGLEW()
{
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    GLenum err = glewInit();
    if( err != GLEW_OK )
    {
        fprintf( stderr, "VIDEO: Failed to initialize GLEW! Error: %s\n", glewGetErrorString(err));
        //exit(EXIT_FAILURE);
        return false;
    }

    // check if OpenGL 3.3 is supported
    if (!GLEW_VERSION_3_3)
        return false;


    return true;
}


