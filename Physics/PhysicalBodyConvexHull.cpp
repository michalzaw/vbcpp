#include "PhysicalBodyConvexHull.hpp"

PhysicalBodyConvexHull::PhysicalBodyConvexHull(Vertex* vertices, unsigned int vertexCount, btScalar mass, bool centerOfMassOffset, btVector3 centerOfMassOffsetValue)
: PhysicalBody(mass, centerOfMassOffset, centerOfMassOffsetValue),
_vertices(vertices), _vertexCount(vertexCount), _verticesvec(nullptr)
{
    updateBody();
}


PhysicalBodyConvexHull::PhysicalBodyConvexHull(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, bool centerOfMassOffset, btVector3 centerOfMassOffsetValue)
: PhysicalBody(mass, centerOfMassOffset, centerOfMassOffsetValue),
_verticesvec(vertices), _vertexCount(vertexCount), _vertices(nullptr)
{
    updateBody();
}


PhysicalBodyConvexHull::PhysicalBodyConvexHull(std::vector<glm::vec3> vertices, btScalar mass, bool centerOfMassOffset, btVector3 centerOfMassOffsetValue)
	: PhysicalBody(mass, centerOfMassOffset, centerOfMassOffsetValue),
	_verticesVector(vertices), _verticesvec(nullptr), _vertexCount(0), _vertices(nullptr)
{
	updateBody();
}



PhysicalBodyConvexHull::~PhysicalBodyConvexHull()
{
    printf("ConvexHullShape - Destruktor\n");
}


void PhysicalBodyConvexHull::updateBody()
{

	if (!_centerOfMassOffset)
	{
		if (_vertices)
			_collShape.reset(new btConvexHullShape((btScalar*)_vertices, _vertexCount, sizeof(Vertex)));//32);
		else if (_verticesvec)
			_collShape.reset(new btConvexHullShape((btScalar*)_verticesvec, _vertexCount, sizeof(glm::vec3)));//32);
		else
		{
			_collShape.reset(new btConvexHullShape((btScalar*)_verticesVector.data(), _verticesVector.size(), sizeof(glm::vec3)));
		}

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

		_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position)));

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState.get(), _collShape.get(), inertia);
		_rigidBody.reset(new btRigidBody(rigidBodyCI));
	}
	else
	{
		btConvexHullShape* shape = nullptr;
		if (_vertices)
			shape = new btConvexHullShape((btScalar*)_vertices, _vertexCount, sizeof(Vertex));//32);
		else if (_verticesvec)
			shape = new btConvexHullShape((btScalar*)_verticesvec, _vertexCount, sizeof(glm::vec3));//32);
		else
			_collShape.reset(new btConvexHullShape((btScalar*)&_verticesVector[0], _verticesVector.size(), sizeof(glm::vec3)));

		btCompoundShape* compoundShape = new btCompoundShape;

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(-_centerOfMassOffsetValue);

		compoundShape->addChildShape(transform, shape);

		btVector3 inertia(0, 0, 0);
		btScalar masses[1] = { _mass };
		btTransform principal;
		principal.setIdentity();
		principal.setOrigin(btVector3(0, 0, 0));
		compoundShape->calculatePrincipalAxisTransform(masses, principal, inertia);

		btTransform centerOfMassOffsetTransform;
		centerOfMassOffsetTransform.setIdentity();
		centerOfMassOffsetTransform.setOrigin(-_centerOfMassOffsetValue);
		_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position), centerOfMassOffsetTransform));

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState.get(), compoundShape, inertia);
		_rigidBody.reset(new btRigidBody(rigidBodyCI));

		_collShape.reset(compoundShape);
	}

	//todo: remove compoindshape childeren in destructor
}
