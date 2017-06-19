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

enum WheelSide
{
    WS_RIGHT = 0,
    WS_LEFT
};

/*
enum DoorState
{
    EDS_OPENING = 0,
    EDS_CLOSING = 1
};


struct Door
{
    PhysicalBodyConvexHull*  body;
    RModel*                  model;
    ConstraintHinge*         hinge;
    DoorState                state;

    void open()
    {
        hinge->getBulletConstraint()->enableAngularMotor(true, -1.9f, 1.15f);
        state = EDS_OPENING;
    }

    void close()
    {
        hinge->getBulletConstraint()->enableAngularMotor(true, 1.9f, 1.15f);
        state = EDS_CLOSING;
    }
};

*/

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

        Gearbox* getGearbox() { if (_gearbox) return _gearbox; }

        Engine* getEngine() { if (_engine) return _engine; }

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

        Gearbox*        _gearbox;
        Engine*         _engine;

        SceneObject*    _steeringWheelObject;
        glm::vec3       _driverPosition;
        LightsList      _lights;
        bool            _isEnableLights;
        HeadlightsList  _headlights;
        bool            _isEnableHeadlights;
        int             _collidesWith;

        WheelList       _wheels;
        DoorList        _doors;

        bool    _accelerate;
        bool    _brake;
        bool    _idle;
        bool    _handbrake;

        float   _brakeForce;
        float   _brakeForceStep;

        float   _maxSteerAngle;
        float   _steerStep;

        // Load config file
        void loadXMLdata(std::string busname);


};


#endif // BUS_H_INCLUDED
