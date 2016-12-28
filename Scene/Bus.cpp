#include "Bus.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "../Utils/Math.h"

#include "../Graphics/GraphicsManager.h"
#include "../Utils/ResourceManager.h"

#include "../Utils/Helpers.hpp"

Bus::Bus(SceneManager* smgr, PhysicsManager* pmgr, std::string filename)
: _sMgr(smgr), _pMgr(pmgr), _sceneObject(0), _chasisBody(0),
_maxSteerAngle(0.55f), _steerStep(0.2f),
_brake(false), _accelerate(false),
_brakeForce(0.0f), _brakeForceStep(0.5f),
_steeringWheelObject(NULL), _driverPosition(0.0f, 0.0f, 0.0f),
_isEnableLights(false), _isEnableHeadlights(false)
{
    std::cout << "Bus Konstruktor" << std::endl;

    loadXMLdata(filename);
}

Bus::~Bus()
{
    std::cout << "Bus Destruktor" << std::endl;

    WheelList::iterator wit = _wheels.begin();

    for (; wit != _wheels.end(); ++wit)
        delete (*wit);

    DoorList::iterator dit = _doors.begin();

    for (; dit != _doors.end(); ++dit)
        delete (*dit);

    _headlights.clear();
}


void Bus::loadXMLdata(std::string busname)
{
    std::string filename = busname + "/config.xml";

    XMLDocument doc;
    doc.LoadFile( filename.c_str() );

    XMLElement* objElement = doc.FirstChildElement("object");

    std::cout << "XML DATA" << std::endl;

    std::string sObjName(objElement->Attribute("name"));

    std::cout<< sObjName << std::endl;

    RModel* busModel = 0;

    glm::vec3 busPosition = glm::vec3(0,0,0);

    std::string texturePath = busname + "/";

    _sceneObject = _sMgr->addSceneObject(sObjName);

    for (XMLElement* child = objElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        const char* ename = child->Name();

        // OBJECT TRANSFORM
        if (strcmp(ename,"transform") == 0)
        {
            std::cout << "XML: Transform" << std::endl;

            const char* cPosition = child->Attribute("position");
            busPosition = XMLstringToVec3(cPosition);
            _sceneObject->setPosition(busPosition);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));

            _sceneObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );

            const char* cScale = child->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _sceneObject->setScale(scale);

        }
        else // BODY DATA
        if (strcmp(ename,"body") == 0)
        {
            std::cout << "XML: Body data" << std::endl;

            std::string modelFile = std::string(child->Attribute("model"));
            std::string modelPath = busname + "/" + modelFile;
            texturePath += std::string(child->Attribute("textures")) + "/";

            busModel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            RenderObject* renderObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel));

            _sceneObject->addComponent(renderObj);

            // Tworzenie fizycznego obiektu karoserii
            const char* cMass = child->Attribute("mass");
            float fMass = (float)atof(cMass);

            btVector3 btPos(busPosition.x, busPosition.y, busPosition.z);


            if (busModel->getCollisionMeshSize() > 0)
            {
                _chasisBody = _pMgr->createPhysicalBodyConvexHull(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), fMass, btPos);
                _chasisBody->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );
                _sceneObject->addComponent(_chasisBody);

                btVector3 dirVec(0,0,1);
                btTransform tmpTransf = _chasisBody->getRigidBody()->getWorldTransform();
                btVector3 dir = tmpTransf.getBasis() * dirVec;

                std::cout << dir.x() << " " << dir.y() << " " << dir.z() << std::endl;
            }
            else
            {
                std::cout << "ERROR: Collision mesh not found in bus model!\n" << std::endl;
                return;
            }

        }
        else // WHEEL DATA
        if (strcmp(ename,"wheel") == 0)
        {
            std::cout << "XML: Wheel data" << std::endl;

            std::string wheelName(child->Attribute("name"));
            std::string wheelModel(child->Attribute("model"));
            std::string side(child->Attribute("side"));
            float mass = (float)atof(child->Attribute("mass"));
            float radius = (float)atof(child->Attribute("radius"));
            float width = (float)atof(child->Attribute("width"));

            WheelSide wheelSide;

            int steering = (int)atoi(child->Attribute("steering"));
            int powered = (int)atoi(child->Attribute("powered"));

            float stiffness = (float)atof(child->Attribute("stiffness"));
            float damping = (float)atof(child->Attribute("damping"));
            float brakeForce = (float)atof(child->Attribute("brakeForce"));

            SceneObject* wheelObj = _sMgr->addSceneObject(wheelName);


            glm::vec3 wheelPosition = XMLstringToVec3(child->Attribute("position"));
            glm::vec3 relativePos = glm::vec3(busPosition.x + wheelPosition.x, busPosition.y + wheelPosition.y, busPosition.z + wheelPosition.z);


            wheelObj->setPosition(relativePos);

            // obracamy model kola jeżli jest po lewej stronie
            if (side == "right")
            {
                wheelSide = WS_RIGHT;
            }
            else
                wheelSide = WS_LEFT;

            btVector3 btWheelPos(relativePos.x, relativePos.y, relativePos.z);

            std::string modelPath = busname + "/" + wheelModel;
            RModel* wheel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            RenderObject* wheelRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(wheel));

            wheelObj->addComponent(wheelRender);

            PhysicalBodyCylinder* wheelCyl = _pMgr->createPhysicalBodyCylinder(btVector3(width, radius, radius), mass, btWheelPos, X_AXIS);
            wheelCyl->getRigidBody()->setFriction(10.0f);
            wheelCyl->getRigidBody()->setRestitution(0.1f);
            wheelObj->addComponent(wheelCyl);

            ConstraintHinge2* hinge1 = _pMgr->createConstraintHinge2(_chasisBody, wheelCyl, btWheelPos, btVector3(0,1,0), btVector3(1,0,0));
            hinge1->setStiffness(2, stiffness);
            hinge1->setDamping(2, damping);
            hinge1->getBulletConstraint()->setLinearUpperLimit(btVector3(0,0,0.25));
            hinge1->getBulletConstraint()->setLinearLowerLimit(btVector3(0,0,-0.14));


            hinge1->getBulletConstraint()->setUpperLimit(0.0f);
            hinge1->getBulletConstraint()->setLowerLimit(0.0f);


            btVector3 lowerlimit;
            hinge1->getBulletConstraint()->getLinearLowerLimit(lowerlimit);
            std::cout << lowerlimit.x() << " " << lowerlimit.y() << " " << lowerlimit.z() << std::endl;

            btVector3 upperlimit;
            hinge1->getBulletConstraint()->getLinearUpperLimit(upperlimit);
            std::cout << upperlimit.x() << " " << upperlimit.y() << " " << upperlimit.z() << std::endl;

            Wheel* w = new Wheel;
            w->body = wheelCyl;
            w->suspension = hinge1;
            w->currentAngle = 0.0f;
            w->brakeForce = brakeForce;
            w->steering = steering;
            w->powered = powered;
            w->wheelSide = wheelSide;

            _wheels.push_back(w);
        }
        else // DOOR DATA
        if (strcmp(ename,"door") == 0)
        {
            std::cout << "XML: Door data" << std::endl;

            std::string doorName(child->Attribute("name"));
            std::string doorType(child->Attribute("type"));
            std::string doorModel(child->Attribute("model"));
            float mass = (float)atof(child->Attribute("mass"));

            glm::vec3 doorPosition = XMLstringToVec3(child->Attribute("position"));
            glm::vec3 relativePos = glm::vec3(busPosition.x + doorPosition.x, busPosition.y + doorPosition.y, busPosition.z + doorPosition.z);

            // poczatek IF
            btVector3 busPivot = XMLstringToBtVec3(child->Attribute("pivotA"));
            btVector3 doorPivot = XMLstringToBtVec3(child->Attribute("pivotB"));

            SceneObject* doorObj = _sMgr->addSceneObject(doorName);

            doorObj->setPosition(relativePos);

            std::string modelPath = busname + "/" + doorModel;

            RModel* dr = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            RenderObject* doorRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(dr));

            doorObj->addComponent(doorRender);

            btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

            PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(dr->getCollisionMesh(), dr->getCollisionMeshSize(), mass, btDoorPos);
            doorObj->addComponent(doorBody);

            ConstraintHinge* doorHinge = _pMgr->createConstraintHinge(_chasisBody, doorBody, busPivot, doorPivot, btVector3(0,1,0), btVector3(0,1,0));

            doorHinge->getBulletConstraint()->setLimit(-1.5,0);

            Door* d = 0;
            if (doorType == "s")
            {
                d = new Door(dr, doorBody, doorHinge);

                d->close();
                _doors.push_back(d);
            }


            //d->body = doorBody;
            //d->model = dr;
            //d->hinge = doorHinge;

            // koniec IF
        }
        else // STERING WHEEL
        if (strcmp(ename,"steering_wheel") == 0)
        {
            std::cout << "XML: Steering wheel" << std::endl;

            std::string modelFile = std::string(child->Attribute("model"));
            std::string modelPath = busname + "/" + modelFile;

            _steeringWheelObject = _sMgr->addSceneObject("steeringWheel");

            RModel* steeringWheelModel = ResourceManager::getInstance().loadModel(modelPath, "./");
            RenderObject* renderObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(steeringWheelModel));

            _steeringWheelObject->addComponent(renderObj);


            const char* cPosition = child->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);
            _steeringWheelObject->setPosition(position);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));
            _steeringWheelObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );

            const char* cScale = child->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _steeringWheelObject->setScale(scale);

            _sceneObject->addChild(_steeringWheelObject);
        }
        else // DRIVER
        if (strcmp(ename,"driver") == 0)
        {
            std::cout << "XML: Driver" << std::endl;

            const char* cPosition = child->Attribute("position");
            _driverPosition = XMLstringToVec3(cPosition);
        }
        else // LIGHT
        if (strcmp(ename,"light") == 0)
        {
            std::cout << "XML: Light" << std::endl;

            const char* cPosition = child->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cColor = child->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = child->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(child->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(child->Attribute("diffuseIntensity"));


            SceneObject* light = _sMgr->addSceneObject("busLight" + toString(_lights.size()));
            Light* lightComponent = GraphicsManager::getInstance().addPointLight(color, ambientIntensity,
                                                                                 diffuseIntensity,
                                                                                 LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            _sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableLights);

            _lights.push_back(lightComponent);
        }
        else // HEADLIGHTS
        if (strcmp(ename,"headlights") == 0)
        {
            std::cout << "XML: Headlights" << std::endl;

            std::string headlightName(child->Attribute("name"));
            const char* cPosition = child->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation = XMLstringToVec3(cRotation);

            const char* cColor = child->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = child->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(child->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(child->Attribute("diffuseIntensity"));
            float cutoff = (float)atof(child->Attribute("cutoff"));


            SceneObject* light = _sMgr->addSceneObject(headlightName);
            Light* lightComponent = GraphicsManager::getInstance().addSpotLight(color, ambientIntensity,
                                                                                diffuseIntensity, cutoff,
                                                                                LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            light->setRotation(rotation);
            _sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableHeadlights);

            _headlights.push_back(lightComponent);
        }
    }

}

