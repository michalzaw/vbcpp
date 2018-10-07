#ifndef PHYSICSMANAGER_HPP_INCLUDED
#define PHYSICSMANAGER_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicalBodyBox.hpp"
#include "PhysicalBodyCylinder.hpp"
#include "PhysicalBodySphere.hpp"
#include "PhysicalBodyStaticPlane.hpp"
#include "PhysicalBodyConvexHull.hpp"
#include "PhysicalBodyBvtTriangleMesh.hpp"
#include "PhysicalBodyRaycastVehicle.h"
#include "PhysicalBodyWheel.h"


#include "ConstraintHinge.hpp"
#include "ConstraintHinge2.hpp"
#include "ConstraintBall.h"

#include "../Utils/RefCounter.h"

// PHYSICS MANAGER

class PhysicsManager : virtual public RefCounter
{
    public:
        PhysicsManager();
        virtual ~PhysicsManager();

        //static PhysicsManager& getInstance();

        int createPhysicsWorld();
        int destroyPhysicsWorld();

        void play();
        void stop();

        bool isRunning();

        void simulate(btScalar timeStep);

        btDiscreteDynamicsWorld*                getDynamicsWorld() { return _dynamicsWorld; }
        btSequentialImpulseConstraintSolver*    getConstraintSolver() { return _constraintSolver; }
        btCollisionDispatcher*                  getCollisionDispatcher() { return _collisionDispatcher; }
        btDefaultCollisionConfiguration*        getDefaultCollisionConfig() { return _collisionConfiguration; }
        btBroadphaseInterface*                  getBrodaphaseInterface() { return _broadphase; }

        PhysicalBodyBox*                createPhysicalBodyBox(btVector3 halfExtents, btScalar mass, short collisionGroup, short collisionFilter);
        PhysicalBodyCylinder*           createPhysicalBodyCylinder(btVector3 dim, btScalar mass, ShapeAlign align, short collisionGroup, short collisionFilter);
        PhysicalBodySphere*             createPhysicalBodySphere(btScalar r, btScalar mass, short collisionGroup, short collisionFilter);
        PhysicalBodyStaticPlane*        createPhysicalBodyStaticPlane(btVector3 planeNormal, btScalar offset, short collisionGroup, short collisionFilter);
        PhysicalBodyConvexHull*         createPhysicalBodyConvexHull(Vertex* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter);
        PhysicalBodyConvexHull*         createPhysicalBodyConvexHull(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter);
        PhysicalBodyBvtTriangleMesh*    createPhysicalBodyBvtTriangleMesh(RStaticModel* model, short collisionGroup, short collisionFilter);
        btCompoundShape*                createCompoundShape();
        PhysicalBodyRaycastVehicle*     createPhysicalBodyRayCastVehicle(Vertex* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter);
        PhysicalBodyRaycastVehicle*     createPhysicalBodyRayCastVehicle(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter);
        PhysicalBodyWheel*              createPhysicalBodyWheel(PhysicalBodyRaycastVehicle* vehicle, btVector3 connectionPoint, float suspensionRestLength, float radius, bool isFrontWheel);

        // Funkcja wywolywana przez SceneObject, nie wywolywac recznie
        void removePhysicalBody(PhysicalBody* physicalBody);

        ConstraintHinge*            createConstraintHinge(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB, btVector3 axisA, btVector3 axisB);
        ConstraintHinge2*           createConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB);
        ConstraintBall*             createConstraintBall(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB);

        void addConstraint(Constraint* c);
        void removeConstraint(Constraint* c);

        //friend class PhysicalBody;

    private:
        btDiscreteDynamicsWorld*                _dynamicsWorld;
        btSequentialImpulseConstraintSolver*    _constraintSolver;
        btCollisionDispatcher*                  _collisionDispatcher;
        btDefaultCollisionConfiguration*        _collisionConfiguration;
        btBroadphaseInterface*                  _broadphase;

        btAlignedObjectArray<PhysicalBody*>     _physicalBodies;
        btAlignedObjectArray<Constraint*>       _constraints;

        bool _running;
};

#endif // PHYSICSMANAGER_HPP_INCLUDED
