#include "BusRaycast.h"

#include "../Scene/ClickableObject.h"

#include "../Utils/Math.h"

#include "../Graphics/GraphicsManager.h"
#include "../Graphics/LoadTexture.h"
#include "../Utils/ResourceManager.h"

#include "../Utils/Helpers.hpp"
#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"
#include "../Utils/Logger.h"


BusRaycast::BusRaycast()
    : _maxSteerAngle(0.55f), _steerAngle(0.0f),
    _brake(false), _accelerate(false), _handbrake(true), _idle(true),
    _isEnableLights(false), _isEnableHeadlights(false),
    _steeringWheelObject(NULL), _desktopObject(NULL), _driverPosition(0.0f, 0.0f, 0.0f),
    _desktop(NULL), _desktopRenderObject(NULL), _desktopClickableObject(NULL)
{
	_displayText.head = "0";
	_displayText.line1 = "katowice";
	_displayText.line2 = "dworzec pkp";
	_displayText.type = TWO_LINE;

}


BusRaycast::~BusRaycast()
{
    Logger::info("Bus Destruktor");

    SceneManager* sceneManager = _modules[0].sceneObject->getSceneManager();

	for (BusRayCastWheel* wheel : _wheels)
	{
		sceneManager->removeSceneObject(wheel->wheel->getSceneObject());
		delete wheel;
	}

	for (Door* door : _doors)
	{
		delete door;
	}

	for (std::pair<DisplayComponent*, BusDisplayType>& display : _displays)
	{
		sceneManager->removeSceneObject(display.first->getSceneObject());
	}

	if (_desktop != NULL)
	{
		delete _desktop;
	}

    for (BusRayCastModule& module : _modules)
    {
        sceneManager->removeSceneObject(module.sceneObject);
    }
}


bool BusRaycast::isAllDoorClosed()
{
    for (unsigned char i = 0; i < _doors.size(); i++)
    {
        if (_doors[i]->getState() == EDS_OPENING)
            return false;
    }
    return true;
}

void BusRaycast::setRandomNumberOfPassengersGettingOff()
{
    if (_numberOfPassengers != 0)
        _numberOfPassengersGettingOff = rand() % _numberOfPassengers;
    else
        _numberOfPassengersGettingOff = 0;
}


void BusRaycast::catchInputFromDesktop()
{
    if (_desktopClickableObject != NULL && _desktopClickableObject->isClicked())
    {
        if (isVectorContains(_desktopClickableObject->getClickedNodes(), _desktop->getButton(DBT_DOOR_1).modelNode))
        {
            doorOpenClose(1);
        }
        if (isVectorContains(_desktopClickableObject->getClickedNodes(), _desktop->getButton(DBT_DOOR_2).modelNode))
        {
            doorOpenClose(2);
        }
    }
}


BusRayCastModule& BusRaycast::getModule(int index)
{
    return _modules[index];
}


glm::vec3 BusRaycast::getDriverPosition()
{
    return _driverPosition;
}


SceneObject* BusRaycast::getSteeringWheelObject()
{
    return _steeringWheelObject;
}


void BusRaycast::setIsEnableLights(bool is)
{
    if (_lights.size() > 0)
    {
        for (LightsList::iterator i = _lights.begin(); i != _lights.end(); ++i)
        {
            (*i)->setIsActive(is);
        }
        _isEnableLights = is;
    }
}


bool BusRaycast::isEnableLights()
{
    return _isEnableLights;
}


void BusRaycast::setIsEnableHeadlights(bool is)
{
    if (_headlights.size() > 0)
    {
        for (LightsList::iterator i = _headlights.begin(); i != _headlights.end(); ++i)
        {
            (*i)->setIsActive(is);
        }

        _isEnableHeadlights = is;
    }

    _desktop->setDesktopBacklightingState(is);
}


bool BusRaycast::isEnableHeadlights()
{
    return _isEnableHeadlights;
}


void BusRaycast::turnLeft(float dt)
{
    if (_steerAngle < _maxSteerAngle)
    {
        _steerAngle += dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }

    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->steering )
        {
            wheel->wheel->setSteeringValue(_steerAngle);
        }
    }
}


void BusRaycast::turnRight(float dt)
{
    if (_steerAngle > -_maxSteerAngle)
    {
        _steerAngle -= dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * -dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }

    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->steering )
        {
            wheel->wheel->setSteeringValue(_steerAngle);
        }
    }
}


void BusRaycast::centerSteringWheel(float dt)
{
    if (_steerAngle > DEAD_ZONE)
    {
        _steerAngle -= dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * -dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }
    else if (_steerAngle < -DEAD_ZONE)
    {
        _steerAngle += dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }

    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->steering )
        {
            wheel->wheel->setSteeringValue(_steerAngle);
        }
    }
}


void BusRaycast::accelerate()
{
    _accelerate = true;
    _brake = false;

    _engine->throttleUp();
}


void BusRaycast::idle()
{
    _accelerate = false;
    _brake = false;

    _engine->throttleDown();
}


