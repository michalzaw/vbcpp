#ifndef PHYSICALBODYSPHERE_HPP_INCLUDED
#define PHYSICALBODYSPHERE_HPP_INCLUDED

#include "PhysicalBody.hpp"

class PhysicalBodySphere : public PhysicalBody
{
    VBCPP_COMPONENT(PhysicalBodySphere, CT_PHYSICAL_BODY)

    public:
        PhysicalBodySphere(btScalar r, btScalar mass);
        virtual ~PhysicalBodySphere();

        btScalar getRadius() { return _radius; }

    private:
        btScalar    _radius;

        void updateBody();
};

#endif // PHYSICALBODYSPHERE_HPP_INCLUDED
