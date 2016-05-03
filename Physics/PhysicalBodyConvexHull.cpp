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
    safe_delete<btRigidBody>(_rigidBody);
    safe_delete<btCollisionShape>(_collShape);
    safe_delete<btDefaultMotionState>(_motionState);


    _collShape = new btConvexHullShape((btScalar*)_vertices, _vertexCount, 32);

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

    _motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState, _collShape, inertia);
    _rigidBody = new btRigidBody(rigidBodyCI);
}
