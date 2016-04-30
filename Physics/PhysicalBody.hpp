#ifndef PHYSICALBODY_HPP_INCLUDED
#define PHYSICALBODY_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

class PhysicalBody
{
    public:
        PhysicalBody(btScalar m, btVector3 pos);
        virtual ~PhysicalBody();

        btRigidBody* getRigidBody() { return _rigidBody; }
        btDefaultMotionState* getMotionState() { return _motionState; }
        btCollisionShape*   getCollisionShape() { return _collShape; }

        btScalar getMass() { return _mass; }
        void getTransform(btTransform& t);

        void setRestitution(btScalar rest) { _rigidBody->setRestitution(rest); }

    protected:
        btRigidBody*            _rigidBody;
        btCollisionShape*       _collShape;
        btDefaultMotionState*   _motionState;
        btScalar                _mass;
        btVector3               _position;

        virtual void updateBody() { }
};

#endif // PHYSICALBODY_HPP_INCLUDED
