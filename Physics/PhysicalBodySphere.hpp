#ifndef PHYSICALBODYSPHERE_HPP_INCLUDED
#define PHYSICALBODYSPHERE_HPP_INCLUDED

#include "PhysicalBody.hpp"

class PhysicalBodySphere : public PhysicalBody
{
    public:
        PhysicalBodySphere(btScalar r, btScalar mass, btVector3 pos);
        virtual ~PhysicalBodySphere();

        btScalar getRadius() { return _radius; }

    private:
        btScalar    _radius;

        void updateBody();
};

#endif // PHYSICALBODYSPHERE_HPP_INCLUDED
