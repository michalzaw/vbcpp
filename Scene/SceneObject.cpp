#include "SceneObject.h"
#include "SceneManager.h"

#include "../Game/AIAgent.h"
#include "../Game/CameraControlComponent.h"
#include "../Game/GameLogicSystem.h"
#include "../Game/PathComponent.h"

#include "../Graphics/BezierCurve.h"
#include "../Graphics/SkeletalAnimationComponent.h"
#include "../Graphics/SkeletalAnimationComponent2.h"
#include "../Graphics/SkeletalAnimationHelperComponent.h"

#include "../Utils/GlmUtils.h"


SceneObject::SceneObject(std::string name, SceneManager* sceneManager, RObject* objectDefinition, SceneObject* parent)
    : _parent(parent),
    _id(0), _name(name), _isActive(true),
    _flags(0),
	_objectDefinition(objectDefinition),
    _sceneManager(sceneManager),
    _rotationMode(RM_QUATERNION),
    _position(0.0f, 0.0f, 0.0f),
    _rotation(0.0f, 0.0f, 0.0f),
    _scale(1.0f, 1.0f, 1.0f)
{
    if (_parent != NULL)
    {
        _parent->addChild(this);
    }

    changedTransform();

    LOG_INFO("Create SceneObject: " + _name);
}


SceneObject::~SceneObject()
{
    LOG_INFO("Destroy SceneObject: " + _name);

    for (std::vector<Component*>:: iterator i = _components.begin(); i != _components.end(); ++i)
    {
        switch ((*i)->getType())
        {
            case CT_RENDER_OBJECT:
            case CT_PREFAB:
                _sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(*i));
                break;

			case CT_ROAD_OBJECT:
                _sceneManager->getGraphicsManager()->removeRoadObject(static_cast<RoadObject*>(*i));
				break;

			case CT_TERRAIN:
                _sceneManager->getGraphicsManager()->removeTerrain(static_cast<Terrain*>(*i));
				break;

            case CT_CAMERA:
                _sceneManager->getGraphicsManager()->removeCamera(static_cast<CameraStatic*>(*i));
                break;

            case CT_LIGHT:
                _sceneManager->getGraphicsManager()->removeLight(static_cast<Light*>(*i));
                break;

            case CT_PHYSICAL_BODY:
                _sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(*i));
                break;

            case CT_SOUND:
                _sceneManager->getSoundManager()->removeSoundComponent(dynamic_cast<SoundComponent*>(*i));
                break;

            case CT_TREE_COMPONENT:
                delete *i;
                break;

            case CT_GRASS:
                _sceneManager->getGraphicsManager()->removeGrassComponent(static_cast<Grass*>(*i));
                break;

            case CT_BUS_STOP:
                _sceneManager->getBusStopSystem()->removeBusStop(static_cast<BusStopComponent*>(*i));
                break;

            case CT_ENVIRONMENT_CAPTURE_COMPONENT:
                _sceneManager->getGraphicsManager()->removeEnvironmetnCaptureComponent(static_cast<EnvironmentCaptureComponent*>(*i));
                break;

            case CT_MIRROR:
                _sceneManager->getGraphicsManager()->removeMirrorComponent(static_cast<MirrorComponent*>(*i));
                break;

            case CT_CLICKABLE_OBJECT:
                _sceneManager->getGraphicsManager()->removeClickableObject(static_cast<ClickableObject*>(*i));
                break;

			case CT_SKY:
                _sceneManager->getGraphicsManager()->removeSky(static_cast<Sky*>(*i));
				break;

			case CT_DISPLAY:
                _sceneManager->getGraphicsManager()->removeDisplayComponent(static_cast<DisplayComponent*>(*i));
				break;

			case CT_CAMERA_CONTROL:
				_sceneManager->getGameLogicSystem()->removeCameraControlComponent(static_cast<CameraControlComponent*>(*i));
				break;

            case CT_SHAPE_POLYGON:
                delete* i;
                break;

            case CT_CROSSROAD:
                _sceneManager->getGraphicsManager()->removeCrossroadComponent(static_cast<CrossroadComponent*>(*i));
                break;

            case CT_ROAD_INTERSECTION:
                _sceneManager->getGraphicsManager()->removeRoadIntersectionComponent(static_cast<RoadIntersectionComponent*>(*i));
                break;

            case CT_SKELETAL_ANIMATION:
                _sceneManager->getGraphicsManager()->removeSkeletalAnimation(static_cast<SkeletalAnimationComponent*>(*i));
                break;

            case CT_SKELETAL_ANIMATION_2:
                _sceneManager->getGraphicsManager()->removeSkeletalAnimation2(static_cast<SkeletalAnimationComponent2*>(*i));
                break;

            case CT_SKELETAL_ANIMATION_HELPER:
                _sceneManager->getGraphicsManager()->removeSkeletalAnimationHelper(static_cast<SkeletalAnimationHelperComponent*>(*i));
                break;

            case CT_BEZIER_CURVE:
                _sceneManager->getGraphicsManager()->removeBezierCurve(static_cast<BezierCurve*>(*i));
                break;

            case CT_AI_AGENT:
                _sceneManager->getGameLogicSystem()->removeAIAgent(static_cast<AIAgent*>(*i));
                break;

            case CT_PATH:
                _sceneManager->getGameLogicSystem()->removePathComponent(static_cast<PathComponent*>(*i));
                break;

        }
    }
}


