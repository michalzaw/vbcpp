#ifndef GUIMANAGER_H_INCLUDED
#define GUIMANAGER_H_INCLUDED


#include <vector>
#include <list>
#include <string>

#include "GUIObject.h"
#include "Image.h"
#include "Label.h"
#include "GUIRenderListElement.h"


class GUIManager
{
    private:
        std::vector<GUIObject*> _objects;

    public:
        GUIManager();
        ~GUIManager();

        Image* addImage(RTexture* texture);
        Label* addLabel(RFont* font, std::string text = "");

        unsigned int getObjectsCount();

        GUIObject* getObject(unsigned int index);

        //void renderAllObjects();
        //std::list<GUIObject*>* getGUIList();
        GUIRenderList* getGUIRenderList();

};


#endif // GUIMANAGER_H_INCLUDED
