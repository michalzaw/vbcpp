#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED


#include <string.h>

#include "OGLDriver.h"
#include "RenderData.h"

#include "../Utils/Strings.h"
#include "RShader.h"


class Renderer
{
    private:
        OGLDriver* _OGLDriver;
        UBO* _lightUBO;

    public:
        Renderer(/* OGLDriver* driver */);
        ~Renderer();

        void Render(RenderData* renderData);

};


#endif // RENDERER_H_INCLUDED
