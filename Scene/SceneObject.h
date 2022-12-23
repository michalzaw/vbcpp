#ifndef SCENEOBJECT_H_INCLUDED
#define SCENEOBJECT_H_INCLUDED


#include <vector>
#include <list>
#include <string>

#include "Component.h"

#include "../Graphics/RotationMode.h"

#include "../Utils/RObject.h"


class SceneManager;


enum SceneObjectFlags
{
    SOF_NONE                        = 0,
    SOF_NOT_SELECTABLE              = 1 << 0,       // Object is not selectable and is not visible in SceneGraphWindow
    SOF_NOT_SELECTABLE_ON_SCENE     = 1 << 1,       // Object is not selectable on scene
    SOF_NOT_SERIALIZABLE            = 1 << 2        // Object is not saved in SceneSaver
};


class SceneObject
{
    private:
        SceneObject*            _parent;
        std::list<SceneObject*> _childrens;

        std::vector<Component*> _components;

        std::string     _name;
        unsigned int    _id;
        bool            _isActive;

        unsigned int    _flags;

		RObject*		_objectDefinition;

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
        SceneObject(const std::string& name, SceneManager* sceneManager, RObject* objectDefinition = nullptr, SceneObject* parent = nullptr);
        ~SceneObject();


        void removeParent();
        bool hasParent();
        SceneObject* getParent();

        void addChild(SceneObject* child);
        bool removeChild(SceneObject* child);
        void removeAllChildren();
        void removeAllChildrenFromScene();
        const std::list<SceneObject*>& getChildren();


        void addComponent(Component* component);
        void removeComponent(Component* component);


		void			    setName(const std::string& name);
        void                setIsActive(bool is);

        const std::string&  getName();
        unsigned int        getId();
        bool                isActive();

        void                setFlags(unsigned int flags);
        void                addFlag(SceneObjectFlags flag);
        unsigned int        getFlags();

		RObject*		    getObjectDefinition();

        Component*          getComponent(unsigned int index);
        Component*          getComponent(ComponentType type);

        unsigned int        getComponentsCount();


        SceneManager* getSceneManager();


        void setPosition(const glm::vec3& position);                               // Set in local space
        void setPosition(float x, float y, float z);
        void setRotation(const glm::vec3& rotation);                               // Rotations in radians
        void setRotation(float x, float y, float z);
        void setRotationQuaternion(const glm::quat& rotation);
        void setRotationQuaternion(float x, float y, float z, float w);
        void setScale(const glm::vec3& scale);
        void setScale(float x, float y, float z);
        void setScale(float scale);

        void move(const glm::vec3& deltaPosition);
        void move(float dx, float dy, float dz);
        void rotate(const glm::vec3& deltaRotation);
        void rotate(float dx, float dy, float dz);
        void scale(const glm::vec3& scale);
        void scale(float x, float y, float z);

        const glm::vec3& getPosition() const;                                      // In local space
        const glm::vec3& getRotation() const;
        const glm::quat& getRotationQuaternion() const;
        const glm::vec3& getScale() const;

        const glm::mat4& getLocalTransformMatrix() const;
        const glm::mat4& getLocalNormalMatrix() const;
        const glm::mat4& getGlobalTransformMatrix() const;
        const glm::mat4& getGlobalNormalMatrix() const;

        void setTransformFromMatrix(const glm::mat4& transformMatrix, bool forceQuaternions = true);

        glm::vec3 getGlobalPosition();
        glm::vec3 transformLocalPointToGlobal(const glm::vec3& point);
        glm::vec3 transformLocalVectorToGlobal(const glm::vec3& vec);

};


#endif // SCENEOBJECT_H_INCLUDED
