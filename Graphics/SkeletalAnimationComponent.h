#ifndef SKELETALANIMATIONCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONCOMPONENT_H_INCLUDED


#include <vector>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "AnimationState.h";

#include "../Scene/Component.h"

#include "../Scripting/Utils/LuaMacros.h"

#include "../Utils/Helpers.hpp"


class RAnimation;
struct AnimationNodeData;
class RAnimatedModel;
class Bone;
struct BoneInfo;


const int MAX_BONES = 100;


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

		void onAttachedToScenObject() override;

		void calculateRootLoopDelta(AnimationState& animationState);

		void calculateModelBonesTranslations(const AnimationNodeData* nodeData);

		glm::mat4 calculateBoneTranslation(RAnimation* animation, Bone* bone, float currentTime, bool lockRootBoneTranslation,
										   const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator);
		glm::quat calculateBoneRotation(RAnimation* animation, Bone* bone, float currentTime);
		glm::quat calculateRotationInterpolated(RAnimation* animation, Bone* bone, float animationTime, float timeToEnd, float duration);

		void calculateBoneTransformInSingleAnimation(AnimationState* animationState, const std::string& nodeName, bool lockRootBoneTranslation,
													 const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator, glm::mat4& outTransform);
		void calculateBoneTransformWithAnimationStateBlending(AnimationState* currentAnimationState, AnimationState* nextAnimationState, const std::string& nodeName, bool lockRootBoneTranslation,
															  const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator, glm::mat4& outTransform);

		void calculateRootMotionInSingleAnimation(AnimationState* currentAnimationState, const AnimationNodeData* node, const glm::mat4& parentTransform,
												  const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator);
		void calculateRootMotionWithAnimationStateBlending(AnimationState* currentAnimationState, AnimationState* nextAnimationState, const AnimationNodeData* node, const glm::mat4& parentTransform,
														   const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator);

		void calculateBoneTransform(AnimationState* currentAnimationState, AnimationState* nextAnimationState, const AnimationNodeData* node,
									std::vector<glm::mat4>& outFinalBoneMatrices, bool lockRootBoneTranslation, bool rootMotion = true, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent();
		~SkeletalAnimationComponent();

		void update(float deltaTime) override;

		void recalculateAllBonesTransform();

		inline const std::unordered_map<std::string, AnimationState>& getAnimationStates() { return  _animationStates; }

		void addAnimationState(AnimationState&& animationState);
		LUAF void setCurrentAnimationState(const std::string& name);
		LUAF void setNextAnimationState(const std::string& name);
		LUAF inline AnimationState* getCurrentAnimationState() { return _currentAnimationState; }
		LUAF inline AnimationState* getNextAnimationState() { return _nextAnimationState; }

		LUAF const std::vector<glm::mat4>& getFinalBoneMatrices();

		LUAF inline const float getAnimationSpeed() { return _animationSpeed; }
		LUAF inline const bool isPlay() { return _play; }
		LUAF inline const bool isLockRootBoneTranslation() { return _lockRootBoneTranslation; }
		LUAF inline const std::string& getRootBone() { return _rootBoneName; }
		LUAF inline const bool isEndToStartFrameBlending() { return _endToStartFrameBlending; }
		LUAF inline const float getEndToStartFrameBlendingTime() { return _endToStartFrameBlendingTime; }
		LUAF inline const float getStateBlendingDuration() { return _stateBlendingDuration; }
		LUAF inline const float getScale() { return _scale; }

		LUAF inline void setAnimationSpeed(float animationSpeed) { _animationSpeed = animationSpeed; }
		LUAF inline void setPlay(bool play) { _play = play; }
		LUAF inline void setLockRootBoneTranslation(bool isLock) { _lockRootBoneTranslation = isLock; }
		LUAF void setRootBone(const std::string& boneName);
		LUAF void setEndToStartFrameBlending(bool endToStartFrameBlending) { _endToStartFrameBlending = endToStartFrameBlending; }
		LUAF void setEndToStartFrameBlendingTime(float endToStartFrameBlendingTime) { _endToStartFrameBlendingTime = endToStartFrameBlendingTime; }
		LUAF void setStateBlendingDuration(float stateBlendingDuration) { _stateBlendingDuration = stateBlendingDuration; }
		LUAF inline void setScale(float scale) { _scale = scale; }

		LUAF glm::vec3 getRootBonePositionInStartFrame(const std::string& stateName = "", bool withOffset = false);
		LUAF glm::vec3 getRootBonePositionInEndFrame(const std::string& stateName = "", bool withOffset = false);
		LUAF glm::vec3 getRootBonePositionInFrame(const AnimationState& animationState, int frame, bool withOffset = false);

};


#endif // SKELETALANIMATIONCOMPONENT_H_INCLUDED
