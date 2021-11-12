#ifndef MIRRORCOMPONENT_H_INCLUDED
#define MIRRORCOMPONENT_H_INCLUDED


#include "CameraStatic.hpp"


class MirrorComponent : public CameraStatic
{
    private:
        std::string  _name;
        Framebuffer* _framebuffer;
        glm::vec3    _normalVector;

		float _renderingDistance;

    public:
        MirrorComponent(std::string name, float renderingDistance)
            : CameraStatic(CPT_PERSPECTIVE),
            _name(name), _renderingDistance(renderingDistance)
        {
            _type = CT_MIRROR;

            setWindowDimensions(256, 512);
            setViewAngle(degToRad(50.0f));
            setNearValue(0.1);
            setFarValue(_renderingDistance);

            _framebuffer = OGLDriver::getInstance().createFramebuffer();
            _framebuffer->addDepthRenderbuffer(256, 512);
            _framebuffer->addTexture(TF_RGBA, 256, 512);
            OGLDriver::getInstance().registerFramebufferForInitialization(_framebuffer);
        }

        Framebuffer* getFramebuffer()
        {
            return _framebuffer;
        }

        void setNormalVector(glm::vec3 normalVector)
        {
            _normalVector = normalVector;
        }

        glm::vec3 getNormalVector()
        {
            return _normalVector;
        }

        std::string getName()
        {
            return _name;
        }

        void calculateReflectionVectorAndRotateCamera(glm::vec3 mainCameraPosition)
        {
            glm::vec3 cameraToMirror = glm::normalize(getPosition() - mainCameraPosition);
            glm::vec3 reflection = glm::normalize(glm::reflect(cameraToMirror, glm::normalize(getSceneObject()->transformLocalVectorToGlobal(_normalVector))));

            //std::cout << getSceneObject()->getRotation().x << " " << getSceneObject()->getRotation().y << " " << getSceneObject()->getRotation().z << std::endl;

            _localDirection = _direction = reflection;
            _localRightVector = _rightVector = glm::normalize(glm::cross(_direction, getSceneObject()->getParent()->transformLocalVectorToGlobal(glm::vec3(0, 1, 0))));
            _localUpVector = _upVector = glm::normalize(glm::cross(_rightVector, _direction));
            _viewMatrixIs = false;
            _lookAtIs = false;
            _upVectorIs = true;
            _directionIs = true;
            _rightVectorIs = true;
            _localUpVectorIs = true;
            _localDirectionIs = true;
            _localRightVectorIs = true;
            _aabbIs = false;
        }

};


#endif // MIRRORCOMPONENT_H_INCLUDED
