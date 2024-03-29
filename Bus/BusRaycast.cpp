#include "BusRaycast.h"

#include "../Scene/ClickableObject.h"

#include "../Game/GameEnvironment.h"

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
    _engineSoundsObject(NULL),
    _desktop(NULL), _desktopRenderObject(NULL), _desktopClickableObject(NULL)
{
	_displayText.head = "";
	_displayText.line1 = "virtual bus";
	_displayText.line2 = "";
	_displayText.type = ONE_LINE;
}


BusRaycast::~BusRaycast()
{
    LOG_INFO("Bus Destruktor");

    SceneManager* sceneManager = _modules[0].sceneObject->getSceneManager();

    if (_engineSoundsObject != NULL)
    {
        sceneManager->removeSceneObject(_engineSoundsObject);
    }

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


float BusRaycast::getSoundVolume(const SoundDefinition& soundDefinition, bool isCameraInBus)
{
    if ((soundDefinition.audibilityType == AT_INTERIOR && !isCameraInBus) ||
        (soundDefinition.audibilityType == AT_EXTERIOR && isCameraInBus))
    {
        return 0.0f;
    }

    float volume = soundDefinition.volume;

    for (const auto& curve : soundDefinition.volumeCurves)
    {
        float variableValue = 0.0f;
        if (curve.variable == SVCV_THROTTLE)
        {
            variableValue = _engine->getThrottle();
        }
        else if (curve.variable == SVCV_RPM)
        {
            variableValue = _engine->getCurrentRPM();
        }

        volume *= getValueFromCurveInPoint(curve.points, variableValue);
    }

    return volume;
}


bool BusRaycast::isCurrentCameraInBus()
{
    for (const auto& busModule : _modules)
    {
        GraphicsManager* graphicsManager = busModule.sceneObject->getSceneManager()->getGraphicsManager();
        CameraStatic* camera = graphicsManager->getCurrentCamera();
        const glm::vec3 cameraPosition = camera->getPosition();

        PhysicalBody* busModulePhysicalObject = static_cast<PhysicalBody*>(busModule.sceneObject->getComponent(CT_PHYSICAL_BODY));

        PhysicsManager* physcsManager = busModule.sceneObject->getSceneManager()->getPhysicsManager();
        bool isCameraInBusModule = physcsManager->isPointInObject(cameraPosition, busModulePhysicalObject);
        if (isCameraInBusModule)
        {
            return true;
        }
    }

    return false;
}


bool BusRaycast::isSoundPlay(SoundTrigger trigger)
{
    for (int i = 0; i < _engineSounds.size(); ++i)
    {
        if (_engine->getEngineSounds()[i].trigger == trigger)
            return _engineSounds[i]->getState() == AL_PLAYING;
    }
    return false;
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
		if (wheel->tag)
		{
			wheel->wheel->setSteeringValue(_steerAngle * -0.5);
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
		if (wheel->tag)
		{
			wheel->wheel->setSteeringValue(_steerAngle * -0.5);
		}
    }
}


void BusRaycast::centerSteringWheel(float dt)
{
    if (_steerAngle > DEAD_ZONE)
    {
        _steerAngle -= dt * STEER_STEP;
        _steerAngle = clamp(_steerAngle, 0.0f, _maxSteerAngle);

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * -dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }
    else if (_steerAngle < -DEAD_ZONE)
    {
        _steerAngle += dt * STEER_STEP;
        _steerAngle = clamp(_steerAngle, -_maxSteerAngle, 0.0f);

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
		if (wheel->tag)
		{
			wheel->wheel->setSteeringValue(_steerAngle * -0.5);
		}
    }
}


void BusRaycast::accelerate(float dt)
{
    _accelerate = true;
    _brake = false;

    _engine->throttleUp(dt);
}


void BusRaycast::idle(float dt)
{
    _accelerate = false;
    _brake = false;

    _engine->throttleDown(dt);
}


void BusRaycast::brakeOn(float dt)
{
    _accelerate = false;
    _brake = true;

    _engine->throttleDown(dt);
}


void BusRaycast::brakeOff()
{
    _brake = false;
}


void BusRaycast::toggleHandbrake()
{
    _handbrake = !_handbrake;
}


bool BusRaycast::getHandbrakeState()
{
    return _handbrake;
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
        bool existStartEngineSound = false;
        for (int i = 0; i < _engineSounds.size(); ++i)
        {
            if (_engine->getEngineSounds()[i].trigger == ST_START_ENGINE)
            {
                _engineSounds[i]->play();
                _engine->setState(ES_STARTING);

                existStartEngineSound = true;
            }
        }

		if (!existStartEngineSound)
		{
            for (int i = 0; i < _engineLoopedSounds.size(); ++i)
            {
                _engineLoopedSounds[i]->play();
            }

			_engine->setState(ES_RUN);
		}
    }
}


