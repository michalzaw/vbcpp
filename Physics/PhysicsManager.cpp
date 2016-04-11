#include "PhysicsManager.hpp"
#include "PhysicalBody.hpp"

#include <cstdio>

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


void PhysicsManager::simulate()
{
    _dynamicsWorld->stepSimulation(1 / 60.f, 10);
}

void PhysicsManager::addPhysicalBody(PhysicalBody* body)
{
    _dynamicsWorld->addRigidBody(body->getRigidBody());
}


void PhysicsManager::removePhysicalBody(PhysicalBody* body)
{
    _dynamicsWorld->removeRigidBody(body->getRigidBody());
}
