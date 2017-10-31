#ifndef BUSRAYCAST_H_INCLUDED
#define BUSRAYCAST_H_INCLUDED


#include <string>

#include "../Scene/SceneManager.h"
#include "../Scene/SoundManager.h"

#include "../Physics/PhysicsManager.hpp"
#include "../Physics/PhysicalBodyWheel.h"


class BusRaycast
{
    public:
        SceneManager*   _sMgr;
        PhysicsManager* _pMgr;
        SoundManager*   _sndMgr;

        SceneObject* _busSceneObject;

        btRaycastVehicle* _bulletVehicle;

        float _wheelAngle;

    public:
        BusRaycast(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename);
        virtual ~BusRaycast();

        void update()
        {
            _bulletVehicle->updateWheelTransform(0, true);
            _bulletVehicle->updateWheelTransform(1, true);
            _bulletVehicle->updateWheelTransform(2, true);
            _bulletVehicle->updateWheelTransform(3, true);
        }

        void turnRight(float dt)
        {
            _wheelAngle -= 0.3 * dt;
            if (_wheelAngle < -0.55)
                _wheelAngle = -0.55;
            _bulletVehicle->setSteeringValue(_wheelAngle, 0);
            _bulletVehicle->setSteeringValue(_wheelAngle, 1);
        }

        void turnLeft(float dt)
        {
            _wheelAngle += 0.3 * dt;
            if (_wheelAngle > 0.55)
                _wheelAngle = 0.55;
            _bulletVehicle->setSteeringValue(_wheelAngle, 0);
            _bulletVehicle->setSteeringValue(_wheelAngle, 1);
        }

        void centerSteringWheel(float dt)
        {
            float deadZone = 0.0025f;

            if (_wheelAngle > deadZone)
            {
                _wheelAngle -= dt * 0.3;

                _bulletVehicle->setSteeringValue(_wheelAngle, 0);
                _bulletVehicle->setSteeringValue(_wheelAngle, 1);
            }
            else
            if (_wheelAngle < -deadZone)
            {
                _wheelAngle += dt * 0.3;

                _bulletVehicle->setSteeringValue(_wheelAngle, 0);
                _bulletVehicle->setSteeringValue(_wheelAngle, 1);
            }
        }
};


#endif // BUSRAYCAST_H_INCLUDED
