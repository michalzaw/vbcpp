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


Label* GUIManager::addLabel(RFont* font, std::string text)
{
    Label* label = new Label(font);
    label->setText(text);

    _objects.push_back(label);

    return label;
}


Button* GUIManager::addButton(RTexture* texture, RFont* font, std::string text)
{
    Button* button = new Button(texture, font, text);

    _objects.push_back(button);

    return button;
}


ProgressBar* GUIManager::addProgressBar(bool indeterminate)
{
    ProgressBar* progressBar = new ProgressBar(indeterminate);

    _objects.push_back(progressBar);

    return progressBar;
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


void GUIManager::update(float deltaTime)
{
    for (GUIObject* guiObject : _objects)
    {
        guiObject->update(deltaTime);
    }
}


/*std::list<GUIObject*>* GUIManager::getGUIList()
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
}*/



GUIRenderList* GUIManager::getGUIRenderList()
{
    GUIRenderList* list = new GUIRenderList;

    for (std::vector<GUIObject*>::iterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        if ((*i)->isActive())
        {
            (*i)->addDataToRenderList(list);
        }
    }

    return list;
}
