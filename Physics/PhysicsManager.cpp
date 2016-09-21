#include "PhysicsManager.hpp"
#include "PhysicalBody.hpp"
#include "PhysicalBodyBox.hpp"
#include "PhysicalBodyCylinder.hpp"
#include "PhysicalBodySphere.hpp"
#include "PhysicalBodyStaticPlane.hpp"
#include "PhysicalBodyConvexHull.hpp"

#include "ConstraintHinge.hpp"

#include <cstdio>

static std::unique_ptr<PhysicsManager> pmInstance;

PhysicsManager& PhysicsManager::getInstance()
{
    if( !pmInstance )
        pmInstance = std::unique_ptr<PhysicsManager>(new PhysicsManager);

    return* pmInstance;
}


PhysicsManager::PhysicsManager()
: _dynamicsWorld(0),
  _constraintSolver(0),
  _collisionDispatcher(0),
  _collisionConfiguration(0),
  _broadphase(0)
{

}

PhysicsManager::~PhysicsManager()
{
  	//printf("Ilosc obiektow kolizji przed czyszczeniem: %d\n", (int)(_dynamicsWorld->getNumCollisionObjects()));

	//delete collision shapes
	for (int j = 0; j < _physicalBodies.size(); j++)
	{
        PhysicalBody* body = _physicalBodies[j];

        //removePhysicalBody(body);
        _dynamicsWorld->removeRigidBody(body->getRigidBody());

        delete body;
	}
	_physicalBodies.clear();


	//delete collision shapes
	for (int j = 0; j < _constraints.size(); j++)
	{
        Constraint* c = _constraints[j];

        //removePhysicalBody(body);
        _dynamicsWorld->removeConstraint(c->getBulletConstraint());

        delete c;
	}
	_constraints.clear();

	//printf("Ilosc obiektow kolizji po czyszczeniu: %d\n", (int)(_dynamicsWorld->getNumCollisionObjects()));

    destroyPhysicsWorld();
}


int PhysicsManager::createPhysicsWorld()
{
    // Build the broadphase
    _broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    _collisionConfiguration = new btDefaultCollisionConfiguration();
    _collisionDispatcher = new btCollisionDispatcher(_collisionConfiguration);

    // The actual physics solver
    _constraintSolver = new btSequentialImpulseConstraintSolver;

    // The world.
    _dynamicsWorld = new btDiscreteDynamicsWorld(_collisionDispatcher, _broadphase, _constraintSolver, _collisionConfiguration);

    // Setting gravity force (we use 10 for simplicity)
    _dynamicsWorld->setGravity(btVector3(0, -10, 0));

    return 0;
}


int PhysicsManager::destroyPhysicsWorld()
{
    // Cleaning Bullet's main pointer

    delete _dynamicsWorld;
    delete _constraintSolver;
    delete _collisionDispatcher;
    delete _collisionConfiguration;
    delete _broadphase;

    return 0;
}


void PhysicsManager::simulate(btScalar timeStep)
{
    _dynamicsWorld->stepSimulation(timeStep, 1);
    //_dynamicsWorld->stepSimulation(1/60.0f, 5);

    for (int i = 0; i < _physicalBodies.size(); i++)
	{
        _physicalBodies[i]->update();
	}
}


PhysicalBodyBox* PhysicsManager::createPhysicalBodyBox(btVector3 halfExtents, btScalar mass, btVector3 pos)
{
    PhysicalBodyBox* b = new PhysicalBodyBox(halfExtents, mass, pos);

    _dynamicsWorld->addRigidBody(b->getRigidBody());

    _physicalBodies.push_back(b);

    return b;
}


PhysicalBodyCylinder* PhysicsManager::createPhysicalBodyCylinder(btVector3 dim, btScalar mass, btVector3 pos, ShapeAlign align)
{
    PhysicalBodyCylinder* b = new PhysicalBodyCylinder(dim, mass, pos, align);

    _dynamicsWorld->addRigidBody(b->getRigidBody());

    _physicalBodies.push_back(b);

    return b;
}


PhysicalBodySphere* PhysicsManager::createPhysicalBodySphere(btScalar r, btScalar mass, btVector3 pos)
{
    PhysicalBodySphere* b = new PhysicalBodySphere(r, mass, pos);

    _dynamicsWorld->addRigidBody(b->getRigidBody());

    _physicalBodies.push_back(b);

    return b;
}


PhysicalBodyStaticPlane* PhysicsManager::createPhysicalBodyStaticPlane(btVector3 planeNormal, btScalar offset)
{
    PhysicalBodyStaticPlane* b = new PhysicalBodyStaticPlane(planeNormal, offset);

    _dynamicsWorld->addRigidBody(b->getRigidBody());

    _physicalBodies.push_back(b);

    return b;
}

PhysicalBodyConvexHull* PhysicsManager::createPhysicalBodyConvexHull(Vertex* vertices, unsigned int vertexCount, btScalar mass, btVector3 pos)
{
    PhysicalBodyConvexHull* b = new PhysicalBodyConvexHull(vertices, vertexCount, mass, pos);

    _dynamicsWorld->addRigidBody(b->getRigidBody());

    _physicalBodies.push_back(b);

    return b;
}

PhysicalBodyBvtTriangleMesh* PhysicsManager::createPhysicalBodyBvtTriangleMesh(RModel* model, btVector3 pos)
{
    PhysicalBodyBvtTriangleMesh* b = new PhysicalBodyBvtTriangleMesh(model, pos);

    _dynamicsWorld->addRigidBody(b->getRigidBody());

    _physicalBodies.push_back(b);

    return b;
}

void PhysicsManager::removePhysicalBody(PhysicalBody* physicalBody)
{
    _dynamicsWorld->removeRigidBody(physicalBody->getRigidBody());

    _physicalBodies.remove(physicalBody);

    delete physicalBody;
}

// CONSTRAINTS

ConstraintHinge* PhysicsManager::createConstraintHinge(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB, btVector3 axisA, btVector3 axisB)
{
    ConstraintHinge* c = new ConstraintHinge(bodyA, bodyB, pivotA, pivotB, axisA, axisB);

    _dynamicsWorld->addConstraint(c->getBulletConstraint());

    _constraints.push_back(c);

    return c;
}


ConstraintHinge2* PhysicsManager::createConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB)
{
    ConstraintHinge2* c = new ConstraintHinge2(bodyA, bodyB, pivot, axisA, axisB);

    _dynamicsWorld->addConstraint(c->getBulletConstraint());

    _constraints.push_back(c);

    return c;
}


void PhysicsManager::addConstraint(Constraint* c)
{
    _dynamicsWorld->addConstraint(c->getBulletConstraint());

    _constraints.push_back(c);
}


void PhysicsManager::removeConstraint(Constraint* c)
{
    _dynamicsWorld->removeConstraint(c->getBulletConstraint());

    _constraints.remove(c);
}

