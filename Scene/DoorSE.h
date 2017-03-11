#ifndef DOORSE_H_INCLUDED
#define DOORSE_H_INCLUDED

#include "Door.h"

enum RotationDir
{
    ERD_CCW = 0,
    ERD_CW
};

class DoorSE : virtual public Door
{
    public:
        DoorSE(SceneObject* sceneObject, glm::vec3 relPosition, RModel* model, PhysicalBodyConvexHull* body,
               RModel* armModel, PhysicalBodyConvexHull* armBody, ConstraintHinge* constraint1, ConstraintHinge* constraint2, RotationDir dir = ERD_CCW, char group = 1)
        : Door(sceneObject, relPosition, model, body, group),
        _arm1Model(armModel), _arm1Body(armBody), _constraint1(constraint1), _constraint2(constraint2), _rotationDir(dir)
        {
            //close();
        }

        DoorSE(SceneManager* sMgr, PhysicsManager* pMgr, PhysicalBodyConvexHull* chasisBody, glm::vec3 busPosition, std::string busName, std::string texturePath, XMLElement* element)
        : Door(sMgr, pMgr)
        {
            std::string armName(element->Attribute("armName"));
            std::string armModel(element->Attribute("arm"));
            float armMass = (float)atof(element->Attribute("armMass"));
            _group = (char)atoi(element->Attribute("group"));

            float arm1lowLimit = (float)atof(element->Attribute("arm1lowLimit"));
            float arm1highLimit = (float)atof(element->Attribute("arm1highLimit"));
            glm::vec2 arm1limits = XMLstringToVec2(element->Attribute("arm1limits"));

            std::string rotDir(element->Attribute("rotationDir"));

            //RotationDir rdir;

            if (rotDir == "CCW")
                _rotationDir = ERD_CCW;
            else
                _rotationDir = ERD_CW;

            glm::vec3 armPosition = XMLstringToVec3(element->Attribute("armPosition"));
            glm::vec3 armRelPos = glm::vec3(busPosition.x + armPosition.x, busPosition.y + armPosition.y, busPosition.z + armPosition.z);

            btVector3 armPivotA = XMLstringToBtVec3(element->Attribute("armPivotA"));
            btVector3 armPivotB = XMLstringToBtVec3(element->Attribute("armPivotB"));

            _arm1SceneObject = _sMgr->addSceneObject(armName);
            _arm1SceneObject->setPosition(armRelPos);

            std::string armPath = "Buses/" + busName + "/" + armModel;

            _arm1Model = ResourceManager::getInstance().loadModel(armPath, texturePath);
            RenderObject* armRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(_arm1Model));
            _arm1SceneObject->addComponent(armRender);

            btVector3 btArmPos(armRelPos.x, armRelPos.y, armRelPos.z);

            int collidesWith = COL_ENV | COL_TERRAIN;
            _arm1Body = _pMgr->createPhysicalBodyConvexHull(_arm1Model->getCollisionMesh(), _arm1Model->getCollisionMeshSize(), armMass,  btArmPos, btVector3(0,0,0), COL_DOOR, collidesWith);
            _arm1SceneObject->addComponent(_arm1Body);

            _constraint1 = _pMgr->createConstraintHinge(chasisBody, _arm1Body, armPivotA, armPivotB, btVector3(0,1,0), btVector3(0,1,0));
            _constraint1->getBulletConstraint()->setLimit(arm1limits.x,arm1limits.y);

            // arm 2

            std::string arm2Name(element->Attribute("arm2Name"));
            std::string arm2Model(element->Attribute("arm2"));
            float arm2Mass = (float)atof(element->Attribute("arm2Mass"));

            float arm2lowLimit = (float)atof(element->Attribute("arm2lowLimit"));
            float arm2highLimit = (float)atof(element->Attribute("arm2highLimit"));

            glm::vec3 arm2Position = XMLstringToVec3(element->Attribute("arm2Position"));
            glm::vec3 arm2RelPos = glm::vec3(busPosition.x + arm2Position.x, busPosition.y + arm2Position.y, busPosition.z + arm2Position.z);

            btVector3 arm2PivotA = XMLstringToBtVec3(element->Attribute("arm2PivotA"));
            btVector3 arm2PivotB = XMLstringToBtVec3(element->Attribute("arm2PivotB"));

            _arm2SceneObject = _sMgr->addSceneObject(arm2Name);
            _arm2SceneObject->setPosition(arm2RelPos);

            std::string arm2Path = "Buses/" + busName + "/" + arm2Model;

            _arm2Model = ResourceManager::getInstance().loadModel(arm2Path, texturePath);
            RenderObject* arm2Render = GraphicsManager::getInstance().addRenderObject(new RenderObject(_arm2Model));
            _arm2SceneObject->addComponent(arm2Render);

            btVector3 btArm2Pos(arm2RelPos.x, arm2RelPos.y, arm2RelPos.z);

            collidesWith = COL_NOTHING;
            _arm2Body = _pMgr->createPhysicalBodyConvexHull(_arm2Model->getCollisionMesh(), _arm2Model->getCollisionMeshSize(), arm2Mass,  btArm2Pos, btVector3(0,0,0), COL_DOOR, collidesWith);
            _arm2SceneObject->addComponent(_arm2Body);

