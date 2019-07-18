#include "PhysicsManager.hpp"
#include "PhysicalBody.hpp"
#include "PhysicalBodyBox.hpp"
#include "PhysicalBodyCylinder.hpp"
#include "PhysicalBodySphere.hpp"
#include "PhysicalBodyStaticPlane.hpp"
#include "PhysicalBodyConvexHull.hpp"

#include "ConstraintHinge.hpp"

#include <cstdio>

//static std::unique_ptr<PhysicsManager> pmInstance;

/*
PhysicsManager& PhysicsManager::getInstance()
{
    if( !pmInstance )
        pmInstance = std::unique_ptr<PhysicsManager>(new PhysicsManager);

    return* pmInstance;
}
*/

PhysicsManager::PhysicsManager()
: _dynamicsWorld(0),
  _constraintSolver(0),
  _collisionDispatcher(0),
  _collisionConfiguration(0),
  _broadphase(0),
  _running(false)
{
    createPhysicsWorld();
}

PhysicsManager::~PhysicsManager()
{
  	//printf("Ilosc obiektow kolizji przed czyszczeniem: %d\n", (int)(_dynamicsWorld->getNumCollisionObjects()));

    std::cout << "Ilosc polaczen na stosie: " << _constraints.size() << std::endl;
	//delete collision shapes
	for (int j = 0; j < _constraints.size(); j++)
	{
	    std::cout << "Constraint: " << _constraints[j] << std::endl;
        Constraint* c = _constraints[j];

        std::cout << "Bullet Constraint: " << c->getBulletConstraint() << std::endl;
        //removePhysicalBody(body);
        _dynamicsWorld->removeConstraint(c->getBulletConstraint());

        delete c;
	}
	_constraints.clear();

	//delete collision shapes
	for (int j = 0; j < _physicalBodies.size(); j++)
	{
        PhysicalBody* body = _physicalBodies[j];

        //removePhysicalBody(body);
        if (body->getRigidBody() != NULL)
            _dynamicsWorld->removeRigidBody(body->getRigidBody());

        delete body;
	}
	_physicalBodies.clear();


	//printf("Ilosc obiektow kolizji po czyszczeniu: %d\n", (int)(_dynamicsWorld->getNumCollisionObjects()));

    destroyPhysicsWorld();
}


void PhysicsManager::play()
{
    _running = true;
}


void PhysicsManager::stop()
{
    _running = false;
}


bool PhysicsManager::isRunning()
{
    return _running;
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
    if (_running)
    {
        _dynamicsWorld->stepSimulation(timeStep, 2);
        //_dynamicsWorld->stepSimulation(1/60.0f, 5);

        for (int i = 0; i < _physicalBodies.size(); i++)
        {
            _physicalBodies[i]->update();
        }
    }

}


PhysicalBodyBox* PhysicsManager::createPhysicalBodyBox(btVector3 halfExtents, btScalar mass, short collisionGroup, short collisionFilter)
{
    PhysicalBodyBox* b = new PhysicalBodyBox(halfExtents, mass);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}


PhysicalBodyCylinder* PhysicsManager::createPhysicalBodyCylinder(btVector3 dim, btScalar mass, ShapeAlign align, short collisionGroup, short collisionFilter)
{
    PhysicalBodyCylinder* b = new PhysicalBodyCylinder(dim, mass, align);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}


PhysicalBodySphere* PhysicsManager::createPhysicalBodySphere(btScalar r, btScalar mass, short collisionGroup, short collisionFilter)
{
    PhysicalBodySphere* b = new PhysicalBodySphere(r, mass);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}


PhysicalBodyStaticPlane* PhysicsManager::createPhysicalBodyStaticPlane(btVector3 planeNormal, btScalar offset, short collisionGroup, short collisionFilter)
{
    PhysicalBodyStaticPlane* b = new PhysicalBodyStaticPlane(planeNormal, offset);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}

PhysicalBodyConvexHull* PhysicsManager::createPhysicalBodyConvexHull(Vertex* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter)
{
    PhysicalBodyConvexHull* b = new PhysicalBodyConvexHull(vertices, vertexCount, mass);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}

