#include "PhysicsComponent.hpp"
#include "../Physics/PhysicsManager.hpp"

PhysicsComponent::PhysicsComponent(PhysicsManager* pm, PhysicalBody* b)
: Component(CT_PHYSICAL_BODY),
_pManager(pm), _body(b)
{

}


PhysicsComponent::~PhysicsComponent()
{
    _pManager->removePhysicalBody(_body);
}


void PhysicsComponent::setPhysicalBody(PhysicalBody* b)
{
    _body = b;
}


PhysicalBody* PhysicsComponent::getPhysicalBody()
{
    return _body;
}