glm::vec3 Bus::getDriverPosition()
{
    return _driverPosition;
}


void Bus::setIsEnableLights(bool is)
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


bool Bus::isEnableLights()
{
    return _isEnableLights;
}


void Bus::setIsEnableHeadlights(bool is)
{
    if (_headlights.size() > 0)
    {
        for (HeadlightsList::iterator i = _headlights.begin(); i != _headlights.end(); ++i)
        {
            (*i)->setIsActive(is);
        }

        _isEnableHeadlights = is;
    }
}

bool Bus::isEnableHeadlights()
{
    return _isEnableHeadlights;
}


void Bus::turnLeft(float dt)
{
    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle > -_maxSteerAngle)
            {
                w->currentAngle -= dt * _steerStep;

                if (_steeringWheelObject)
                {
                    /*glm::vec3 rotation = _steeringWheelObject->getTransform()->getRotation();
                    rotation.y += 0.005f;
                    _steeringWheelObject->getTransform()->setRotation(rotation);*/

                    _steeringWheelObject->rotate(0.0f, dt * _steerStep * 2 * PI / _maxSteerAngle, 0.0f);
                }
            }

            w->suspension->getBulletConstraint()->setLowerLimit(w->currentAngle);
            w->suspension->getBulletConstraint()->setUpperLimit(w->currentAngle);
        }
    }
}


