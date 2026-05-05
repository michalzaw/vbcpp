#ifndef AIAGENT_H_INCLUDED
#define AIAGENT_H_INCLUDED


#include <glm/glm.hpp>

#include "../../Scene/Component.h"


class PathComponent;
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

		void setSpeed(float speed);
		float getSpeed();

		inline void setMotionFromAnimation(bool motionFromAnimation) { _motionFromAnimation = motionFromAnimation; }
		inline bool isMotionFromAnimation() { return _motionFromAnimation; }

		void setCurrentPath(PathComponent* path);
		PathComponent* getCurrentPath() { return _currentPath; }

		void update(float deltaTime) override;
};


#endif // AIAGENT_H_INCLUDED
