#ifndef BUS_H_INCLUDED
#define BUS_H_INCLUDED


#include <vector>

#include "../Utils/RefCounter.h"

#include "../Utils/Gearbox.h"
#include "../Utils/Engine.h"
#include "../Utils/LocalizationSystem.h"

#include "../Scene/SceneObject.h"

#include "../Graphics/Light.h"
#include "../Graphics/MirrorComponent.h"
#include "../Graphics/DisplayComponent.h"

#include "BusDescription.h"
#include "Door.h"


typedef std::vector<Door*>  DoorList;
typedef std::vector<Light*> LightsList;


enum WheelSide
{
    WS_RIGHT = 0,
    WS_LEFT
};


class Bus : public RefCounter
{

    protected:
        unsigned int _numberOfPassengers;
        unsigned int _numberOfPassengersGettingOff;
        SoundComponent* _announcementSource;
		SoundComponent* _engineSoundSource;
		SoundComponent* _engineStartSoundSource;
		SoundComponent* _engineStopSoundSource;

        BusDescription _busDescription;

        LocalizationSystem _texts;

    public:
        Bus()
        {
            _numberOfPassengers = 0;
            _numberOfPassengersGettingOff = 0;

			_announcementSource = nullptr;
			_engineSoundSource = nullptr;
			_engineStartSoundSource = nullptr;
			_engineStopSoundSource = nullptr;
        }

        virtual SceneObject* getSceneObject() = 0;

        virtual glm::vec3 getDriverPosition() = 0;
        virtual SceneObject* getSteeringWheelObject() = 0;

        virtual void setIsEnableLights(bool is) = 0;
        virtual bool isEnableLights() = 0;
        virtual void setIsEnableHeadlights(bool is) = 0;
        virtual bool isEnableHeadlights() = 0;

        virtual void turnLeft(float dt) = 0;
        virtual void turnRight(float dt) = 0;
        virtual void centerSteringWheel(float dt) = 0;
        virtual void accelerate(float dt) = 0;
        virtual void idle(float dt) = 0;
        virtual void brakeOn(float dt) = 0;
        virtual void brakeOff() = 0;
        virtual void toggleHandbrake() = 0;

		virtual bool getHandbrakeState() = 0;

        virtual Gearbox* getGearbox() = 0;
        virtual Engine* getEngine() = 0;
        virtual void startEngine() = 0;
        virtual void stopEngine() = 0;

        virtual void doorGroupOpenClose(char doorGroup) = 0;
        virtual Door* getDoor(unsigned char doorIndex) = 0;
        virtual int getDoorsCount() = 0;

        virtual MirrorComponent* getMirror(int index) = 0;
        virtual int getMirrorsCount() = 0;

		virtual DisplayText& getDisplayText() = 0;
		virtual void updateDisplays() = 0;

        virtual SceneObject* getDesktopObject() = 0;

        virtual float getBusSpeed() = 0;

        virtual void update(float deltaTime) = 0;

        virtual void replaceMaterialsByName(std::vector<Material*>& altMaterials)
        {

        }

        void setNumberOfPassengers(unsigned int number)
        {
            _numberOfPassengers = number;
        }

        unsigned int getNumberOfPassengers()
        {
            return _numberOfPassengers;
        }

        void setNumberOfPassengersGettingOff(unsigned int number)
        {
            _numberOfPassengersGettingOff = number;
        }

        unsigned int getNumberOfPassengersGettingOff()
        {
            return _numberOfPassengersGettingOff;
        }

        SoundComponent* getAnnouncementSource()
        {
            return _announcementSource;
        }

		SoundComponent* getEngineSoundSource()
		{
			return _engineSoundSource;
		}

		SoundComponent* getEngineStartSoundSource()
		{
			return _engineStartSoundSource;
		}

		SoundComponent* getEngineStopSoundSource()
		{
			return _engineStopSoundSource;
		}

        BusDescription& getBusDescription()
        {
            return _busDescription;
        }

        LocalizationSystem& getTexts()
        {
            return _texts;
        }

        std::string getText(const std::string& textValueOrId)
        {
            if (startsWith(textValueOrId, "$"))
            {
                return _texts.getText(textValueOrId.substr(1));
            }

            return textValueOrId;
        }

};


#endif // BUS_H_INCLUDED
