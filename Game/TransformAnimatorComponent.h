#ifndef TRANSFORMANIMATORCOMPONENT_H_INCLUDED
#define TRANSFORMANIMATORCOMPONENT_H_INCLUDED


#include "../Scene/Component.h"

#include "../Scripting/Utils/LuaMacros.h"


class TransformAnimatorComponent final : public Component
{
	VBCPP_COMPONENT(TransformAnimatorComponent, CT_TRANSFORM_ANIMATOR)

	private:
        float _translationTime;
        float _rotationTime;
        float _rotationLocalTime;
        float _scaleTime;

        glm::vec3 _translationSpeed;
        glm::vec3 _rotationSpeed;
        glm::vec3 _rotationLocalSpeed;
        glm::vec3 _scaleSpeed;

	public:
		TransformAnimatorComponent();

		LUAF void move(const glm::vec3& deltaPosition, float time);
        LUAF void move(float dx, float dy, float dz, float time);
        LUAF void rotate(const glm::vec3& deltaRotation, float time);
        LUAF void rotate(float dx, float dy, float dz, float time);
        LUAF void rotateLocal(const glm::vec3& deltaRotation, float time);
        LUAF void rotateLocal(float dx, float dy, float dz, float time);
        LUAF void scale(const glm::vec3& scale, float time);
        LUAF void scale(float dx, float dy, float dz, float time);

        void update(float deltaTime) override;

};


#endif // TRANSFORMANIMATORCOMPONENT_H_INCLUDED
