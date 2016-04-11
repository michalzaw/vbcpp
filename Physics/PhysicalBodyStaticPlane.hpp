#ifndef PHYSICALBODYSTATICPLANE_HPP_INCLUDED
#define PHYSICALBODYSTATICPLANE_HPP_INCLUDED

#include "PhysicalBody.hpp"

class PhysicalBodyStaticPlane : public PhysicalBody
{
    public:
        PhysicalBodyStaticPlane(btVector3 planeNormal, btScalar offset);
        virtual ~PhysicalBodyStaticPlane();

    private:
        btScalar    _offset;

        void updateBody();
};

#endif // PHYSICALBODYSTATICPLANE_HPP_INCLUDED
