#include "MapView.h"

// nie do konca wiem po co to tutaj, ale bez tego jest blad ze gl jest includowane przed glew
#include <GL/glew.h>

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>

#include "../../Bus/BusRaycast.h"

#include "../../Graphics/BezierCurve.h"
#include "../../Graphics/CameraStatic.hpp"
#include "../../Graphics/Framebuffer.h"
#include "../../Graphics/GraphicsManager.h"
#include "../../Graphics/OGLDriver.h"
#include "../../Graphics/RenderData.h"
#include "../../Graphics/Renderer.h"

#include "../../GUI/GUIManager.h"

#include "../../Scene/SceneManager.h"

#include "../../Utils/QuaternionUtils.h"


MapView::MapView(GUIManager* gui, SceneManager* sceneManager)
	: _gui(gui),
	_mode(MVM_NAVIGATION),
	_showPavements(false),
	_isInitialized(false)
{
	const glm::vec2& windowSize = Renderer::getInstance().getWindowDimensions();


	Framebuffer* framebuffer1 = OGLDriver::getInstance().createFramebuffer();
	framebuffer1->addTexture(TF_RGBA, windowSize.y * 0.9f, windowSize.y * 0.9f);
	framebuffer1->init();

	Framebuffer* framebuffer2 = OGLDriver::getInstance().createFramebuffer();
	framebuffer2->addTexture(TF_RGBA, windowSize.x * 0.15f, windowSize.x * 0.15f);
	framebuffer2->init();

	_framebuffers.push_back(framebuffer1);
	_framebuffers.push_back(framebuffer2);


	Image* image1 = gui->addImage(_framebuffers[0]->getTexture());
	image1->setInvertY(false);
	image1->setPosition((windowSize.x - image1->getSize().x) / 2.0f, (windowSize.y - image1->getSize().y) / 2.0f);
	image1->setIsActive(false);

	Image* image2 = gui->addImage(_framebuffers[1]->getTexture());
	image2->setInvertY(false);
	image2->setPosition(windowSize.x - image2->getSize().x, 0.0f);
	image2->setIsActive(false);

	_images.push_back(image1);
	_images.push_back(image2);

	_images[_mode]->setIsActive(true);
}


MapView::~MapView()
{
	for (Framebuffer* framebuffer : _framebuffers)
	{
		OGLDriver::getInstance().deleteFramebuffer(framebuffer);
	}

	_framebuffers.clear();


	for (Image* image : _images)
	{
		_gui->removeObject(image);
	}

	_images.clear();


	if (_isInitialized)
	{
		delete _sceneManager;

		for (RRoadProfile* profile : _profiles)
		{
			delete profile;
		}

		_profiles.clear();
	}

	
	_isInitialized = false;

}


CameraStatic* MapView::createCameraForWorldMapMode()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("mapViewCameraWorldMapMode");

	CameraStatic* camera = _sceneManager->getGraphicsManager()->addCameraStatic(CPT_ORTHOGRAPHIC);
	camera->setOrthoProjectionParams(-500.0f, 500.0f, -500.0f, 500.0f, 1000.0f, -1000.0f);
	cameraObject->addComponent(camera);

	cameraObject->setRotation(-90.0f, 0.0f, 0.0f);

	return camera;
}


CameraStatic* MapView::createCameraFormNavigationMode()
{
	SceneObject* cameraObject = _sceneManager->addSceneObject("mapViewCameraNavigationMode");

	CameraStatic* camera = _sceneManager->getGraphicsManager()->addCameraStatic(CPT_PERSPECTIVE);
	camera->setWindowDimensions(200, 200);
	camera->setViewAngle(degToRad(45.0f));
	camera->setNearValue(0.01f);
	camera->setFarValue(1000.0f);
	cameraObject->addComponent(camera);

	cameraObject->setRotation(-90.0f, 0.0f, 0.0f);

	return camera;
}


