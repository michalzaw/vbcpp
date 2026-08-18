#ifndef AIAGENT_H_INCLUDED
#define AIAGENT_H_INCLUDED


#include <glm/glm.hpp>

#include "PathComponent.h"

#include "../../Scene/Component.h"

#include "../../Scripting/Utils/LuaMacros.h"


class SkeletalAnimationComponent;


class AIAgent : public Component
{
	VBCPP_COMPONENT(AIAgent, CT_AI_AGENT)

	protected:
		float _speed;
		bool _motionFromAnimation;

		PathComponent* _currentPath;
		SkeletalAnimationComponent* _skeletalAnimationComponent;

		float _speedInAnimation;

		unsigned int _currentPointIndex;
		float _t;

		bool _isInitializedStartPosition;

		void reset();
		void move(const glm::vec3& point1, const glm::vec3& point2);

		void onAttachedToScenObject() override;

	public:
		AIAgent();

		LUAF void setSpeed(float speed);
		LUAF float getSpeed();

		LUAF inline void setMotionFromAnimation(bool motionFromAnimation) { _motionFromAnimation = motionFromAnimation; }
		LUAF inline bool isMotionFromAnimation() { return _motionFromAnimation; }

		LUAF void setCurrentPath(PathComponent* path);
		LUAF inline PathComponent* getCurrentPath() { return _currentPath; }

		void update(float deltaTime) override;
};


#endif // AIAGENT_H_INCLUDED
