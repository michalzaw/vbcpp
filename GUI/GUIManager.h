#ifndef GUIMANAGER_H_INCLUDED
#define GUIMANAGER_H_INCLUDED


#include <vector>
#include <list>

#include "GUIObject.h"
#include "Image.h"


class GUIManager
{
    private:
        std::vector<GUIObject*> _objects;

    public:
        GUIManager();
        ~GUIManager();

        Image* addImage(RTexture* texture);

        unsigned int getObjectsCount();

        GUIObject* getObject(unsigned int index);

        //void renderAllObjects();
        std::list<GUIObject*>* getGUIList();

};


#endif // GUIMANAGER_H_INCLUDED
