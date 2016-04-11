#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

// GLFW, GLEW
#include <GLEW/glew.h>

// Include GLU header with GLFW
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <string>
using std::string;

class Window
{
    public:
        Window();
        ~Window();

        bool createWindow(int w, int h, int posx, int posy);
        GLFWwindow* getWindow() { return _win; }

        void setWindowTitle(string title);
        void setWindowSize(int w, int h);

        void swapBuffers();
        void updateEvents();
        bool isOpened();
        void setCloseFlag();

    private:
        GLFWwindow* _win;
        int         _width, _height;
        int         _xPos, _yPos;
        string      _title;

        static void errorCallback(int error, const char* description);
        bool initGLEW();
};

#endif // WINDOW_H_INCLUDED
