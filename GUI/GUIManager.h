#ifndef GUIMANAGER_H_INCLUDED
#define GUIMANAGER_H_INCLUDED


#include <vector>
#include <list>
#include <string>

#include "GUIObject.h"
#include "Image.h"
#include "Label.h"
#include "Button.h"
#include "GUIRenderListElement.h"
#include "ProgressBar.h"


class GUIManager
{
    private:
        std::vector<GUIObject*> _objects;

    public:
        GUIManager();
        ~GUIManager();

        Image* addImage(RTexture* texture);
        Label* addLabel(RFont* font, std::string text = "");
        Button* addButton(RTexture* texture, RFont* font = NULL, std::string text = "");
        ProgressBar* addProgressBar(bool indeterminate = true);

        unsigned int getObjectsCount();

        GUIObject* getObject(unsigned int index);

        void update(float deltaTime);

        //void renderAllObjects();
        //std::list<GUIObject*>* getGUIList();
        GUIRenderList* getGUIRenderList();

};


#endif // GUIMANAGER_H_INCLUDED
