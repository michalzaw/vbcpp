#include "BusRaycast.h"


BusRaycast::BusRaycast(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename)
    : _sMgr(smgr), _pMgr(pmgr), _sndMgr(sndMgr),
    _wheelAngle(0.0f)
{
    _busSceneObject = _sMgr->addSceneObject("busRayCast");
    _busSceneObject->setPosition(0,5,0);

    RModel* busModel = ResourceManager::getInstance().loadModel("Buses\\h9\\h9.3ds", "Buses\\h9\\PKS\\");
    GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), _busSceneObject);

    //PhysicalBodyBox* physicalBody = _pMgr->createPhysicalBodyBox(btVector3(1.5f, 1.0f, 6.0f), 800.0f, COL_BUS, COL_TERRAIN | COL_ENV);
    PhysicalBody* physicalBody = _pMgr->createPhysicalBodyConvexHull(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), 800.0f, COL_BUS, COL_TERRAIN | COL_ENV);
    physicalBody->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );
    _busSceneObject->addComponent(physicalBody);

    btRaycastVehicle::btVehicleTuning vehicleTuning;
    btVehicleRaycaster* rayCaster = new btDefaultVehicleRaycaster(_pMgr->getDynamicsWorld());
    _bulletVehicle = new btRaycastVehicle(vehicleTuning, physicalBody->getRigidBody(), rayCaster);
    _bulletVehicle->setCoordinateSystem(0, 1, 2);
    _pMgr->getDynamicsWorld()->addVehicle(_bulletVehicle);

    _bulletVehicle->addWheel(btVector3(1.0f, -0.45f, 2.6f), btVector3(0.0f, -1.0f, 0.0f),
                             btVector3(-1.0f, 0.0f, 0.0f), 0.6f, 0.5, vehicleTuning, true);
    _bulletVehicle->addWheel(btVector3(-1.0f, -0.45f, 2.6f), btVector3(0.0f, -1.0f, 0.0f),
                             btVector3(-1.0f, 0.0f, 0.0f), 0.6f, 0.5, vehicleTuning, true);
    _bulletVehicle->addWheel(btVector3(1.0f, -0.45f, -1.85f), btVector3(0.0f, -1.0f, 0.0f),
                             btVector3(-1.0f, 0.0f, 0.0f), 0.6f, 0.5, vehicleTuning, false);
    _bulletVehicle->addWheel(btVector3(-1.0f, -0.45f, -1.85), btVector3(0.0f, -1.0f, 0.0f),
                             btVector3(-1.0f, 0.0f, 0.0f), 0.6f, 0.5, vehicleTuning, false);

    for (int i = 0; i < _bulletVehicle->getNumWheels(); ++i)
	{
		btWheelInfo& wheel = _bulletVehicle->getWheelInfo(i);

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
	wheel1Object->addComponent(pmgr->createPhysicalBodyWheel(new PhysicalBodyWheel(_bulletVehicle, 0)));

	SceneObject* wheel2Object = _sMgr->addSceneObject("wheel2");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel2Object);
	wheel2Object->addComponent(pmgr->createPhysicalBodyWheel(new PhysicalBodyWheel(_bulletVehicle, 1)));

	SceneObject* wheel3Object = _sMgr->addSceneObject("wheel3");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel3Object);
	wheel3Object->addComponent(pmgr->createPhysicalBodyWheel(new PhysicalBodyWheel(_bulletVehicle, 2)));

	SceneObject* wheel4Object = _sMgr->addSceneObject("wheel4");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel4Object);
	wheel4Object->addComponent(pmgr->createPhysicalBodyWheel(new PhysicalBodyWheel(_bulletVehicle, 3)));
}


BusRaycast::~BusRaycast()
{

}
