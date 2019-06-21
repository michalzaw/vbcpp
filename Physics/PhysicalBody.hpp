#ifndef PHYSICALBODY_HPP_INCLUDED
#define PHYSICALBODY_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "../Scene/Component.h"
#include "../Scene/SceneObject.h"

#include "../Utils/Math.h"

#include <memory>

class PhysicsManager;
class Constraint;

#define BIT(x) (1<<(x))
enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_TERRAIN = BIT(0), //<Collide with terrain
    COL_BUS = BIT(1), //<Collide with bus chassis
    COL_DOOR = BIT(2), //<Collide with bus door
    COL_WHEEL = BIT(3), // Collide with bus wheel
    COL_ENV = BIT(4)
};

class PhysicalBody : public Component
{
    public:
        PhysicalBody(btScalar m);
        virtual ~PhysicalBody();

        btRigidBody* getRigidBody() { return _rigidBody.get(); }
        btDefaultMotionState* getMotionState() { return _motionState.get(); }
        btCollisionShape*   getCollisionShape() { return _collShape.get(); }

        btScalar getMass() { return _mass; }

        void setRestitution(btScalar rest) { _rigidBody->setRestitution(rest); }

        void addConstraint(Constraint* c)
        {
            _constraints.push_back(c);
        }

        void setDefaultPosition(btVector3 pos)
        {
            _position = pos;
        }

        virtual void update();


        virtual void changedTransform();

    protected:
        btDiscreteDynamicsWorld*                _dynamicsWorld;
        std::unique_ptr<btRigidBody>            _rigidBody;
        std::unique_ptr<btCollisionShape>       _collShape;
        std::unique_ptr<btDefaultMotionState>   _motionState;
        btScalar                _mass;
        btVector3               _position;      // default position
        btVector3               _oldScale;

        std::vector<Constraint*> _constraints;

        bool _isUpdateTransformFromObject;

        virtual void updateBody() { }

        virtual void onAttachedToScenObject()
        {
            if (_rigidBody)
                _rigidBody->setUserPointer(_object);
        }
};

#endif // PHYSICALBODY_HPP_INCLUDED