void BusRaycast::brakeOn()
{
    _accelerate = false;
    _brake = true;

    _engine->throttleDown();
}


void BusRaycast::brakeOff()
{
    _brake = false;
}


void BusRaycast::toggleHandbrake()
{
    _handbrake = !_handbrake;
}


Gearbox* BusRaycast::getGearbox()
{
    if (_gearbox) return _gearbox.get(); else return 0;
}


Engine* BusRaycast::getEngine()
{
    if (_engine) return _engine.get(); else return 0;
}


void BusRaycast::startEngine()
{
    if (_engine)
    {
        _engine->turnOn();

        //SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
        sndC->play();
    }
}


void BusRaycast::stopEngine()
{
    if (_engine)
    {
        _engine->turnOff();

        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
        sndC->stop();
    }
}



void BusRaycast::doorOpenClose(char doorGroup)
{
    _desktop->clickButton((DesktopButtonType)(DBT_DOOR_1 + doorGroup - 1));

    for (unsigned char i = 0; i < _doors.size(); i++)
    {
        if (_doors[i]->getGroup() == doorGroup)
        {
            if (_doors[i]->getState() == EDS_CLOSING)
            {
                _doors[i]->open();

                _desktop->setLightState((DesktopLightType)(DLT_DOOR_1 + doorGroup - 1), true);
            }
            else
            if (_doors[i]->getState() == EDS_OPENING)
            {
                _doors[i]->close();

                _desktop->setLightState((DesktopLightType)(DLT_DOOR_1 + doorGroup - 1), false);

                if (isAllDoorClosed())
                    setRandomNumberOfPassengersGettingOff();
            }
        }
    }
}


Door* BusRaycast::getDoor(unsigned char doorIndex)
{
    if (doorIndex <= _doors.size()-1)
        return _doors[doorIndex];
    else
        return 0;
}


int BusRaycast::getDoorsCount()
{
    return _doors.size();
}


MirrorComponent* BusRaycast::getMirror(int index)
{
    if (index <= _mirrors.size()-1)
        return _mirrors[index];
    else
        return NULL;
}


int BusRaycast::getMirrorsCount()
{
    return _mirrors.size();
}


DisplayText& BusRaycast::getDisplayText()
{
	return _displayText;
}


void BusRaycast::updateDisplays()
{
	for (int i = 0; i < _displays.size(); ++i)
	{
		BusDisplayType type = _displays[i].second;
		if (type == BDT_LINE_AND_DIRECTION)
		{
			_displays[i].first->setText(_displayText);
		}
		else
		{
			DisplayText text;
			text.line1 = _displayText.head;
			text.type = ONE_LINE;

			_displays[i].first->setText(text);
		}
	}
}


SceneObject* BusRaycast::getDesktopObject()
{
    return _desktopObject;
}


float BusRaycast::getBusSpeed()
{
    return _modules[0].rayCastVehicle->getRayCastVehicle()->getCurrentSpeedKmHour();
}


void BusRaycast::update(float deltaTime)
{
    catchInputFromDesktop();

    _engine->update(deltaTime);

    SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
    sndC->setPitch(_engine->getCurrentRPM() / 1000);

    btScalar wheelAngularVelocity = 0.0f;

    // przyspieszanie
    if (_accelerate)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            if ( wheel->powered )
            {
                wheel->wheel->applyEngineForce(_gearbox->currentRatio() * _engine->getCurrentTorque() * 0.32f);;
            }
        }
    }
    // bus is braking
    else if (_brake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            wheel->wheel->applyEngineForce(0.0f);
            wheel->wheel->setBrake(wheel->maxBrakeForce);
        }
    } // bus engine is idle
    else if (!_accelerate && !_brake && !_handbrake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            wheel->wheel->applyEngineForce(0.0f);
            wheel->wheel->setBrake(0.13f * abs(_gearbox->currentRatio()) ); // we set a little damping to simulate air and rolling resistance
        }
    }
    else if (!_handbrake && !_brake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            wheel->wheel->setBrake(0.0f);
        }
    } // bus is on handbrake
    if (_handbrake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            if (wheel->handbrake)
            {
                wheel->wheel->setBrake(wheel->maxBrakeForce);
            }
        }
    }

    char poweredWheels = 0;

    // If we do not accelerate at the moment - we have to calculate and simulate engine braking
    // Get wheel linear speed to calculate RPM of the engine
    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->powered )
        {
            wheelAngularVelocity += wheel->wheel->getWheelInfo().m_deltaRotation / deltaTime;
            poweredWheels++;
            //break;
        }
    }

    _engine->setRPM(wheelAngularVelocity/poweredWheels, _gearbox->currentRatio());

    if (_desktop != NULL)
    {
        _desktop->setIndicatorValue(DIT_SPEEDOMETER, getBusSpeed());
        _desktop->setIndicatorValue(DIT_TACHOMETER, _engine->getCurrentRPM());

        _desktop->update(deltaTime);
    }
}
