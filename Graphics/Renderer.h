#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <string.h>
#include <list>

#include "OGLDriver.h"
#include "RenderData.h"
#include "RShader.h"
#include "GraphicsManager.h"
#include "TreeComponent.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/RefCounter.h"

#include "../GUI/GUIObject.h"
#include "../GUI/Image.h"
#include "../GUI/Label.h"
#include "../GUI/GUIRenderListElement.h"


//#define DRAW_AABB


class Renderer : virtual public RefCounter
{
    private:
        //OGLDriver* _OGLDriver;
        std::vector<RShader*> _shaderList;
        UBO* _lightUBO;

        unsigned int _screenWidth;
        unsigned int _screenHeight;

        bool _isShadowMappingEnable;

    public:
        Renderer(unsigned int screenWidth, unsigned int screenHeight/* OGLDriver* driver */);
        ~Renderer();

        void init();

        void setIsShadowMappingEnable(bool isEnable);
        bool isShadowMappingEnable();

        void renderAll();
        void renderDepth(RenderData* renderData);
        void renderScene(RenderData* renderData);
        void renderGUI(GUIRenderList* renderList);//std::list<GUIObject*>* GUIObjectsList);

};


#endif // RENDERER_H_INCLUDED