void SceneObject::changedTransform()
{
    _localTransformMatrixIsCalculated = false;
    _localNormalMatrixIsCalculated = false;
    _globalTransformMatrixIsCalculated = false;
    _globalNormalMatrixIsCalculated = false;

    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->changedTransform();
    }

    for (std::vector<Component*>::iterator i = _components.begin(); i != _components.end(); ++i)
    {
        (*i)->changedTransform();
    }
}


void SceneObject::calculateLocalTransformMatrix() const
{
    glm::mat4 pos = glm::translate(_position);

    glm::mat4 rot;

    if (_rotationMode == RM_EULER_ANGLES)
    {
        rot = glm::rotate(_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rot *= glm::rotate(_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rot *= glm::rotate(_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    else
    {
        rot = glm::mat4_cast(_rotationQuaternion);
    }

    glm::mat4 s = glm::scale(_scale);

    _localTransformMatrix = pos * rot * s;
}


void SceneObject::calculateGlobalTransformMatrix() const
{
    _globalTransformMatrix = getLocalTransformMatrix();

    if (_parent != NULL)
    {
        _globalTransformMatrix = _parent->getGlobalTransformMatrix() * _globalTransformMatrix;
    }
}


void SceneObject::removeParent()
{
    if (_parent != NULL)
    {
        _parent->removeChild(this);
        _parent = NULL;
    }
}


bool SceneObject::hasParent()
{
    return static_cast<bool>(_parent);
}


SceneObject* SceneObject::getParent()
{
    return _parent;
}


void SceneObject::addChild(SceneObject* child)
{
    if (child == NULL)
    {
        return;
    }

    _childrens.push_back(child);

    child->removeParent();
    child->_parent = this;
}


bool SceneObject::removeChild(SceneObject* child)
{
    if (child == NULL)
    {
        return false;
    }

    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        if ((*i) == child)
        {
            child->_parent = NULL;
            _childrens.erase(i);

            return true;
        }
    }

    return false;
}


void SceneObject::removeAllChildren()
{
    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->_parent = NULL;
    }

    _childrens.clear();
}


void SceneObject::removeAllChildrenFromScene()
{
    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        _sceneManager->removeChildSceneObject(*i);
    }

    _childrens.clear();
}


std::list<SceneObject*>& SceneObject::getChildren()
{
    return _childrens;
}


void SceneObject::addComponent(Component* component)
{
    if (component != NULL)
    {
        _components.push_back(component);

        component->setSceneObject(this);

        component->changedTransform();
    }
}


void SceneObject::removeComponent(Component* component)
{
    for (std::vector<Component*>::iterator i = _components.begin(); i != _components.end(); ++i)
    {
        if (*i == component)
        {
            i = _components.erase(i);

			switch (component->getType())
            {
                case CT_RENDER_OBJECT:
                case CT_PREFAB:
                    _sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(component));
                    break;

				case CT_ROAD_OBJECT:
                    _sceneManager->getGraphicsManager()->removeRoadObject(static_cast<RoadObject*>(component));
					break;

				case CT_TERRAIN:
                    _sceneManager->getGraphicsManager()->removeTerrain(static_cast<Terrain*>(component));
					break;

                case CT_CAMERA:
                    _sceneManager->getGraphicsManager()->removeCamera(static_cast<CameraStatic*>(component));
                    break;

                case CT_LIGHT:
                    _sceneManager->getGraphicsManager()->removeLight(static_cast<Light*>(component));
                    break;

                case CT_PHYSICAL_BODY:
                    _sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(component));
                    break;

                case CT_SOUND:
                    _sceneManager->getSoundManager()->removeSoundComponent(dynamic_cast<SoundComponent*>(component));
                    break;

                case CT_TREE_COMPONENT:
                    delete component;
                    break;

                case CT_GRASS:
                    _sceneManager->getGraphicsManager()->removeGrassComponent(static_cast<Grass*>(component));
                    break;

                case CT_BUS_STOP:
                    _sceneManager->getBusStopSystem()->removeBusStop(static_cast<BusStopComponent*>(component));
                    break;

                case CT_ENVIRONMENT_CAPTURE_COMPONENT:
                    _sceneManager->getGraphicsManager()->removeEnvironmetnCaptureComponent(static_cast<EnvironmentCaptureComponent*>(component));
                    break;

                case CT_MIRROR:
                    _sceneManager->getGraphicsManager()->removeMirrorComponent(static_cast<MirrorComponent*>(component));
                    break;

                case CT_CLICKABLE_OBJECT:
                    _sceneManager->getGraphicsManager()->removeClickableObject(static_cast<ClickableObject*>(component));
                    break;

				case CT_SKY:
                    _sceneManager->getGraphicsManager()->removeSky(static_cast<Sky*>(component));
					break;

				case CT_DISPLAY:
                    _sceneManager->getGraphicsManager()->removeDisplayComponent(static_cast<DisplayComponent*>(component));
					break;

				case CT_CAMERA_CONTROL:
                    _sceneManager->getGameLogicSystem()->removeCameraControlComponent(static_cast<CameraControlComponent*>(component));
					break;

                case CT_SHAPE_POLYGON:
                    delete component;
                    break;

                case CT_CROSSROAD:
                    _sceneManager->getGraphicsManager()->removeCrossroadComponent(static_cast<CrossroadComponent*>(component));
                    break;

                case CT_ROAD_INTERSECTION:
                    _sceneManager->getGraphicsManager()->removeRoadIntersectionComponent(static_cast<RoadIntersectionComponent*>(component));
                    break;

                case CT_SKELETAL_ANIMATION:
                    _sceneManager->getGraphicsManager()->removeSkeletalAnimation(static_cast<SkeletalAnimationComponent*>(component));
                    break;

                case CT_SKELETAL_ANIMATION_2:
                    _sceneManager->getGraphicsManager()->removeSkeletalAnimation2(static_cast<SkeletalAnimationComponent2*>(component));
                    break;

                case CT_SKELETAL_ANIMATION_HELPER:
                    _sceneManager->getGraphicsManager()->removeSkeletalAnimationHelper(static_cast<SkeletalAnimationHelperComponent*>(component));
                    break;

                case CT_BEZIER_CURVE:
                    _sceneManager->getGraphicsManager()->removeBezierCurve(static_cast<BezierCurve*>(component));
                    break;

                case CT_AI_AGENT:
                    _sceneManager->getGameLogicSystem()->removeAIAgent(static_cast<AIAgent*>(component));
                    break;

                case CT_PATH:
                    _sceneManager->getGameLogicSystem()->removePathComponent(static_cast<PathComponent*>(component));
                    break;

            }

            return;
        }
    }
}


