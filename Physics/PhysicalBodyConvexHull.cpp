#include "PhysicalBodyConvexHull.hpp"

PhysicalBodyConvexHull::PhysicalBodyConvexHull(Vertex* vertices, unsigned int vertexCount, btScalar mass, btVector3 pos)
: PhysicalBody(mass, pos),
_vertices(vertices), _vertexCount(vertexCount)
{
    updateBody();
}


PhysicalBodyConvexHull::~PhysicalBodyConvexHull()
{
    printf("ConvexHullShape - Destruktor\n");
}


void PhysicalBodyConvexHull::updateBody()
{

    _collShape.reset( new btConvexHullShape((btScalar*)_vertices, _vertexCount, sizeof(Vertex)) );//32);

    /*
    _collShape = new btConvexHullShape;

    for (int i = 0; i < _vertexCount; i++)
    {
        btVector3 point(btVector3(_vertices[i].Position[0], _vertices[i].Position[1], _vertices[i].Position[2]));

        dynamic_cast<btConvexHullShape>(_collShape->addPoint(btVector3(0,0,0)));
    }
    */

    btVector3 inertia(0, 0, 0);
    _collShape->calculateLocalInertia(_mass, inertia);

    _motionState.reset( new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position)) );

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState.get(), _collShape.get(), inertia);
    _rigidBody.reset( new btRigidBody(rigidBodyCI) );
}
