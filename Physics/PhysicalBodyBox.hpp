#ifndef PHYSICALBODYBOX_HPP_INCLUDED
#define PHYSICALBODYBOX_HPP_INCLUDED

#include "PhysicalBody.hpp"

class PhysicalBodyBox : public PhysicalBody
{
    public:
        PhysicalBodyBox(btVector3 halfExtents, btScalar mass);
        virtual ~PhysicalBodyBox();

        btVector3 getHalfExtents() { return _halfExtents; }

    private:
        btVector3   _halfExtents;

        void updateBody();
};

#endif // PHYSICALBODYBOX_HPP_INCLUDED
