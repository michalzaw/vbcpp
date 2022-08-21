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
	private:
		RAnimation* _animation;
		RAnimatedModel* _animatedModel;

		std::vector<glm::mat4> _finalBoneMatrices;
		float _currentTime;

		int _startFrame;
		int _endFrame;
		int _animationTicksPerSecond;

		bool _play;

		bool _lockRootBoneTranslation;
		std::string _rootBoneName;
		int _rootNodeIndex;

		std::vector<glm::mat4> _translationMatrices;

		void onAttachedToScenObject() override;

		void calculateModelBonesTranslations(const AnimationNodeData* nodeData);

		glm::mat4 calculateBoneTranslation(Bone* bone, const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator);

		void calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent(RAnimation* animation);
		~SkeletalAnimationComponent();

		void update(float deltaTime) override;

		void recalculateAllBonesTransform();

		inline RAnimation* getAnimation() { return _animation; }
		void setAnimation(RAnimation* animation);

		inline const std::vector<glm::mat4>& getFinalBoneMatrices() { return _finalBoneMatrices; }

		inline const float getCurrentTime() { return _currentTime; }
		inline const int getStartFrame() { return _startFrame; }
		inline const int getEndFrame() { return _endFrame; }
		inline const int getAnimationTicksPerSecond() { return _animationTicksPerSecond; }
		inline const bool isPlay() { return _play; }
		inline const std::string& getRootBone() { return _rootBoneName; }
		inline const bool isLockRootBoneTranslation() { return _lockRootBoneTranslation; }

		inline const float getAnimationDuration() { return _endFrame - _startFrame; }

		inline void setCurrentTime(float currentTime) { _currentTime = currentTime; }
		inline void setStartFrame(int startFrame) { _startFrame = startFrame; }
		inline void setEndFrame(int endFrame) { _endFrame = endFrame; }
		inline void setAnimationTicksPerSecond(int animationTicksPerSecond) { _animationTicksPerSecond = animationTicksPerSecond; }
		inline void setPlay(bool play) { _play = play; }
		inline void setLockRootBoneTranslation(bool isLock) { _lockRootBoneTranslation = isLock; }
		void setRootBone(const std::string& boneName);

};


#endif // SKELETALANIMATIONCOMPONENT_H_INCLUDED