PhysicalBodyConvexHull* PhysicsManager::createPhysicalBodyConvexHull(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter)
{
    PhysicalBodyConvexHull* b = new PhysicalBodyConvexHull(vertices, vertexCount, mass);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}

PhysicalBodyBvtTriangleMesh* PhysicsManager::createPhysicalBodyBvtTriangleMesh(RStaticModel* model, short collisionGroup, short collisionFilter)
{
    PhysicalBodyBvtTriangleMesh* b = new PhysicalBodyBvtTriangleMesh(model);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}


btCompoundShape* PhysicsManager::createCompoundShape()
{
    btCompoundShape* c = new btCompoundShape();
}


PhysicalBodyRaycastVehicle* PhysicsManager::createPhysicalBodyRayCastVehicle(Vertex* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter)
{
    PhysicalBodyRaycastVehicle* b = new PhysicalBodyRaycastVehicle(vertices, vertexCount, mass, this);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}

PhysicalBodyRaycastVehicle* PhysicsManager::createPhysicalBodyRayCastVehicle(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, short collisionGroup, short collisionFilter)
{
    PhysicalBodyRaycastVehicle* b = new PhysicalBodyRaycastVehicle(vertices, vertexCount, mass, this);

    _dynamicsWorld->addRigidBody(b->getRigidBody(), collisionGroup, collisionFilter);

    _physicalBodies.push_back(b);

    return b;
}


PhysicalBodyWheel* PhysicsManager::createPhysicalBodyWheel(PhysicalBodyRaycastVehicle* vehicle, btVector3 connectionPoint, float suspensionRestLength, float radius, bool isFrontWheel)
{
    int index = vehicle->addWheel(connectionPoint, suspensionRestLength, radius, isFrontWheel);
    PhysicalBodyWheel* b = new PhysicalBodyWheel(vehicle->getRayCastVehicle(), index);

    _physicalBodies.push_back(b);

    return b;
}


void PhysicsManager::removePhysicalBody(PhysicalBody* physicalBody)
{
	btRigidBody* rigidBody = physicalBody->getRigidBody();
	if (rigidBody != nullptr)
	{
		_dynamicsWorld->removeRigidBody(rigidBody);
		_dynamicsWorld->removeCollisionObject(rigidBody);
	}

    _physicalBodies.remove(physicalBody);

    delete physicalBody;
}

// CONSTRAINTS

ConstraintHinge* PhysicsManager::createConstraintHinge(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB, btVector3 axisA, btVector3 axisB)
{
    ConstraintHinge* c = new ConstraintHinge(bodyA, bodyB, pivotA, pivotB, axisA, axisB);

    std::cout << "Constraint created: " << c << std::endl;
    std::cout << "Bullet constraint: " << c->getBulletConstraint() << std::endl;

    _dynamicsWorld->addConstraint(c->getBulletConstraint(), true);

    _constraints.push_back(c);

    return c;
}


ConstraintHinge2* PhysicsManager::createConstraintHinge2(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivot, btVector3 axisA, btVector3 axisB)
{
    ConstraintHinge2* c = new ConstraintHinge2(bodyA, bodyB, pivot, axisA, axisB);

    std::cout << "Constraint created: " << c << std::endl;
    std::cout << "Bullet constraint: " << c->getBulletConstraint() << std::endl;

    _dynamicsWorld->addConstraint(c->getBulletConstraint(), true);

    _constraints.push_back(c);

    return c;
}


ConstraintBall* PhysicsManager::createConstraintBall(PhysicalBody* bodyA, PhysicalBody* bodyB, btVector3 pivotA, btVector3 pivotB)
{
    ConstraintBall* c = new ConstraintBall(bodyA, bodyB, pivotA, pivotB);

    std::cout << "Constraint created: " << c << std::endl;
    std::cout << "Bullet constraint: " << c->getBulletConstraint() << std::endl;

    _dynamicsWorld->addConstraint(c->getBulletConstraint(), true);

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