void Bus::turnRight(float dt)
{
    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle < _maxSteerAngle)
            {
                w->currentAngle += dt * _steerStep;

                if (_steeringWheelObject)
                {
                    /*glm::vec3 rotation = _steeringWheelObject->getTransform()->getRotation();
                    rotation.y -= 0.005f;
                    _steeringWheelObject->getTransform()->setRotation(rotation);*/

                    _steeringWheelObject->rotate(0.0f, -dt * _steerStep * 2 * PI / _maxSteerAngle, 0.0f);
                }
            }

            w->suspension->getBulletConstraint()->setLowerLimit(w->currentAngle);
            w->suspension->getBulletConstraint()->setUpperLimit(w->currentAngle);
        }
    }
}


void Bus::accelerate()
{
    _accelerate = true;
    _brake = false;

    updatePhysics();
}


void Bus::brakeOn()
{
    _accelerate = false;
    _brake = true;

    updatePhysics();
}


void Bus::brakeOff()
{
    _brake = false;

    updatePhysics();
}


void Bus::idle()
{
    _accelerate = false;
    _brake = false;

    updatePhysics();
}


Door* Bus::getDoor(unsigned char doorIndex)
{
    if (doorIndex <= _doors.size()-1)
        return _doors[doorIndex];
    else
        return 0;
}

void Bus::doorOpenClose(unsigned char doorIndex)
{
    if (doorIndex <= _doors.size()-1)
    {
        if (_doors[doorIndex]->getState() == EDS_CLOSING)
            _doors[doorIndex]->open();
        else
        if (_doors[doorIndex]->getState() == EDS_OPENING)
             _doors[doorIndex]->close();
    }

}

/*
void Bus::closeDoor(unsigned char doorIndex)
{
    if (doorIndex <= _doors.size()-1)
        _doors[doorIndex]->close();
}
*/

void Bus::updatePhysics()
{

    // przyspieszanie
    if (_accelerate)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            if ( w->powered )
            {
                btTransform tmpDirection = w->body->getRigidBody()->getWorldTransform();
                btVector3 forwardDir = tmpDirection.getBasis().getColumn(0);

                w->body->getRigidBody()->applyTorque(forwardDir * 4.0f);
            }
        }
    }

    // hamowanie
    if (_brake)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            w->body->getRigidBody()->setDamping(0, w->brakeForce);
        }
    }
    else
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            w->body->getRigidBody()->setDamping(0, 0.0f);
        }
    }
}
