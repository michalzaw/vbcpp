#ifndef AIAGENT_H_INCLUDED
#define AIAGENT_H_INCLUDED


#include <glm/glm.hpp>

#include "../Scene/Component.h"


class PathComponent;
class SkeletalAnimationComponent;


class AIAgent : public Component
{
	protected:
		float _speed;

		PathComponent* _currentPath;
		SkeletalAnimationComponent* _skeletalAnimationComponent;

		float _speedInAnimation;
		float _baseAnimationTicksPerSecond;

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

		void setCurrentPath(PathComponent* path);
		PathComponent* getCurrentPath() { return _currentPath; }

		void update(float deltaTime) override;
};


#endif // AIAGENT_H_INCLUDED
