#include "GUIManager.h"


GUIManager::GUIManager()
{

}


GUIManager::~GUIManager()
{
    for (std::vector<GUIObject*>::iterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        delete *i;
    }
}


Image* GUIManager::addImage(RTexture* texture)
{
    Image* image = new Image(texture);

    _objects.push_back(image);

    return image;
}


unsigned int GUIManager::getObjectsCount()
{
    return _objects.size();
}


GUIObject* GUIManager::getObject(unsigned int index)
{
    if (index < _objects.size())
        return _objects[index];

    return NULL;
}


std::list<GUIObject*>* GUIManager::getGUIList()
{
    std::list<GUIObject*>* list = new std::list<GUIObject*>;

    for (std::vector<GUIObject*>::iterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        if ((*i)->isActive())
        {
            list->push_back(*i);
        }
    }

    return list;
}
