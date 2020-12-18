#ifndef BUSRAYCAST_H_INCLUDED
#define BUSRAYCAST_H_INCLUDED


#include <string>
#include <unordered_map>

#include "Bus.h"
#include "DoorSimple.h"
#include "DoorSE.h"
#include "DoorClassic.h"
#include "Desktop.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SoundManager.h"

#include "../Physics/PhysicsManager.hpp"
#include "../Physics/PhysicalBodyWheel.h"
#include "../Physics/PhysicalBodyRaycastVehicle.h"


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
	bool                  tag;
    bool                  powered;
    bool                  handbrake;
    float                 maxBrakeForce;
    WheelSide             wheelSide;
};


enum BusDisplayType
{
	BDT_LINE_AND_DIRECTION,
	BDT_LINE
};


class BusRaycast : public Bus
{
    friend class BusLoader;

    static constexpr float STEER_STEP = 0.3f;
    static constexpr float DEAD_ZONE = 0.0025f;

    public:
        BusRaycast();
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

		virtual bool getHandbrakeState()
		{
			return _handbrake;
		}

        virtual Gearbox* getGearbox();
        virtual Engine* getEngine();
        virtual void startEngine();
        virtual void stopEngine();

        virtual void doorOpenClose(char doorGroup);
        virtual Door* getDoor(unsigned char doorIndex);
        virtual int getDoorsCount();

        virtual MirrorComponent* getMirror(int index);
        virtual int getMirrorsCount();

		virtual DisplayText& getDisplayText();
		virtual void updateDisplays();

        virtual SceneObject* getDesktopObject();

        virtual float getBusSpeed();

        virtual void update(float deltaTime);

    private:
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

        std::vector<MirrorComponent*> _mirrors;

		DisplayText _displayText;
		std::vector<std::pair<DisplayComponent*, BusDisplayType>> _displays;

        Desktop* _desktop;
        RenderObject* _desktopRenderObject;
        ClickableObject* _desktopClickableObject;

        bool isAllDoorClosed();
        void setRandomNumberOfPassengersGettingOff();

        void catchInputFromDesktop();
};


#endif // BUSRAYCAST_H_INCLUDED