// todo: uzycie tych funkcji powoduje exception :(
RRoadProfile* MapView::createRoadProfile()
{
	Material* laneMaterial = new Material;
	laneMaterial->shader = MINIMAP_MATERIAL;
	laneMaterial->diffuseColor = glm::vec4(0.96f, 0.83f, 0.37f, 1.0f);
	laneMaterial->shininess = 96.0f;

	Material* laneIntersectionMaterial = new Material;
	laneIntersectionMaterial->shader = MINIMAP_MATERIAL;
	laneIntersectionMaterial->diffuseColor = glm::vec4(0.96f, 0.83f, 0.37f, 1.0f);
	laneIntersectionMaterial->shininess = 96.0f;

	// nie dodaje laneMaterial do _materials poniewaz zostaje on usuniety w destruktorze RoadLane
	_materials.push_back(laneIntersectionMaterial);

	RRoadProfile* roadProfile = new RRoadProfile("", "", "", "", laneIntersectionMaterial);

	RoadLane lane;
	lane.r1 = -5.5f;
	lane.r2 = 5.5f;
	lane.height1 = 0.0f;
	lane.height2 = 0.0f;
	lane.material = laneMaterial;
	roadProfile->getRoadLanes().push_back(lane);

	_profiles.push_back(roadProfile);

	return roadProfile;
}


RRoadProfile* MapView::createPavementProfile()
{
	Material* pavementMaterial = new Material;
	pavementMaterial->shader = MINIMAP_MATERIAL;
	pavementMaterial->diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	pavementMaterial->shininess = 96.0f;

	Material* pavementIntersectionMaterial = new Material;
	pavementIntersectionMaterial->shader = MINIMAP_MATERIAL;
	pavementIntersectionMaterial->diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	pavementIntersectionMaterial->shininess = 96.0f;

	// nie dodaje pavementMaterial do _materials poniewaz zostaje on usuniety w destruktorze RoadLane
	_materials.push_back(pavementIntersectionMaterial);

	RRoadProfile* pavementProfile = new RRoadProfile("", "", "", "", pavementIntersectionMaterial);

	RoadLane lane2;
	lane2.r1 = -1.0f;
	lane2.r2 = 1.0f;
	lane2.height1 = 0.0f;
	lane2.height2 = 0.0f;
	lane2.material = pavementMaterial;
	pavementProfile->getRoadLanes().push_back(lane2);

	_profiles.push_back(pavementProfile);

	return pavementProfile;
}


void MapView::setMode(MapViewMode mode)
{
	if (_mode != MVM_DISABLE)
	{
		_images[_mode]->setIsActive(false);
	}

	_mode = mode;

	if (_mode != MVM_DISABLE)
	{
		_images[_mode]->setIsActive(true);
	}
}


MapViewMode MapView::getMode()
{
	return _mode;
}


// todo: taka sama funkcja jak w SceneLoader
RoadObject* findRoadObjectBySceneObjectName(SceneManager* sceneManager, const std::string& name)
{
	SceneObject* roadSceneObject = sceneManager->getSceneObject(name);
	if (roadSceneObject != nullptr)
	{
		Component* roadObject = roadSceneObject->getComponent(CT_ROAD_OBJECT);
		if (roadObject != nullptr)
		{
			return dynamic_cast<RoadObject*>(roadObject);
		}
		else
		{
			LOG_ERROR("Cannot find Road in object: " + name);
		}
	}
	else
	{
		LOG_ERROR("Cannot find object: " + name);
	}

	return nullptr;
}


