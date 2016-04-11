#ifndef PHYSICSMANAGER_HPP_INCLUDED
#define PHYSICSMANAGER_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

class PhysicalBody;

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

        void addPhysicalBody(PhysicalBody* body);
        void removePhysicalBody(PhysicalBody* body);

    private:
        btDiscreteDynamicsWorld*                _dynamicsWorld;
        btSequentialImpulseConstraintSolver*    _constraintSolver;
        btCollisionDispatcher*                  _collisionDispatcher;
        btDefaultCollisionConfiguration*        _collisionConfiguration;
        btBroadphaseInterface*                  _broadphase;
};

#endif // PHYSICSMANAGER_HPP_INCLUDED