            //_sceneObject->addChild(_arm2SceneObject);

            _constraint2 = _pMgr->createConstraintHinge(chasisBody, _arm2Body, arm2PivotA, arm2PivotB, btVector3(0,1,0), btVector3(0,1,0));

            // door model

            std::string doorName(element->Attribute("name"));
            std::string doorModel(element->Attribute("model"));
            float doorMass = (float)atof(element->Attribute("mass"));

            glm::vec3 doorPosition = XMLstringToVec3(element->Attribute("position"));
            glm::vec3 relativePos = glm::vec3(armPosition.x + doorPosition.x, armPosition.y + doorPosition.y, armPosition.z + doorPosition.z);

            btVector3 doorPivotA = XMLstringToBtVec3(element->Attribute("pivotA"));
            btVector3 doorPivotB = XMLstringToBtVec3(element->Attribute("pivotB"));

            _sceneObject = _sMgr->addSceneObject(doorName);
            _sceneObject->setPosition(relativePos);

            //_sceneObject->addChild(_arm1SceneObject);

            std::string doorPath = "Buses/" + busName + "/" + doorModel;

            _doorModel = ResourceManager::getInstance().loadModel(doorPath, texturePath);
            RenderObject* doorRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(_doorModel));
            _sceneObject->addComponent(doorRender);

            btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

            collidesWith = COL_ENV;
            PhysicalBodyConvexHull* _doorBody = _pMgr->createPhysicalBodyConvexHull(_doorModel->getCollisionMesh(), _doorModel->getCollisionMeshSize(), doorMass, btDoorPos, btVector3(0,0,0), COL_DOOR, collidesWith);
            _sceneObject->addComponent(_doorBody);

            _doorToArm1Hinge = _pMgr->createConstraintHinge(_arm1Body, _doorBody, doorPivotA, doorPivotB, btVector3(0,1,0), btVector3(0,1,0));

            if (rotDir == "CCW")
                _doorToArm1Hinge->getBulletConstraint()->setLimit(-1.9,0.0);
            else
                _doorToArm1Hinge->getBulletConstraint()->setLimit(0.0,1.9);

            btVector3 pivotC = XMLstringToBtVec3(element->Attribute("pivotC"));
            btVector3 pivotD = XMLstringToBtVec3(element->Attribute("pivotD"));

            _doorToArm2Hinge = _pMgr->createConstraintHinge(_arm2Body, _doorBody, pivotC, pivotD, btVector3(0,1,0), btVector3(0,1,0));

            open();
            //close();
        }

        virtual ~DoorSE()
        {
            if (_constraint1)
                _pMgr->removeConstraint(_constraint1);

            if (_constraint2)
                _pMgr->removeConstraint(_constraint2);

            _pMgr->removeConstraint(_doorToArm1Hinge);
            _pMgr->removeConstraint(_doorToArm2Hinge);

            if (_arm1Body)
                _pMgr->removePhysicalBody(_arm1Body);

            if (_arm1SceneObject)
                _sMgr->removeSceneObject(_arm1SceneObject);

            if (_arm2Body)
                _pMgr->removePhysicalBody(_arm2Body);

            if (_arm2SceneObject)
                _sMgr->removeSceneObject(_arm2SceneObject);
        }

        void open()
        {
            if (_rotationDir == ERD_CCW)
                _constraint1->getBulletConstraint()->enableAngularMotor(true, -1.9f, 2.15f);
            else
                _constraint1->getBulletConstraint()->enableAngularMotor(true, 1.9f, 2.15f);

            _state = EDS_OPENING;
        }

        void close()
        {
            if (_rotationDir == ERD_CCW)
                _constraint1->getBulletConstraint()->enableAngularMotor(true, 1.9f, 2.15f);
            else
                _constraint1->getBulletConstraint()->enableAngularMotor(true, -1.9f, 2.15f);

            _state = EDS_CLOSING;
        }

        void setRelativePosition(glm::mat4 matParent)
        {
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(matParent, scale, rotation, translation, skew, perspective);

            glm::mat4 pos = glm::translate(translation + _relPosition);

            glm::mat4 rot = glm::mat4_cast(rotation);



            glm::mat4 s = glm::scale(glm::vec3(1,1,1));

            _sceneObject->setLocalTransformMatrix(pos * rot * s);
        }

        RModel* getArmModel() { return _arm1Model; }
        PhysicalBodyConvexHull* getArmBody() { return _arm1Body; }

        ConstraintHinge* getConstraint1() { return _constraint1; }
        ConstraintHinge* getConstraint2() { return _constraint2; }

    protected:
        RModel*                 _arm1Model;
        RModel*                 _arm2Model;
        SceneObject*            _arm1SceneObject;
        SceneObject*            _arm2SceneObject;
        PhysicalBodyConvexHull* _arm1Body;
        PhysicalBodyConvexHull* _arm2Body;
        ConstraintHinge*        _constraint1;   // bus - arm
        ConstraintHinge*        _constraint2;   // arm - door
        ConstraintHinge*        _doorToArm1Hinge;
        ConstraintHinge*        _doorToArm2Hinge;
        RotationDir             _rotationDir;
};

#endif // DOORSE_H_INCLUDED