void SceneObject::setName(std::string name)
{
	_name = name;
}


void SceneObject::setIsActive(bool is)
{
    _isActive = is;
}


std::string SceneObject::getName()
{
    return _name;
}

unsigned int SceneObject::getId()
{
    return _id;
}


Component* SceneObject::getComponent(unsigned int index)
{
    if (index < _components.size())
        return _components[index];

    return NULL;
}


Component* SceneObject::getComponent(ComponentType type)
{
    for (int i = 0; i < _components.size(); ++i)
    {
        if (_components[i]->getType() == type)
            return _components[i];
    }

    return NULL;
}


bool SceneObject::isActive()
{
    if (_parent != NULL)
        return _parent->isActive() && _isActive;

    return _isActive;
}


void SceneObject::setFlags(unsigned int flags)
{
    _flags = flags;
}


void SceneObject::addFlag(SceneObjectFlags flag)
{
    _flags = _flags | flag;
}


unsigned int SceneObject::getFlags()
{
    return _flags;
}


RObject* SceneObject::getObjectDefinition()
{
	return _objectDefinition;
}


unsigned int SceneObject::getComponentsCount()
{
    return _components.size();
}


SceneManager* SceneObject::getSceneManager()
{
    return _sceneManager;
}


void SceneObject::setPosition(glm::vec3 position)
{
    _position = position;

    changedTransform();
}