void BusRaycast::stopEngine()
{
    if (_engine)
    {
        for (int i = 0; i < _engineLoopedSounds.size(); ++i)
        {
            _engineLoopedSounds[i]->stop();
        }

        bool existStopEngineSound = false;
        for (int i = 0; i < _engineSounds.size(); ++i)
        {
            if (_engine->getEngineSounds()[i].trigger == ST_STOP_ENGINE)
            {
                _engineSounds[i]->play();
                _engine->setState(ES_STOPPING);

                existStopEngineSound = true;
            }
        }
		
        if (!existStopEngineSound)
		{
			_engine->setState(ES_OFF);
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
    _engine->update(deltaTime);

	if (_engine->getState() == ES_STARTING && !isSoundPlay(ST_START_ENGINE))
	{
		_engine->setState(ES_RUN);

        for (int i = 0; i < _engineLoopedSounds.size(); ++i)
        {
            _engineLoopedSounds[i]->play();
        }
	}
	else if (_engine->getState() == ES_STOPPING && !isSoundPlay(ST_STOP_ENGINE))
	{
		_engine->setState(ES_OFF);
	}

    bool isCameraInBus = isCurrentCameraInBus();

    for (int i = 0; i < _engineLoopedSounds.size(); ++i)
    {
        _engineLoopedSounds[i]->setGain(getSoundVolume(_engine->getEngineLoopedSounds()[i], isCameraInBus));
        if (_engine->getEngineLoopedSounds()[i].rpm > 0.0f)
        {
            _engineLoopedSounds[i]->setPitch(_engine->getCurrentRPM() / _engine->getEngineLoopedSounds()[i].rpm);
        }
    }
    for (int i = 0; i < _engineSounds.size(); ++i)
    {
        if (_engineSounds[i] != nullptr)
        {
            _engineSounds[i]->setGain(getSoundVolume(_engine->getEngineSounds()[i], isCameraInBus));
        }
    }

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
                //wheel->wheel->applyEngineForce(_gearbox->currentRatio() * _engine->getCurrentTorque() * 0.32f);;
				wheel->wheel->applyEngineForce(_gearbox->currentRatio() * _engine->getCurrentTorque() * _engine->getDifferentialRatio() * 0.95f * 0.1f);; // * 0.1f because, the bus mass is 10 x smaller than than real mass
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

    GameEnvironment::Variables::floatVaribles[GameEnvironment::PredefinedVariables::BUS_SPEED] = getBusSpeed();
    GameEnvironment::Variables::floatVaribles[GameEnvironment::PredefinedVariables::BUS_ENGINE_RPM] = _engine->getCurrentRPM();

    if (_desktop != NULL)
    {
        _desktop->update(deltaTime);
    }
}


void replaceMaterialsByNameInSceneObject(SceneObject* sceneObject, std::vector<Material*>& altMaterials)
{
    Component* renderObject = sceneObject->getComponent(CT_RENDER_OBJECT);
    if (renderObject != nullptr)
    {
        static_cast<RenderObject*>(renderObject)->replaceMaterialsByName(altMaterials);
    }

    for (SceneObject* child : sceneObject->getChildren())
    {
        replaceMaterialsByNameInSceneObject(child, altMaterials);
    }
}


void BusRaycast::replaceMaterialsByName(std::vector<Material*>& altMaterials)
{
    for (const BusRayCastModule& busModule : _modules)
    {
        replaceMaterialsByNameInSceneObject(busModule.sceneObject, altMaterials);
    }

    for (Door* door : _doors)
    {
        replaceMaterialsByNameInSceneObject(door->getSceneObject(), altMaterials);
    }
}


void BusRaycast::doorOpen(char door)
{
    if (_doors[door]->getState() == EDS_CLOSING)
    {
        _doors[door]->open();
    }
}


void BusRaycast::doorClose(char door)
{
    if (_doors[door]->getState() == EDS_OPENING)
    {
        _doors[door]->close();

        if (isAllDoorClosed())
            setRandomNumberOfPassengersGettingOff();
    }
}


void BusRaycast::doorOpenClose(char door)
{
    if (_doors[door]->getState() == EDS_CLOSING)
    {
        _doors[door]->open();
    }
    else if (_doors[door]->getState() == EDS_OPENING)
    {
        _doors[door]->close();

        if (isAllDoorClosed())
            setRandomNumberOfPassengersGettingOff();
    }
}


void BusRaycast::doorGroupOpen(char doorGroup)
{
    for (unsigned char i = 0; i < _doors.size(); i++)
    {
        if (_doors[i]->getGroup() == doorGroup)
        {
            if (_doors[i]->getState() == EDS_CLOSING)
            {
                _doors[i]->open();
            }
        }
    }
}


void BusRaycast::doorGroupClose(char doorGroup)
{
    for (unsigned char i = 0; i < _doors.size(); i++)
    {
        if (_doors[i]->getGroup() == doorGroup)
        {
            if (_doors[i]->getState() == EDS_OPENING)
            {
                _doors[i]->close();

                if (isAllDoorClosed())
                    setRandomNumberOfPassengersGettingOff();
            }
        }
    }
}


void BusRaycast::doorGroupOpenClose(char doorGroup)
{
    for (unsigned char i = 0; i < _doors.size(); i++)
    {
        if (_doors[i]->getGroup() == doorGroup)
        {
            if (_doors[i]->getState() == EDS_CLOSING)
            {
                _doors[i]->open();
            }
            else if (_doors[i]->getState() == EDS_OPENING)
            {
                _doors[i]->close();

                if (isAllDoorClosed())
                    setRandomNumberOfPassengersGettingOff();
            }
        }
    }
}


void BusRaycast::doorBlock(char door)
{
    _doors[door]->block();
}


void BusRaycast::doorUnblock(char door)
{
    _doors[door]->unblock();
}
