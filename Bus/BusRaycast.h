#ifndef BUSRAYCAST_H_INCLUDED
#define BUSRAYCAST_H_INCLUDED


#include <string>

#include "Bus.h"
#include "DoorSimple.h"
#include "DoorSE.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SoundManager.h"

#include "../Physics/PhysicsManager.hpp"
#include "../Physics/PhysicalBodyWheel.h"
#include "../Physics/PhysicalBodyRaycastVehicle.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;


struct BusRayCastModule
{
    SceneObject* sceneObject;
    PhysicalBodyRaycastVehicle* rayCastVehicle;
    btVector3 jointPosition;
};


struct BusRayCastWheel
{
    PhysicalBodyWheel* wheel;
    bool                  steering;
    bool                  powered;
    bool                  handbrake;
    float                 maxBrakeForce;
    WheelSide             wheelSide;
};


class BusRaycast : public Bus
{
    static constexpr float STEER_STEP = 0.3f;
    static constexpr float DEAD_ZONE = 0.0025f;

    public:
        BusRaycast(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename);
        virtual ~BusRaycast();

        SceneObject* getSceneObject() { return _modules[0].sceneObject; }

        virtual BusRayCastModule& getModule(int index);

        virtual glm::vec3 getDriverPosition();
        virtual SceneObject* getSteeringWheelObject();

        virtual void setIsEnableLights(bool is);
        virtual bool isEnableLights();
        virtual void setIsEnableHeadlights(bool is);
        virtual bool isEnableHeadlights();

        virtual void turnLeft(float dt);
        virtual void turnRight(float dt);
        virtual void centerSteringWheel(float dt);
        virtual void accelerate();
        virtual void idle();
        virtual void brakeOn();
        virtual void brakeOff();
        virtual void toggleHandbrake();

        virtual Gearbox* getGearbox();
        virtual Engine* getEngine();
        virtual void startEngine();
        virtual void stopEngine();

        virtual void doorOpenClose(char doorGroup);
        virtual Door* getDoor(unsigned char doorIndex);
        virtual int getDoorsCount();

        virtual void update(float deltaTime);

    public:
        SceneManager*   _sMgr;
        PhysicsManager* _pMgr;
        SoundManager*   _sndMgr;

        std::unique_ptr<Gearbox>    _gearbox;
        std::unique_ptr<Engine>     _engine;

        std::vector<BusRayCastModule> _modules;

        float   _maxSteerAngle;
        float   _steerAngle;

        bool    _brake;
        bool    _accelerate;
        bool    _handbrake;
        bool    _idle;

        float   _brakeForce;
        float   _brakeForceStep;

        SceneObject*    _steeringWheelObject;
        SceneObject*    _desktopObject;
        glm::vec3       _driverPosition;
        LightsList      _lights;
        bool            _isEnableLights;
        LightsList      _headlights;
        bool            _isEnableHeadlights;

        std::vector<BusRayCastWheel*>       _wheels;
        DoorList        _doors;

        int             _collidesWith;

        void loadXMLdata(std::string busname);
        bool isAllDoorClosed()
        {
            for (unsigned char i = 0; i < _doors.size(); i++)
            {
                if (_doors[i]->getState() == EDS_OPENING)
                    return false;
            }
            return true;
        }

        void setRandomNumberOfPassengersGettingOff()
        {
            if (_numberOfPassengers != 0)
                _numberOfPassengersGettingOff = rand() % _numberOfPassengers;
            else
                _numberOfPassengersGettingOff = 0;
        }
};


#endif // BUSRAYCAST_H_INCLUDED
