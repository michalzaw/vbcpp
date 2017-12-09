#ifndef BUS_H_INCLUDED
#define BUS_H_INCLUDED


#include <vector>

#include "../Utils/RefCounter.h"

#include "../Utils/Gearbox.h"
#include "../Utils/Engine.h"

#include "../Scene/SceneObject.h"

#include "../Graphics/Light.h"

#include "Door.h"


class Bus : public RefCounter
{
    public:
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
        virtual void accelerate() = 0;
        virtual void idle() = 0;
        virtual void brakeOn() = 0;
        virtual void brakeOff() = 0;
        virtual void toggleHandbrake() = 0;

        virtual Gearbox* getGearbox() = 0;
        virtual Engine* getEngine() = 0;
        virtual void startEngine() = 0;
        virtual void stopEngine() = 0;

        virtual void doorOpenClose(char doorGroup) = 0;
        virtual Door* getDoor(unsigned char doorIndex) = 0;

        virtual void update(float deltaTime) = 0;

};


#endif // BUS_H_INCLUDED
