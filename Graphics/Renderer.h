#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED


#include <string.h>
#include <list>

#include "OGLDriver.h"
#include "RenderData.h"
#include "RShader.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"

#include "../GUI/GUIObject.h"
#include "../GUI/Image.h"


class Renderer
{
    private:
        //OGLDriver* _OGLDriver;
        std::vector<RShader*> _shaderList;
        UBO* _lightUBO;

        unsigned int _screenWidth;
        unsigned int _screenHeight;

    public:
        Renderer(unsigned int screenWidth, unsigned int screenHeight/* OGLDriver* driver */);
        ~Renderer();

        void Render(RenderData* renderData);
        void RenderGUI(std::list<GUIObject*>* GUIObjectsList);

};


#endif // RENDERER_H_INCLUDED
