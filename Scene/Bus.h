#ifndef BUS_H_INCLUDED
#define BUS_H_INCLUDED

#include "../Utils/RefCounter.h"
#include "../Utils/Strings.h"

#include "SceneObject.h"
#include "SceneManager.h"
#include "../Physics/PhysicsManager.hpp"

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
typedef std::vector<Light*> LightsList;

class Bus : virtual public RefCounter
{
    public:
        Bus(SceneManager* smgr, PhysicsManager* pmgr, std::string filename);
        virtual ~Bus();

        SceneObject* getSceneObject() { return _sceneObject; }
        SceneObject* getSteeringWheelObject() { return _steeringWheelObject; }

        glm::vec3 getDriverPosition();
        void setIsEnableLights(bool is);
        bool isEnableLights();
        void setIsEnableHeadlights(bool is);
        bool isEnableHeadlights();

        void turnLeft();
        void turnRight();
        void accelerate();
        void idle();
        void brakeOn();
        void brakeOff();



    private:
        SceneObject*    _sceneObject;
        PhysicalBodyConvexHull* _chasisBody;
        SceneManager*   _sMgr;
        PhysicsManager* _pMgr;

        SceneObject*    _steeringWheelObject;
        glm::vec3       _driverPosition;
        LightsList      _lights;
        bool            _isEnableLights;
        Light*          _headlights[2];
        bool            _isEnableHeadlights;

        WheelList       _wheels;

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