void MapView::init(SceneManager* sceneManager)
{
	GraphicsManager* graphicsManager = new GraphicsManager;
	PhysicsManager* physicsManager = new PhysicsManager;
	SoundManager* soundManager = new SoundManager();
	soundManager->setMute(true);
	_sceneManager = new SceneManager(graphicsManager, physicsManager, soundManager);

	_cameras.push_back(createCameraForWorldMapMode());
	_cameras.push_back(createCameraFormNavigationMode());


	RRoadProfile* roadProfile = createRoadProfile();
	RRoadProfile* pavementProfile = createPavementProfile();


	// drogi
	for (RoadObject* roadObject : sceneManager->getGraphicsManager()->getRoadObjects())
	{
		if (roadObject->getSceneObject()->getParent() != nullptr ||								// pomijam te ktore sa czscia skrzyzowan
			(!_showPavements) && roadObject->getRoadProfile()->getType() == RPT_PAVEMENT)		// pomijam te ktorych typ to pavement i flaga _showPavements=true
		{
			continue;
		}

		SceneObject* roadSceneObject = _sceneManager->addSceneObject(roadObject->getSceneObject()->getName());

		BezierCurve* bezierCurve = roadObject->getSceneObject()->getComponentWithCasting<BezierCurve>(CT_BEZIER_CURVE);
		if (bezierCurve != nullptr)
		{
			// todo: funkcja kopiuj¹ca krzywa beziera
			std::vector<int> segmentsPointsCount;
			for (int i = 0; i < bezierCurve->getSegmentsCount(); ++i)
			{
				segmentsPointsCount.push_back(bezierCurve->getSegmentPointsCount(i));
			}
			BezierCurve* newBezierCurve = _sceneManager->getGraphicsManager()->addBezierCurve(bezierCurve->getPoints(), segmentsPointsCount, bezierCurve->getMarginBegin(), bezierCurve->getMarginEnd(), bezierCurve->getOffsetFromBaseCurve());

			roadSceneObject->addComponent(newBezierCurve);
		}

		RRoadProfile* profile = roadObject->getRoadProfile()->getType() == RPT_ROAD ? roadProfile : pavementProfile;

		RoadObject* newRoadObject = _sceneManager->getGraphicsManager()->addRoadObject(roadObject->getRoadType(), profile, roadObject->getPoints(), roadObject->getSegments(), false, roadSceneObject);
	}

	
	// skrzyzowania
	for (RoadIntersectionComponent* roadIntersection: sceneManager->getGraphicsManager()->getRoadIntersectionComponents())
	{
		SceneObject* roadIntersectionSceneObject = _sceneManager->addSceneObject(roadIntersection->getSceneObject()->getName());
		roadIntersectionSceneObject->setPosition(roadIntersection->getSceneObject()->getPosition());

		RoadIntersectionComponent* newRoadIntersection = _sceneManager->getGraphicsManager()->addRoadIntersection(roadProfile);
		newRoadIntersection->setQuality(roadIntersection->getQuality());
		newRoadIntersection->setEdgeRoadProfileNumberOfLanesToRemove(1);

		roadIntersectionSceneObject->addComponent(newRoadIntersection);

		int newRoadIndex = 0;
		for (int i = 0; i < roadIntersection->getConnectedRoads().size(); ++i)
		{
			const RoadConnectedToIntersection& connectionPoint = roadIntersection->getConnectedRoads()[i];
			const std::string& name = connectionPoint.road->getSceneObject()->getName();

			RoadObject* connectedRoad = findRoadObjectBySceneObjectName(_sceneManager, name);
			if (connectedRoad != nullptr)
			{
				connectedRoad->setConnectionPointWithRoadIntersection(connectionPoint.connectionPointInRoadIndex, newRoadIntersection);

				newRoadIntersection->setLength(newRoadIndex, roadIntersection->getLength(i));
				newRoadIntersection->setWidth(newRoadIndex, roadIntersection->getWidth(i));
				newRoadIntersection->setArc(newRoadIndex, roadIntersection->getArc(i));

				++newRoadIndex;
			}
		}
	}


	// drogi
	for (RoadObject* roadObject : _sceneManager->getGraphicsManager()->getRoadObjects())
	{
		roadObject->buildModel();
	}

	// skrzyzowania
	for (RoadIntersectionComponent* roadIntersection : _sceneManager->getGraphicsManager()->getRoadIntersectionComponents())
	{
		roadIntersection->createPolygon();
	}


	// pojazd
	Material* material = new Material;
	material->shader = MINIMAP_MATERIAL;
	material->diffuseColor = glm::vec4(1.0f, 0.0, 0.0, 1.0f);
	material->shininess = 96.0f;

	SceneObject* cubeSceneObject = _sceneManager->addSceneObject("cube");
	Cube* cube = new Cube(8.0f, material);
	cube->init();
	_sceneManager->getGraphicsManager()->addRenderObject(cube, cubeSceneObject);


	physicsManager->drop();
	soundManager->drop();


	_isInitialized = true;
}


