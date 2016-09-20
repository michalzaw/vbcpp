#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED


#include "OGLDriver.h"
#include "RenderData.h"
#include "RShader.h"


class Renderer
{
    private:
        OGLDriver* _OGLDriver;

    public:
        Renderer(/* OGLDriver* driver */);
        ~Renderer();

        void Render(RenderData* renderData);

};


#endif // RENDERER_H_INCLUDED
