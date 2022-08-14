#ifndef SKELETALANIMATIONCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONCOMPONENT_H_INCLUDED


#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class RAnimation;
struct AnimationNodeData;
class RAnimatedModel;


const int MAX_BONES = 100;


// animation without retargeting
class SkeletalAnimationComponent final : public Component
{
	private:
		RAnimation* _animation;
		RAnimatedModel* _animatedModel;

		std::vector<glm::mat4> _finalBoneMatrices;
		float _currentTime;

		int _startFrame;
		int _endFrame;
		int _animationTicksPerSecond;

		bool _play;

		std::string _boneWithLockedTranslation;
		int _boneWithLockedTranslationIndex;

		void onAttachedToScenObject() override;

		void calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent(RAnimation* animation);
		~SkeletalAnimationComponent();

		void update(float deltaTime) override;

		inline const std::vector<glm::mat4>& getFinalBoneMatrices() { return _finalBoneMatrices; }
		inline RAnimation* getAnimation() { return _animation; }

		inline const float getCurrentTime() { return _currentTime; }
		inline const int getStartFrame() { return _startFrame; }
		inline const int getEndFrame() { return _endFrame; }
		inline const int getAnimationTicksPerSecond() { return _animationTicksPerSecond; }
		inline const bool isPlay() { return _play; }
		inline const std::string& getBoneWithLockedTranslation() { return _boneWithLockedTranslation; }

		inline const float getAnimationDuration() { return _endFrame - _startFrame; }

		inline void setCurrentTime(float currentTime) { _currentTime = currentTime; }
		inline void setStartFrame(int startFrame) { _startFrame = startFrame; }
		inline void setEndFrame(int endFrame) { _endFrame = endFrame; }
		inline void setAnimationTicksPerSecond(int animationTicksPerSecond) { _animationTicksPerSecond = animationTicksPerSecond; }
		inline void setPlay(bool play) { _play = play; }
		void setBoneWithLockedTranslation(const std::string& boneName);

};


#endif // SKELETALANIMATIONCOMPONENT_H_INCLUDED
