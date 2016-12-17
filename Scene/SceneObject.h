#ifndef SCENEOBJECT_H_INCLUDED
#define SCENEOBJECT_H_INCLUDED


#include <vector>
#include <list>
#include <string>

#include "Component.h"


class SceneManager;


class SceneObject
{
    private:
        SceneObject*            _parent;
        std::list<SceneObject*> _childrens;

        std::vector<Component*> _components;

        std::string     _name;
        unsigned int    _id;
        Transform       _transform;
        Transform       _globalTransform;
        bool            _isActive;

        bool            _transformIsChanged;

        SceneManager*   _sceneManager;


        void calculateGlobalTransform();

    public:
        SceneObject(std::string name, SceneManager* sceneManager, SceneObject* parent = NULL);
        ~SceneObject();


        void removeParent();
        bool hasParent();
        SceneObject* getParent();

        void addChild(SceneObject* child);
        bool removeChild(SceneObject* child);
        void removeAllChildren();


        void addComponent(Component* component);
        void removeComponent(Component* component);

        void            setIsActive(bool is);

        std::string     getName();
        unsigned int    getId();
        Transform*      getTransform();
        Transform*      getGlobalTransform();
        bool            isActive();

        Component*      getComponent(unsigned int index);
        unsigned int    getComponentsCount();

        SceneManager* getSceneManager();


        void changedTransform();

};


#endif // SCENEOBJECT_H_INCLUDED
