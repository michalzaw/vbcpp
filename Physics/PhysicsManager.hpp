#ifndef PHYSICSMANAGER_HPP_INCLUDED
#define PHYSICSMANAGER_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicalBodyBox.hpp"
#include "PhysicalBodyCylinder.hpp"
#include "PhysicalBodySphere.hpp"
#include "PhysicalBodyStaticPlane.hpp"

#include "ConstraintHinge.hpp"
#include "ConstraintHinge2.hpp"


// PHYSICS MANAGER

class PhysicsManager
{
    public:
        PhysicsManager();
        ~PhysicsManager();

        int createPhysicsWorld();
        int destroyPhysicsWorld();

        void simulate();

        btDiscreteDynamicsWorld*                getDynamicsWorld() { return _dynamicsWorld; }
        btSequentialImpulseConstraintSolver*    getConstraintSolver() { return _constraintSolver; }
        btCollisionDispatcher*                  getCollisionDispatcher() { return _collisionDispatcher; }
        btDefaultCollisionConfiguration*        getDefaultCollisionConfig() { return _collisionConfiguration; }
        btBroadphaseInterface*                  getBrodaphaseInterface() { return _broadphase; }

        PhysicalBodyBox*            createPhysicalBodyBox(btVector3 halfExtents, btScalar mass, btVector3 pos);
        PhysicalBodyCylinder*       createPhysicalBodyCylinder(btVector3 dim, btScalar mass, btVector3 pos, ShapeAlign align);
        PhysicalBodySphere*         createPhysicalBodySphere(btScalar r, btScalar mass, btVector3 pos);
        PhysicalBodyStaticPlane*    createPhysicalBodyStaticPlane(btVector3 planeNormal, btScalar offset);

        ConstraintHinge*            createConstraintHinge(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB, btVector3 axisA, btVector3 axisB);
        ConstraintHinge2*           createConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB);

        void addConstraint(Constraint* c);
        void removeConstraint(Constraint* c);

    private:
        btDiscreteDynamicsWorld*                _dynamicsWorld;
        btSequentialImpulseConstraintSolver*    _constraintSolver;
        btCollisionDispatcher*                  _collisionDispatcher;
        btDefaultCollisionConfiguration*        _collisionConfiguration;
        btBroadphaseInterface*                  _broadphase;

        btAlignedObjectArray<PhysicalBody*>     _physicalBodies;
        btAlignedObjectArray<Constraint*>       _constraints;
};

#endif // PHYSICSMANAGER_HPP_INCLUDED
