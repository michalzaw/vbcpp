#ifndef PHYSICALBODYWHEEL_H_INCLUDED
#define PHYSICALBODYWHEEL_H_INCLUDED


#include "PhysicalBody.hpp"


class PhysicalBodyWheel : public PhysicalBody
{
    public:
        PhysicalBodyWheel(btRaycastVehicle* vehicle, int index);
        virtual ~PhysicalBodyWheel();

    private:
        btRaycastVehicle* _vehicle;
        int _index;

        virtual void update();
        virtual void changedTransform()
        {

        }

};


#endif // PHYSICALBODYWHEEL_H_INCLUDED
