#ifndef SKELETALANIMATIONCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONCOMPONENT_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class RAnimation;
struct AnimationNodeData;
class RAnimatedModel;


class SkeletalAnimationComponent final : public Component
{
	private:
		static const int MAX_BONES = 100;

		RAnimation* _animation;
		RAnimatedModel* _animatedModel;

		std::vector<glm::mat4> _finalBoneMatrices;
		float _currentTime;

		float _animationDuration;
		int _animationTicksPerSecond;

		bool _play;

		void onAttachedToScenObject() override;

		void calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent(RAnimation* animation);
		~SkeletalAnimationComponent();

		void update(float deltaTime) override;

		inline const std::vector<glm::mat4>& getFinalBoneMatrices() { return _finalBoneMatrices; }
		inline RAnimation* getAnimiation() { return _animation; }

		inline const float getCurrentTime() { return _currentTime; }
		inline const float getAnimationDuration() { return _animationDuration; }
		inline const int getAnimationTicksPerSecond() { return _animationTicksPerSecond; }
		inline const bool isPlay() { return _play; }

		void setCurrentTime(float currentTime) { _currentTime = currentTime; }
		void setAnimationDuration(float animationDuration) { _animationDuration = animationDuration; }
		void setAnimationTicksPerSecond(int animationTicksPerSecond) { _animationTicksPerSecond = animationTicksPerSecond; }
		void setPlay(bool play) { _play = play; }

};


#endif // SKELETALANIMATIONCOMPONENT_H_INCLUDED
