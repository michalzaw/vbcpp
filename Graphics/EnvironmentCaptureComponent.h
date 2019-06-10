#ifndef ENVIRONMENTCAPTURECOMPONENT_H_INCLUDED
#define ENVIRONMENTCAPTURECOMPONENT_H_INCLUDED


#include "../Scene/Component.h"

#include "../Utils/RTextureCubeMap.h"


class EnvironmentCaptureComponent : public Component
{
    private:
        RTextureCubeMap* _environmentMap;
        glm::mat4 _rotationMatrix;

    public:
        EnvironmentCaptureComponent();
        EnvironmentCaptureComponent(RTextureCubeMap* environmentMap);
        ~EnvironmentCaptureComponent();

        RTextureCubeMap* getEnvironmentMap();
        glm::mat4 getRotationMatrix();

        virtual void changedTransform();

};


#endif // ENVIRONMENTCAPTURECOMPONENT_H_INCLUDED
