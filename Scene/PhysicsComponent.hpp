#ifndef PHYSICSCOMPONENT_HPP_INCLUDED
#define PHYSICSCOMPONENT_HPP_INCLUDED

#include "../Physics/PhysicalBody.hpp"
//#include "../Physics/PhysicsManager.hpp"
#include "Component.h"

class PhysicsManager;

class PhysicsComponent : virtual public Component
{
    public:
        PhysicsComponent(PhysicsManager* pm, PhysicalBody* b);
        virtual ~PhysicsComponent();

        void setPhysicalBody(PhysicalBody* b);

        PhysicalBody* getPhysicalBody();

    protected:
        PhysicsManager* _pManager;
        PhysicalBody*   _body;
};


#endif // PHYSICSCOMPONENT_HPP_INCLUDED
