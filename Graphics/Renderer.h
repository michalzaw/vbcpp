#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED


#include <string.h>

#include "OGLDriver.h"
#include "RenderData.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "RShader.h"
#include "../Utils/RefCounter.h"

class Renderer : virtual public RefCounter
{
    private:
        //OGLDriver* _OGLDriver;
        std::vector<RShader*> _shaderList;
        UBO* _lightUBO;

    public:
        Renderer(/* OGLDriver* driver */);
        ~Renderer();

        void Render(RenderData* renderData);

};


#endif // RENDERER_H_INCLUDED
