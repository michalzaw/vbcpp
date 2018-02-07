#ifndef PHYSICALBODYWHEEL_H_INCLUDED
#define PHYSICALBODYWHEEL_H_INCLUDED


#include "PhysicalBody.hpp"


class PhysicalBodyWheel : public PhysicalBody
{
    public:
        PhysicalBodyWheel(btRaycastVehicle* vehicle, int index);
        virtual ~PhysicalBodyWheel();

        float getSteeringValue();
        void setSteeringValue(float angle);

        void setBrake(float brake)
        {
            _vehicle->setBrake(brake, _index);
        }

        void applyEngineForce(float force)
        {
            _vehicle->applyEngineForce(force, _index);
        }

        btWheelInfo& getWheelInfo();

    private:
        btRaycastVehicle* _vehicle;
        int _index;

        float _steeringValue;

        virtual void update();
        virtual void changedTransform()
        {

        }

};


#endif // PHYSICALBODYWHEEL_H_INCLUDED
