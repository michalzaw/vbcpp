#ifndef BUS_H_INCLUDED
#define BUS_H_INCLUDED

#include "../Utils/RefCounter.h"

#include "SceneObject.h"
#include "SceneManager.h"
#include "../Physics/PhysicsManager.hpp"

enum WheelSide
{
    WS_RIGHT = 0,
    WS_LEFT
};


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
typedef std::vector<Door*> DoorList;

class Bus : virtual public RefCounter
{
    public:
        Bus(SceneManager* smgr, PhysicsManager* pmgr, std::string filename);
        virtual ~Bus();

        SceneObject* getSceneObject() { return _sceneObject; }

        void turnLeft();
        void turnRight();
        void accelerate();
        void idle();
        void brakeOn();
        void brakeOff();

        void openDoor(unsigned char doorIndex);
        void closeDoor(unsigned char doorIndex);
        Door* getDoor(unsigned char doorIndex) { return _doors[doorIndex]; };


    private:
        SceneObject*    _sceneObject;
        PhysicalBodyConvexHull* _chasisBody;
        SceneManager*   _sMgr;
        PhysicsManager* _pMgr;

        WheelList       _wheels;
        DoorList        _doors;

        bool    _accelerate;
        bool    _brake;
        bool    _idle;

        float   _brakeForce;
        float   _brakeForceStep;

        float   _maxSteerAngle;
        float   _steerStep;

        int _wheelCollisionMask;
        int _chasisCollisionMask;

        void loadXMLdata(std::string busname);
        void updatePhysics();
};


#endif // BUS_H_INCLUDED
