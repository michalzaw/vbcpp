#ifndef PHYSICALBODY_HPP_INCLUDED
#define PHYSICALBODY_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "../Scene/Component.h"
#include "../Scene/SceneObject.h"

#include <memory>

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

    protected:
        std::unique_ptr<btRigidBody>            _rigidBody;
        std::unique_ptr<btCollisionShape>       _collShape;
        std::unique_ptr<btDefaultMotionState>   _motionState;
        btScalar                _mass;
        btVector3               _position;

        virtual void updateBody() { }
};

#endif // PHYSICALBODY_HPP_INCLUDED
