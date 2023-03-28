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

    float dampingCompression;
    float dampingRelaxation;
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

        glm::vec3 getDriverPosition() override;
        SceneObject* getSteeringWheelObject() override;

        void setIsEnableLights(bool is) override;
        bool isEnableLights() override;
        void setIsEnableHeadlights(bool is) override;
        bool isEnableHeadlights() override;

        void turnLeft(float dt) override;
        void turnRight(float dt) override;
        void centerSteringWheel(float dt) override;
        void accelerate(float dt) override;
        void idle(float dt) override;
        void brakeOn(float dt) override;
        void brakeOff() override;
        void toggleHandbrake() override;
        bool getHandbrakeState() override;

        Gearbox* getGearbox() override;
        Engine* getEngine() override;
        void startEngine() override;
        void stopEngine() override;

        BusRayCastWheel* getWheel(unsigned char wheelIndex);
        int getWheelsCount();

        Door* getDoor(unsigned char doorIndex) override;
        int getDoorsCount() override;

        MirrorComponent* getMirror(int index) override;
        int getMirrorsCount() override;

		DisplayText& getDisplayText() override;
		void updateDisplays() override;

        SceneObject* getDesktopObject() override;

        float getBusSpeed() override;

        void update(float deltaTime) override;

        void replaceMaterialsByName(std::vector<Material*>& altMaterials) override;

        void doorOpen(char door);
        void doorClose(char door);
        void doorOpenClose(char door);
        void doorGroupOpen(char doorGroup);
        void doorGroupClose(char doorGroup);
        void doorGroupOpenClose(char doorGroup) override;
        void doorBlock(char door);
        void doorUnblock(char door);

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

        SceneObject* _engineSoundsObject;
        std::vector<SoundComponent*> _engineLoopedSounds;
        std::vector<SoundComponent*> _engineSounds;

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

        float getSoundVolume(const SoundDefinition& soundDefinition, bool isCameraInBus);

        bool isCurrentCameraInBus();

        bool isSoundPlay(SoundTrigger trigger);
};


#endif // BUSRAYCAST_H_INCLUDED