void SceneObject::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}


void SceneObject::setRotation(glm::vec3 rotation)
{
    _rotation = rotation;
    _rotationQuaternion = glm::quat(rotation);

    _rotationMode = RM_EULER_ANGLES;

    changedTransform();
}


void SceneObject::setRotation(float x, float y, float z)
{
    setRotation(glm::vec3(x, y, z));
}


void SceneObject::setRotationQuaternion(glm::quat rotation)
{
    _rotationQuaternion = rotation;
    _rotation = glm::eulerAngles(rotation);

    _rotationMode = RM_QUATERNION;

    changedTransform();
}


void SceneObject::setRotationQuaternion(float x, float y, float z, float w)
{
    _rotationQuaternion.x = x;
    _rotationQuaternion.y = y;
    _rotationQuaternion.z = z;
    _rotationQuaternion.w = w;

    _rotationMode = RM_QUATERNION;

    changedTransform();
}


void SceneObject::setScale(glm::vec3 scale)
{
    _scale = scale;

    changedTransform();
}


void SceneObject::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}


void SceneObject::setScale(float scale)
{
    setScale(glm::vec3(scale, scale, scale));
}


void SceneObject::move(glm::vec3 deltaPosition)
{
    _position += deltaPosition;

    changedTransform();
}


void SceneObject::move(float dx, float dy, float dz)
{
    move(glm::vec3(dx, dy, dz));
}


void SceneObject::rotate(glm::vec3 deltaRotation)
{
    _rotation += deltaRotation;

    _rotationMode = RM_EULER_ANGLES;

    changedTransform();
}


void SceneObject::rotate(float dx, float dy, float dz)
{
    rotate(glm::vec3(dx, dy, dz));
}


void SceneObject::scale(glm::vec3 scale)
{
    _scale *= scale;

    changedTransform();
}


void SceneObject::scale(float x, float y, float z)
{
    scale(glm::vec3(x, y, z));
}


glm::vec3 SceneObject::getPosition() const
{
    return _position;
}


glm::vec3 SceneObject::getRotation() const
{
    return _rotation;
}


glm::quat SceneObject::getRotationQuaternion() const
{
    return _rotationQuaternion;
}


glm::vec3 SceneObject::getScale() const
{
    return _scale;
}


glm::mat4& SceneObject::getLocalTransformMatrix() const
{
    if (!_localTransformMatrixIsCalculated)
    {
        calculateLocalTransformMatrix();

        _localTransformMatrixIsCalculated = true;
    }

    return _localTransformMatrix;
}


glm::mat4& SceneObject::getLocalNormalMatrix() const
{
    if (!_localNormalMatrixIsCalculated)
    {
        _localNormalMatrix = glm::transpose(glm::inverse(getLocalTransformMatrix()));

        _localNormalMatrixIsCalculated = true;
    }

    return _localNormalMatrix;
}


glm::mat4& SceneObject::getGlobalTransformMatrix() const
{
    if (!_globalTransformMatrixIsCalculated)
    {
        calculateGlobalTransformMatrix();

        _globalTransformMatrixIsCalculated = true;
    }

    return _globalTransformMatrix;
}


glm::mat4& SceneObject::getGlobalNormalMatrix() const
{
    if (!_globalNormalMatrixIsCalculated)
    {
        _globalNormalMatrix = glm::transpose(glm::inverse(getGlobalTransformMatrix()));

        _globalNormalMatrixIsCalculated = true;
    }

    return _globalNormalMatrix;
}


void SceneObject::setTransformFromMatrix(const glm::mat4& transformMatrix, bool forceQuaternions/* = true*/)
{
    glm::vec3 translation;
    glm::quat orientation;
    glm::vec3 scale;
    GlmUtils::decomposeMatrix(transformMatrix, translation, orientation, scale);

    setPosition(translation);
    setScale(scale);

    if (_rotationMode == RM_QUATERNION || forceQuaternions)
    {
        setRotationQuaternion(orientation);
    }
    else
    {
        glm::vec3 rotationEulerAngles = glm::eulerAngles(orientation);
        setRotation(rotationEulerAngles);
    }
}
