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
_maxSteerAngle(0.65f), _steerStep(0.0005f),
_brake(false), _accelerate(false),
_brakeForce(0.0f), _brakeForceStep(0.5f)
{
    std::cout << "Bus Konstruktor" << std::endl;

    loadXMLdata(filename);
}

Bus::~Bus()
{
    std::cout << "Bus Destruktor" << std::endl;

    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
        delete (*it);

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
    //PhysicalBodyConvexHull* chasisBody = 0;
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
            _sceneObject->getTransform()->setPosition(busPosition);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));

            _sceneObject->getTransform()->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );

            const char* cScale = child->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _sceneObject->getTransform()->setScale(scale);

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


            // PhysicalBodyConvexHull* wheel3Body = physMgr->createPhysicalBodyConvexHull(wheel3model->GetVertices(), wheel3model->GetQuantumOfVertices(), 1.0f, btVector3(3.0f,3,-3));

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
            // ConstraintHinge2* hinge1 = physMgr->createConstraintHinge2(wheel1Body, boxBody2, btVector3(1,3,1), btVector3(0,1,0), btVector3(1,0,0));

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


            wheelObj->getTransform()->setPosition(relativePos);

            // obracamy model kola jeżli jest po lewej stronie
            if (side == "right")
            {
                wheelSide = WS_RIGHT;
                //wheelObj->getTransform()->SetRotation(glm::vec3(0,DegToRad(45.0f),0));
            }
            else
                wheelSide = WS_LEFT;



            btVector3 btWheelPos(relativePos.x, relativePos.y, relativePos.z);

            RModel* wheel = ResourceManager::getInstance().loadModel(wheelModel, texturePath);
            RenderObject* wheelRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(wheel));

            wheelObj->addComponent(wheelRender);

            PhysicalBodyCylinder* wheelCyl = _pMgr->createPhysicalBodyCylinder(btVector3(width, radius, radius), mass, btWheelPos, X_AXIS);
            wheelCyl->getRigidBody()->setFriction(10.0f);
            wheelCyl->getRigidBody()->setRestitution(0.1f);
            wheelObj->addComponent(wheelCyl);
            //wheelCyl->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );

            //btVector3 suspension(0,0.5,0);
            //btVector3 hingePos(btWheelPos + suspension);
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

            std::string wheelName(child->Attribute("name"));
            std::string wheelModel(child->Attribute("model"));
            float mass = (float)atof(child->Attribute("mass"));

            glm::vec3 doorPosition = XMLstringToVec3(child->Attribute("position"));
            glm::vec3 relativePos = glm::vec3(busPosition.x + doorPosition.x, busPosition.y + doorPosition.y, busPosition.z + doorPosition.z);

            btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

            glm::vec3 busPivot = XMLstringToVec3(child->Attribute("pivotA"));
            glm::vec3 doorPivot = XMLstringToVec3(child->Attribute("pivotB"));


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
            _steeringWheelObject->getTransform()->setPosition(position);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));
            _steeringWheelObject->getTransform()->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );

            const char* cScale = child->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _steeringWheelObject->getTransform()->setScale(scale);

            _sceneObject->addChild(_steeringWheelObject);
        }
    }

}

void Bus::turnLeft()
{
    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle > -_maxSteerAngle)
            {
                w->currentAngle -= _steerStep;

                glm::vec3 rotation = _steeringWheelObject->getTransform()->getRotation();
                rotation.y += 0.005f;
                _steeringWheelObject->getTransform()->setRotation(rotation);
            }

            w->suspension->getBulletConstraint()->setLowerLimit(w->currentAngle);
            w->suspension->getBulletConstraint()->setUpperLimit(w->currentAngle);
        }
    }
}

void Bus::turnRight()
{
    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle < _maxSteerAngle)
            {
                w->currentAngle += _steerStep;


                glm::vec3 rotation = _steeringWheelObject->getTransform()->getRotation();
                rotation.y -= 0.005f;
                _steeringWheelObject->getTransform()->setRotation(rotation);
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
