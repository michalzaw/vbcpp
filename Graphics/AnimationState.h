#ifndef ANIMATIONSTATE_H_INCLUDED
#define ANIMATIONSTATE_H_INCLUDED


#include <string>

#include <glm/glm.hpp>

#include "../Scripting/Utils/LuaMacros.h"


class RAnimation;


struct AnimationState
{
	LUAV std::string name;
	RAnimation* animation;

	LUAV float currentTime;
	LUAV float previousTime;

	LUAV glm::vec3 rootLoopDeltaPosition;

	LUAV glm::vec3 previousRootPosition;
	LUAV glm::vec3 rootDeltaInLastFrame;

	AnimationState(const std::string& name = "", RAnimation* animation = nullptr, float currentTime = 0.0f)
		: name(name), animation(animation), currentTime(currentTime), previousTime(currentTime),
		previousRootPosition(0.0f), rootDeltaInLastFrame(0.0f)
	{

	}

	inline void setAnimation(RAnimation* animation) { this->animation = animation != nullptr ? animation : this->animation; }
	inline RAnimation* getAnimation() { return animation; }

	LUAF inline void setCurrentTime(float currentTime) { this->currentTime = currentTime; }
	LUAF inline float getCurrentTime() { return currentTime; }

};



#endif // ANIMATIONSTATE_H_INCLUDED
