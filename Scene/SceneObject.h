#ifndef SCENEOBJECT_H_INCLUDED
#define SCENEOBJECT_H_INCLUDED


#include <vector>
#include <list>
#include <string>

#include "Component.h"

#include "../Graphics/RotationMode.h"


class SceneManager;


class SceneObject
{
    private:
        SceneObject*            _parent;
        std::list<SceneObject*> _childrens;

        std::vector<Component*> _components;

        std::string     _name;
        unsigned int    _id;
        bool            _isActive;

        SceneManager*   _sceneManager;


        RotationMode _rotationMode;

        glm::vec3 _position;
        glm::vec3 _rotation;
        glm::quat _rotationQuaternion;
        glm::vec3 _scale;

        mutable glm::mat4 _localTransformMatrix;            mutable bool _localTransformMatrixIsCalculated;
        mutable glm::mat4 _localNormalMatrix;               mutable bool _localNormalMatrixIsCalculated;
        mutable glm::mat4 _globalTransformMatrix;           mutable bool _globalTransformMatrixIsCalculated;
        mutable glm::mat4 _globalNormalMatrix;              mutable bool _globalNormalMatrixIsCalculated;


        void changedTransform();
        void calculateLocalTransformMatrix() const;
        void calculateGlobalTransformMatrix() const;

    public:
        SceneObject(std::string name, SceneManager* sceneManager, SceneObject* parent = NULL);
        ~SceneObject();


        void removeParent();
        bool hasParent();
        SceneObject* getParent();

        void addChild(SceneObject* child);
        bool removeChild(SceneObject* child);
        void removeAllChildren();
        std::list<SceneObject*>& getChildren();


        void addComponent(Component* component);
        void removeComponent(Component* component);


        void            setIsActive(bool is);

        std::string     getName();
        unsigned int    getId();
        bool            isActive();

        Component*      getComponent(unsigned int index);
        Component*      getComponent(ComponentType type);

        unsigned int    getComponentsCount();


        SceneManager* getSceneManager();


        void setPosition(glm::vec3 position);                               // Set in local space
        void setPosition(float x, float y, float z);
        void setRotation(glm::vec3 rotation);                               // Rotations in radians
        void setRotation(float x, float y, float z);
        void setRotationQuaternion(glm::quat rotation);
        void setRotationQuaternion(float x, float y, float z, float w);
        void setScale(glm::vec3 scale);
        void setScale(float x, float y, float z);
        void setScale(float scale);

        void move(glm::vec3 deltaPosition);
        void move(float dx, float dy, float dz);
        void rotate(glm::vec3 deltaRotation);
        void rotate(float dx, float dy, float dz);
        void scale(glm::vec3 scale);
        void scale(float x, float y, float z);

        glm::vec3 getPosition() const;                                      // In local space
        glm::vec3 getRotation() const;
        glm::quat getRotationQuaternion() const;
        glm::vec3 getScale() const;

        glm::mat4& getLocalTransformMatrix() const;
        glm::mat4& getLocalNormalMatrix() const;
        glm::mat4& getGlobalTransformMatrix() const;
        glm::mat4& getGlobalNormalMatrix() const;

        glm::vec3 transformLocalPointToGlobal(glm::vec3 point)
        {
            glm::vec4 p = getGlobalTransformMatrix() * glm::vec4(point.x, point.y, point.z, 1.0f);
            return glm::vec3(p.x, p.y, p.z);
        }
        glm::vec3 transformLocalVectorToGlobal(glm::vec3 vec)
        {
            glm::vec4 v = getGlobalNormalMatrix() * glm::vec4(vec.x, vec.y, vec.z, 0.0f);
            return glm::vec3(v.x, v.y, v.z);
        }

};


#endif // SCENEOBJECT_H_INCLUDED