void MapView::update(Bus* bus)
{
	if (_mode == MVM_DISABLE)
	{
		return;
	}

	SceneObject* cubeObject = _sceneManager->getSceneObject("cube");
	cubeObject->setPosition(bus->getSceneObject()->getPosition());
	cubeObject->setRotationQuaternion(bus->getSceneObject()->getRotationQuaternion());


	if (_mode == MVM_NAVIGATION)
	{
		_cameras[_mode]->getSceneObject()->setPosition(bus->getSceneObject()->getPosition());

		PhysicalBodyRaycastVehicle* rayCastVehicle = bus->getSceneObject()->getComponentWithCasting<PhysicalBodyRaycastVehicle>(CT_PHYSICAL_BODY);
		if (rayCastVehicle != nullptr)
		{
			const btVector3& dir = rayCastVehicle->getRayCastVehicle()->getForwardVector();
			glm::vec2 busDirection = glm::normalize(glm::vec2(dir.x(), dir.z()));

			//float cosTheta = glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), glm::normalize(busDirection));
			float yAngle = atan2(busDirection.x, busDirection.y);

			_cameras[_mode]->getSceneObject()->setRotation(degToRad(-45.0f), yAngle, 0.0f);

			_cameras[_mode]->getSceneObject()->move(busDirection.x * -15.0f * 2.75f, 30.0f * 2.75f, busDirection.y * -15.0f * 2.75f);
			//_camera->getSceneObject()->move(_camera->getDirection() * -40.0f);
		}
	}

	//_camera->getSceneObject()->setRotationQuaternion(bus->getSceneObject()->getRotationQuaternion());
	//_camera->getSceneObject()->setTransformFromMatrix(bus->getSceneObject()->getGlobalTransformMatrix());
	//_camera->getSceneObject()->rotate(-45.0f, 0.0f, 0.0f);
	//_camera->getSceneObject()->move(0.0f, 10.0f, 0.0f);

	RenderData* renderData = new RenderData;
	renderData->camera = _cameras[_mode];
	renderData->framebuffer = _framebuffers[_mode];
	renderData->renderPass = RP_CUSTOM;

	renderData->MVMatrix = renderData->camera->getProjectionMatrix() * renderData->camera->getViewMatrix();

	RenderListElement tempRenderElement;

	/*for (RoadObject* roadObect : _sceneManager->getGraphicsManager()->getRoadObjects())
	{
		tempRenderElement.type = RET_SINGLE;

		tempRenderElement.model = roadObect->getModel();
		tempRenderElement.object = roadObect->getSceneObject();
		tempRenderElement.renderObject = roadObect;

		ModelNode* modelNode = tempRenderElement.renderObject->getModelRootNode();
		if (modelNode != nullptr) // todo: tu czasem jest null, trzeba zdiagnozowac dlaczego
		{
			Renderer::addStaticModelNodeToRenderList(modelNode, tempRenderElement, renderData->renderList, RP_CUSTOM, 0);
		}
	}*/

	for (RenderObject* roadObect : _sceneManager->getGraphicsManager()->getRenderObjects())
	{
		tempRenderElement.type = RET_SINGLE;

		tempRenderElement.model = roadObect->getModel();
		tempRenderElement.object = roadObect->getSceneObject();
		tempRenderElement.renderObject = roadObect;

		ModelNode* modelNode = tempRenderElement.renderObject->getModelRootNode();
		if (modelNode != nullptr) // todo: tu czasem jest null, trzeba zdiagnozowac dlaczego
		{
			Renderer::getInstance().addStaticModelNodeToRenderList(modelNode, tempRenderElement, renderData->renderList, RP_CUSTOM, 0);
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.7f);
	Renderer::getInstance().renderScene(renderData);
}
