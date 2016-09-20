#include "PhysicalBodyCylinder.hpp"

#include "../Utils/Helpers.hpp"

PhysicalBodyCylinder::PhysicalBodyCylinder(btVector3 dim, btScalar mass, btVector3 pos, ShapeAlign align)
: PhysicalBody(mass, pos),
_dimensions(dim), _align(align)
{
    updateBody();
}


PhysicalBodyCylinder::~PhysicalBodyCylinder()
{
    printf("PhysicalBodyBox - Destruktor\n");
}


void PhysicalBodyCylinder::updateBody()
{

    btVector3 halfExtents(_dimensions.getX() * 0.5f, _dimensions.getY() * 0.5f, _dimensions.getZ() * 0.5f);

    switch(_align)
    {
        case X_AXIS:
            _collShape.reset( new btCylinderShapeX(halfExtents) );
            break;

        case Y_AXIS:
            _collShape.reset( new btCylinderShape(halfExtents) );
            break;

        case Z_AXIS:
            _collShape.reset( new btCylinderShapeZ(halfExtents) );
            break;
    }

    btVector3 inertia(0, 0, 0);
    _collShape->calculateLocalInertia(_mass, inertia);

    _motionState.reset( new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position)) );

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState.get(), _collShape.get(), inertia);
    _rigidBody.reset( new btRigidBody(rigidBodyCI) );
}
