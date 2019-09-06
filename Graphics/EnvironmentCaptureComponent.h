#ifndef ENVIRONMENTCAPTURECOMPONENT_H_INCLUDED
#define ENVIRONMENTCAPTURECOMPONENT_H_INCLUDED


#include "../Scene/Component.h"

#include "../Utils/RTextureCubeMap.h"


class EnvironmentCaptureComponent : public Component
{
    private:
        RTextureCubeMap* _environmentMap;
		RTextureCubeMap* _irradianceMap;
		RTextureCubeMap* _specularIrradianceMap;
        glm::mat4 _rotationMatrix;

    public:
        EnvironmentCaptureComponent();
        EnvironmentCaptureComponent(RTextureCubeMap* environmentMap, RTextureCubeMap* irradianceMap = NULL, RTextureCubeMap* specularIrradianceMap = NULL);
        ~EnvironmentCaptureComponent();

        RTextureCubeMap* getEnvironmentMap();
		RTextureCubeMap* getIrradianceMap();
		RTextureCubeMap* getSpecularIrradianceMap();
        glm::mat4 getRotationMatrix();

        virtual void changedTransform();

};


#endif // ENVIRONMENTCAPTURECOMPONENT_H_INCLUDED
