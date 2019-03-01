#include "EnvironmentCaptureComponent.h"


#include "../Scene/SceneObject.h"


EnvironmentCaptureComponent::EnvironmentCaptureComponent()
    : Component(CT_ENVIRONMENT_CAPTURE_COMPONENT),
    _environmentMap(NULL),
    _rotationMatrix(1.0f)
{

}


EnvironmentCaptureComponent::EnvironmentCaptureComponent(RTextureCubeMap* environmentMap)
    : Component(CT_ENVIRONMENT_CAPTURE_COMPONENT),
    _environmentMap(environmentMap),
    _rotationMatrix(1.0f)
{

}


EnvironmentCaptureComponent::~EnvironmentCaptureComponent()
{

}


RTextureCubeMap* EnvironmentCaptureComponent::getEnvironmentMap()
{
    return _environmentMap;
}


glm::mat4 EnvironmentCaptureComponent::getRotationMatrix()
{
    return _rotationMatrix;
}


void EnvironmentCaptureComponent::changedTransform()
{
    _rotationMatrix = glm::inverse(glm::mat4_cast(_object->getRotationQuaternion()));
}
