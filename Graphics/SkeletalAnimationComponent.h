#ifndef SKELETALANIMATIONCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONCOMPONENT_H_INCLUDED


#include <vector>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


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
		RAnimation* _animation;
		RAnimatedModel* _animatedModel;

		std::vector<glm::mat4> _finalBoneMatrices;
		bool _finalBoneMatricesIsCalculated;
		float _currentTime;

		float _animationSpeed;

		bool _play;

		bool _lockRootBoneTranslation;
		std::string _rootBoneName;
		int _rootNodeIndex;

		bool _endToStartFrameBlending;
		float _blendingTime;

		float _scale;

		std::vector<glm::mat4> _translationMatrices;

		void onAttachedToScenObject() override;

		void calculateModelBonesTranslations(const AnimationNodeData* nodeData);

		glm::mat4 calculateBoneTranslation(Bone* bone, const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator);
		glm::mat4 calculateBoneRotation(Bone* bone);
		glm::mat4 calculateRotationInterpolated(Bone* bone, float animationTime, float timeToEnd, float duration);

		void calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent(RAnimation* animation);
		~SkeletalAnimationComponent();

		void update(float deltaTime) override;

		void recalculateAllBonesTransform();

		inline RAnimation* getAnimation() { return _animation; }
		void setAnimation(RAnimation* animation);

		const std::vector<glm::mat4>& getFinalBoneMatrices();

		inline const float getCurrentTime() { return _currentTime; }
		inline const float getAnimationSpeed() { return _animationSpeed; }
		inline const bool isPlay() { return _play; }
		inline const bool isLockRootBoneTranslation() { return _lockRootBoneTranslation; }
		inline const std::string& getRootBone() { return _rootBoneName; }
		inline const bool isEndToStartFrameBlending() { return _endToStartFrameBlending; }
		inline const float getBlendingTime() { return _blendingTime; }
		inline const float getScale() { return _scale; }

		inline void setCurrentTime(float currentTime) { _currentTime = currentTime; }
		inline void setAnimationSpeed(float animationSpeed) { _animationSpeed = animationSpeed; }
		inline void setPlay(bool play) { _play = play; }
		inline void setLockRootBoneTranslation(bool isLock) { _lockRootBoneTranslation = isLock; }
		void setRootBone(const std::string& boneName);
		void setEndToStartFrameBlending(bool endToStartFrameBlending) { _endToStartFrameBlending = endToStartFrameBlending; }
		void setBlendingTime(float blendingTime) { _blendingTime = blendingTime; }
		inline void setScale(float scale) { _scale = scale; }

		glm::vec3 getRootBonePositionInStartFrame();
		glm::vec3 getRootBonePositionInEndFrame();

};


#endif // SKELETALANIMATIONCOMPONENT_H_INCLUDED
