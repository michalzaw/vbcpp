#include "PhysicalBodyGhost.h"

#include "../Game/GameConfig.h"

#include "../Scene/InternalHelperComponent.h"
#include "../Scene/SceneManager.h"


PhysicalBodyGhost::PhysicalBodyGhost(const btVector3& size)
	: PhysicalBody(PhysicalBodyType::GHOST, 0.0f),
    _size(size),
    _ghostObject(nullptr),
    _helperSceneObject(nullptr)
{
	updateBody();
}


PhysicalBodyGhost::~PhysicalBodyGhost()
{

}


void PhysicalBodyGhost::setSize(const btVector3& size)
{
    _size = size;

    _collShape.reset(new btBoxShape(_size * 0.5f));

    _ghostObject->setCollisionShape(_collShape.get());


    if (_helperSceneObject != nullptr)
    {
        _helperSceneObject->setScale(_size.x(), _size.y(), _size.z());
    }
}


const btVector3& PhysicalBodyGhost::getSize()
{
    return _size;
}


void PhysicalBodyGhost::update()
{
    _objectsBeginCollision.clear();
    _objectsEndCollision.clear();

    std::set<PhysicalBody*> objectsEndCollision = _collidesWith;

    for (int i = 0; i < _ghostObject->getNumOverlappingObjects(); ++i)
    {
        btCollisionObject* body = _ghostObject->getOverlappingObject(i);
        PhysicalBody* physicalBody = static_cast<PhysicalBody*>(body->getUserPointer());
        if (physicalBody != nullptr)
        {
            setCollisionWith(physicalBody);

            objectsEndCollision.erase(physicalBody);
        }
    }

    for (PhysicalBody* physicalBody : objectsEndCollision)
    {
        setNotCollisionWith(physicalBody);
    }
}


void PhysicalBodyGhost::changedTransform()
{
    if (_isUpdateTransformFromObject)
    {
        btTransform transf;

        const glm::mat4& m = _object->getGlobalTransformMatrix();
        transf.setFromOpenGLMatrix(&m[0][0]);

        _ghostObject->setWorldTransform(transf);
    }
}


void PhysicalBodyGhost::onAttachedToScenObject()
{
    if (_ghostObject)
    {
        _ghostObject->setUserPointer((PhysicalBody*) this);
    }

    if (GameConfig::getInstance().mode == GM_EDITOR)
    {
        // create editor helper
        _helperSceneObject = getSceneObject()->getSceneManager()->addSceneObject("editor#StopComponent helper");
        _helperSceneObject->setFlags(SOF_NOT_SELECTABLE | SOF_NOT_SELECTABLE_ON_SCENE | SOF_NOT_SERIALIZABLE);

        Material* material = new Material;
        material->shader = NOTEXTURE_MATERIAL;
        material->shininess = 96.0f;
        material->diffuseColor = glm::vec4(0.32f, 0.32f, 0.32f, 1.0f);

        Cube* cube = new Cube(1, material);
        cube->init();
        cube->setCastShadows(false);
        getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(cube, _helperSceneObject);

        _helperSceneObject->setScale(_size.x(), _size.y(), _size.z());

        getSceneObject()->addChild(_helperSceneObject);

        InternalHelperComponent* helper = new InternalHelperComponent(getSceneObject());
        _helperSceneObject->addComponent(helper);
    }
}


void PhysicalBodyGhost::updateBody()
{
    _collShape.reset(new btBoxShape(_size * 0.5f));

    _ghostObject = new btGhostObject;
    _ghostObject->setCollisionShape(_collShape.get());
    _ghostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), _position));

    _ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
}
