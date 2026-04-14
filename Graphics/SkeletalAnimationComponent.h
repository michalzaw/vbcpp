#ifndef SKELETALANIMATIONCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONCOMPONENT_H_INCLUDED


#include <vector>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "../Scene/Component.h"

#include "../Utils/Helpers.hpp"


class RAnimation;
struct AnimationNodeData;
class RAnimatedModel;
class Bone;
struct BoneInfo;


const int MAX_BONES = 100;


struct AnimationState
{
	std::string name;
	RAnimation* animation;

	float currentTime;

	AnimationState()
		: name(""), animation(nullptr), currentTime(0.0f)
	{

	}

	inline void setAnimation(RAnimation* animation) { this->animation = animation != nullptr ? animation : this->animation; }
	inline RAnimation* getAnimation() { return animation; }

	inline void setCurrentTime(float currentTime) { this->currentTime = currentTime; }
	inline float getCurrentTime() { return currentTime; }

};


// animation without retargeting
class SkeletalAnimationComponent final : public Component
{
	VBCPP_COMPONENT(SkeletalAnimationComponent, CT_SKELETAL_ANIMATION)

	private:
		std::unordered_map<std::string, AnimationState> _animationStates;
		RAnimatedModel* _animatedModel;

		AnimationState* _currentAnimationState;
		AnimationState* _nextAnimationState;

		std::vector<glm::mat4> _finalBoneMatrices;
		bool _finalBoneMatricesIsCalculated;

		float _animationSpeed;

		bool _play;

		bool _lockRootBoneTranslation;
		std::string _rootBoneName;
		int _rootNodeIndex;

		bool _endToStartFrameBlending;
		float _endToStartFrameBlendingTime;

		float _stateBlendingDuration;
		float _stateBlndingTime;
		float _stateBlendingFactor;

		float _scale;

		std::vector<glm::mat4> _translationMatrices;

		glm::mat4 _rootBoneTranslation;

		void onAttachedToScenObject() override;

		void calculateModelBonesTranslations(const AnimationNodeData* nodeData);

		glm::mat4 calculateBoneTranslation(Bone* bone, const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator);
		glm::quat calculateBoneRotation(RAnimation* animation, Bone* bone, float currentTime);
		glm::quat calculateRotationInterpolated(RAnimation* animation, Bone* bone, float animationTime, float timeToEnd, float duration);

		void calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent();
		~SkeletalAnimationComponent();

		void update(float deltaTime) override;

		void recalculateAllBonesTransform();

		inline const std::unordered_map<std::string, AnimationState>& getAnimationStates() { return  _animationStates; }

		void addAnimationState(AnimationState&& animationState);
		void setCurrentAnimationState(const std::string& name);
		void setNextAnimationState(const std::string& name);
		inline AnimationState* getCurrentAnimationState() { return _currentAnimationState; }
		inline AnimationState* getNextAnimationState() { return _nextAnimationState; }

		const std::vector<glm::mat4>& getFinalBoneMatrices();

		inline const float getAnimationSpeed() { return _animationSpeed; }
		inline const bool isPlay() { return _play; }
		inline const bool isLockRootBoneTranslation() { return _lockRootBoneTranslation; }
		inline const std::string& getRootBone() { return _rootBoneName; }
		inline const bool isEndToStartFrameBlending() { return _endToStartFrameBlending; }
		inline const float getEndToStartFrameBlendingTime() { return _endToStartFrameBlendingTime; }
		inline const float getStateBlendingDuration() { return _stateBlendingDuration; }
		inline const float getScale() { return _scale; }

		inline void setAnimationSpeed(float animationSpeed) { _animationSpeed = animationSpeed; }
		inline void setPlay(bool play) { _play = play; }
		inline void setLockRootBoneTranslation(bool isLock) { _lockRootBoneTranslation = isLock; }
		void setRootBone(const std::string& boneName);
		void setEndToStartFrameBlending(bool endToStartFrameBlending) { _endToStartFrameBlending = endToStartFrameBlending; }
		void setEndToStartFrameBlendingTime(float endToStartFrameBlendingTime) { _endToStartFrameBlendingTime = endToStartFrameBlendingTime; }
		void setStateBlendingDuration(float stateBlendingDuration) { _stateBlendingDuration = stateBlendingDuration; }
		inline void setScale(float scale) { _scale = scale; }

		inline glm::mat4 getRootBoneTranslation() { return _rootBoneTranslation; }

		glm::vec3 getRootBonePositionInStartFrame();
		glm::vec3 getRootBonePositionInEndFrame();

};


#endif // SKELETALANIMATIONCOMPONENT_H_INCLUDED
