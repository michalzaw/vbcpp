#ifndef BUS_H_INCLUDED
#define BUS_H_INCLUDED

#include "../Utils/RefCounter.h"
#include "../Utils/Strings.h"

#include "../Utils/Gearbox.h"
#include "../Utils/Engine.h"

#include "SceneObject.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "../Physics/PhysicsManager.hpp"

#include "DoorSimple.h"
#include "DoorSE.h"

#include <memory>

enum WheelSide
{
    WS_RIGHT = 0,
    WS_LEFT
};

struct Wheel
{
    PhysicalBodyCylinder* body;
    ConstraintHinge2*     suspension;
    bool                  steering;
    bool                  powered;
    float                 currentAngle;
    float                 brakeForce;
    WheelSide             wheelSide;
};

typedef std::vector<Wheel*> WheelList;
typedef std::vector<Door*>  DoorList;
typedef std::vector<Light*> LightsList;
typedef std::vector<Light*> HeadlightsList;


//! Bus vehicle class
class Bus : virtual public RefCounter
{
    public:
        Bus(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename);
        virtual ~Bus();

        SceneObject* getSceneObject() { return _sceneObject; }

        // Steering wheel methods
        glm::vec3 getDriverPosition();
        SceneObject* getSteeringWheelObject() { return _steeringWheelObject; }

        // Lights methods
        void setIsEnableLights(bool is);
        bool isEnableLights();
        void setIsEnableHeadlights(bool is);
        bool isEnableHeadlights();

        // Driving methods
        void turnLeft(float dt);
        void turnRight(float dt);
        void centerSteringWheel(float dt);
        void accelerate();
        void idle();
        void brakeOn();
        void brakeOff();
        void toggleHandbrake();

        Gearbox* getGearbox() { if (_gearbox) return _gearbox.get(); else return 0; }

        Engine* getEngine() { if (_engine) return _engine.get(); else return 0; }

        void startEngine();
        void stopEngine();

        // Door methods
        void doorOpenClose(char doorGroup);
        //void closeDoor(unsigned char doorIndex);
        Door* getDoor(unsigned char doorIndex);

        // Update internal physics - doors, wheels etc
        void updatePhysics(float dt);

    private:
        SceneObject*    _sceneObject;
        PhysicalBodyConvexHull* _chasisBody;
        SceneManager*   _sMgr;
        PhysicsManager* _pMgr;
        SoundManager*   _sndMgr;

        std::unique_ptr<Gearbox>    _gearbox;
        std::unique_ptr<Engine>     _engine;

        float   _maxSteerAngle;
        float   _steerStep;

        bool    _brake;
        bool    _accelerate;
        bool    _handbrake;
        bool    _idle;

        float   _brakeForce;
        float   _brakeForceStep;

        SceneObject*    _steeringWheelObject;
        glm::vec3       _driverPosition;
        LightsList      _lights;
        bool            _isEnableLights;
        HeadlightsList  _headlights;
        bool            _isEnableHeadlights;
        int             _collidesWith;

        WheelList       _wheels;
        DoorList        _doors;






        // Load config file
        void loadXMLdata(std::string busname);


};


#endif // BUS_H_INCLUDED
