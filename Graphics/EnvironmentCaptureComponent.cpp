#include "EnvironmentCaptureComponent.h"


#include "../Scene/SceneObject.h"


EnvironmentCaptureComponent::EnvironmentCaptureComponent()
    : Component(CT_ENVIRONMENT_CAPTURE_COMPONENT),
    _environmentMap(NULL), _irradianceMap(NULL),
    _rotationMatrix(1.0f)
{

}


EnvironmentCaptureComponent::EnvironmentCaptureComponent(RTextureCubeMap* environmentMap, RTextureCubeMap* irradianceMap, RTextureCubeMap* specularIrradianceMap)
    : Component(CT_ENVIRONMENT_CAPTURE_COMPONENT),
    _environmentMap(environmentMap), _irradianceMap(irradianceMap), _specularIrradianceMap(specularIrradianceMap),
    _rotationMatrix(1.0f)
{
	if (_irradianceMap == NULL)
		_irradianceMap = _environmentMap;

	if (_specularIrradianceMap == NULL)
		_specularIrradianceMap = _environmentMap;
}


EnvironmentCaptureComponent::~EnvironmentCaptureComponent()
{

}


RTextureCubeMap* EnvironmentCaptureComponent::getEnvironmentMap()
{
    return _environmentMap;
}


RTextureCubeMap* EnvironmentCaptureComponent::getIrradianceMap()
{
	return _irradianceMap;
}


RTextureCubeMap* EnvironmentCaptureComponent::getSpecularIrradianceMap()
{
	return _specularIrradianceMap;
}


glm::mat4 EnvironmentCaptureComponent::getRotationMatrix()
{
    return _rotationMatrix;
}


void EnvironmentCaptureComponent::changedTransform()
{
    _rotationMatrix = glm::inverse(glm::mat4_cast(_object->getRotationQuaternion()));
}
