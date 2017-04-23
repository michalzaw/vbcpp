#ifndef PHYSICALBODYCYLINDER_HPP_INCLUDED
#define PHYSICALBODYCYLINDER_HPP_INCLUDED

#include "PhysicalBody.hpp"

enum ShapeAlign
{
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS
};

class PhysicalBodyCylinder : virtual public PhysicalBody
{
    public:
        PhysicalBodyCylinder(btVector3 dim, btScalar mass, ShapeAlign align = Y_AXIS);
        virtual ~PhysicalBodyCylinder();

    private:
        btVector3   _dimensions;
        ShapeAlign  _align;

        void updateBody();
};

#endif // PHYSICALBODYCYLINDER_HPP_INCLUDED
