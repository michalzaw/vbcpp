#include "BusRaycast.h"


BusRaycast::BusRaycast(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename)
    : _sMgr(smgr), _pMgr(pmgr), _sndMgr(sndMgr),
    _maxSteerAngle(0.55f), _steerAngle(0.0f),
    _steeringWheelObject(NULL),
    _brake(false), _accelerate(false), _handbrake(true), _idle(true)
{
    _modules.push_back(BusRayCastModule());

    _modules[0].sceneObject = _sMgr->addSceneObject("busRayCast");
    _modules[0].sceneObject->setPosition(0,5,0);

    RModel* busModel = ResourceManager::getInstance().loadModel("Buses\\h9\\h9.3ds", "Buses\\h9\\PKS\\");
    GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), _modules[0].sceneObject);

    PhysicalBodyRaycastVehicle* v = _pMgr->createPhysicalBodyRayCastVehicle(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), 800.0f, COL_BUS, COL_TERRAIN | COL_ENV);
    v->setWheelCollisionFilter(COL_WHEEL, COL_TERRAIN | COL_ENV);
    v->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );
    _modules[0].sceneObject->addComponent(v);
    _modules[0].rayCastVehicle = v;

    _wheels.push_back(new BusRayCastWheel());
    _wheels.push_back(new BusRayCastWheel());
    _wheels.push_back(new BusRayCastWheel());
    _wheels.push_back(new BusRayCastWheel());

    _wheels[0]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(1.0f, -0.45f, 2.6f), 0.6f, 0.5, true);
    _wheels[1]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(-1.0f, -0.45f, 2.6f), 0.6f, 0.5, true);
    _wheels[2]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(1.0f, -0.45f, -1.85f), 0.6f, 0.5, false);
    _wheels[3]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(-1.0f, -0.45f, -1.85), 0.6f, 0.5, false);

    _wheels[0]->steering = true;
    _wheels[1]->steering = true;
    _wheels[2]->steering = false;
    _wheels[3]->steering = false;

    _wheels[0]->powered = false;
    _wheels[1]->powered = false;
    _wheels[2]->powered = true;
    _wheels[3]->powered = true;

    _wheels[0]->handbrake = false;
    _wheels[1]->handbrake = false;
    _wheels[2]->handbrake = true;
    _wheels[3]->handbrake = true;

    _wheels[0]->maxBrakeForce = 40;
    _wheels[1]->maxBrakeForce = 40;
    _wheels[2]->maxBrakeForce = 20;
    _wheels[3]->maxBrakeForce = 20;

    for (int i = 0; i < _wheels.size(); ++i)
	{
		btWheelInfo& wheel = _wheels[i]->wheel->getWheelInfo();

		// Original
		/*wheel.m_suspensionStiffness = 20.0f;
		wheel.m_wheelsDampingCompression = 4.472f;
		wheel.m_wheelsDampingRelaxation = 3.578f;
		wheel.m_frictionSlip = 1000.0f;
		wheel.m_rollInfluence = 0.1f;*/

		wheel.m_suspensionStiffness = 35.0f;//25.0f;
		wheel.m_wheelsDampingCompression = 0.25f * 2 * sqrt(wheel.m_suspensionStiffness);
		wheel.m_wheelsDampingRelaxation = 0.3f * 2 * sqrt(wheel.m_suspensionStiffness);
		wheel.m_frictionSlip = 0.8f;
		wheel.m_rollInfluence = 0.1f;
	}

	RModel* wheelModel = ResourceManager::getInstance().loadModel("Buses\\h9\\fwheel.3ds", "Buses\\h9\\PKS\\");
	SceneObject* wheel1Object = _sMgr->addSceneObject("wheel1");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel1Object);
	wheel1Object->addComponent(_wheels[0]->wheel);

	SceneObject* wheel2Object = _sMgr->addSceneObject("wheel2");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel2Object);
	wheel2Object->addComponent(_wheels[1]->wheel);

	SceneObject* wheel3Object = _sMgr->addSceneObject("wheel3");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel3Object);
	wheel3Object->addComponent(_wheels[2]->wheel);

	SceneObject* wheel4Object = _sMgr->addSceneObject("wheel4");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel4Object);
	wheel4Object->addComponent(_wheels[3]->wheel);

	_gearbox = std::unique_ptr<Gearbox> (new Gearbox("gbx_ikarus260"));
    _engine = std::unique_ptr<Engine> (new Engine("eng_wskmielec"));

    if (_engine->getSoundFilename() != "")
    {
        SoundComponent* soundComp = new SoundComponent(_engine->getSoundFilename(), EST_PLAYER, true);
        //_sceneObject->addComponent(soundComp);
        _modules[0].sceneObject->addComponent(soundComp);
        soundComp->setGain(_engine->getSoundVolume());

        _sndMgr->addSoundComponent(soundComp);
    }

    _driverPosition = glm::vec3(0.75f, 0.44f, 3.51f);
}


BusRaycast::~BusRaycast()
{

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
            _steeringWheelObject->rotate(0.0f, -dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
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
            _steeringWheelObject->rotate(0.0f, dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
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
            _steeringWheelObject->rotate(0.0f, dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }
    else if (_steerAngle < -DEAD_ZONE)
    {
        _steerAngle += dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, -dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
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

        //SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
        sndC->stop();
    }
}



void BusRaycast::doorOpenClose(char doorGroup)
{

}


Door* BusRaycast::getDoor(unsigned char doorIndex)
{

}


void BusRaycast::update(float deltaTime)
{
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
}
