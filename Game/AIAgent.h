#ifndef AIAGENT_H_INCLUDED
#define AIAGENT_H_INCLUDED


#include <glm/glm.hpp>

#include "../Scene/Component.h"


class PathComponent;


class AIAgent : public Component
{
	protected:
		float _speed;

		PathComponent* _currentPath;

		unsigned int _currentPointIndex;
		float _t;

		void reset();
		void move(const glm::vec3& point1, const glm::vec3& point2);

	public:
		AIAgent();

		void setSpeed(float speed);
		float getSpeed();

		void setCurrentPath(PathComponent* path);
		PathComponent* getCurrentPath() { return _currentPath; }

		void update(float deltaTime) override;
};


#endif // AIAGENT_H_INCLUDED
