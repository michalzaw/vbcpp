#include "PhysicalBodyGhost.h"


PhysicalBodyGhost::PhysicalBodyGhost()
	: PhysicalBody(0.0f)
{
	updateBody();
}


PhysicalBodyGhost::~PhysicalBodyGhost()
{

}


void PhysicalBodyGhost::update()
{
    _objectsBeginCollision.clear();
    _objectsEndCollision.clear();

    std::set<PhysicalBody*> objectsEndCollision = _collidesWith;

    for (int i = 0; i < _ghostObject->getNumOverlappingObjects(); ++i)
    {
        btCollisionObject* body = _ghostObject->getOverlappingObject(i);
        PhysicalBody* physicalBody = static_cast<PhysicalBody*>(body->getUserPointer());
        if (physicalBody != nullptr)
        {
            setCollisionWith(physicalBody);

            objectsEndCollision.erase(physicalBody);
        }
    }

    for (PhysicalBody* physicalBody : objectsEndCollision)
    {
        setNotCollisionWith(physicalBody);
    }
}


void PhysicalBodyGhost::changedTransform()
{
    if (_isUpdateTransformFromObject)
    {
        btTransform transf;

        const glm::mat4& m = _object->getGlobalTransformMatrix();
        transf.setFromOpenGLMatrix(&m[0][0]);

        _ghostObject->setWorldTransform(transf);
    }
}


void PhysicalBodyGhost::onAttachedToScenObject()
{
    if (_ghostObject)
    {
        _ghostObject->setUserPointer((PhysicalBody*) this);
    }
}


void PhysicalBodyGhost::updateBody()
{
    _collShape.reset(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));

    _ghostObject = new btGhostObject;
    _ghostObject->setCollisionShape(_collShape.get());
    _ghostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), _position));

    _ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
}
