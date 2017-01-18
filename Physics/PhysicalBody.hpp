#ifndef PHYSICALBODY_HPP_INCLUDED
#define PHYSICALBODY_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "../Scene/Component.h"
#include "../Scene/SceneObject.h"

#include <memory>

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
        PhysicalBody(btScalar m, btVector3 pos);
        virtual ~PhysicalBody();

        btRigidBody* getRigidBody() { return _rigidBody.get(); }
        btDefaultMotionState* getMotionState() { return _motionState.get(); }
        btCollisionShape*   getCollisionShape() { return _collShape.get(); }

        btScalar getMass() { return _mass; }
        //void getTransform(btTransform& t);

        void setRestitution(btScalar rest) { _rigidBody->setRestitution(rest); }

        void update();

        /*
        void changedTransform(glm::vec3 position, glm::vec3 rotation)
        {
            btVector3 btPos(position.x, position.y, position.z);

            btTransform transf;
            transf.setOrigin(btPos);


            btQuaternion quat(rotation.x, rotation.y, rotation.z, 1);
            transf.setRotation(quat);


        }*/

    protected:
        std::unique_ptr<btRigidBody>            _rigidBody;
        std::unique_ptr<btCollisionShape>       _collShape;
        std::unique_ptr<btDefaultMotionState>   _motionState;
        btScalar                _mass;
        btVector3               _position;

        virtual void updateBody() { }
};

#endif // PHYSICALBODY_HPP_INCLUDED
