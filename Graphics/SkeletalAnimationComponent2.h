#ifndef SKELETALANIMATIONCOMPONENT2_H_INCLUDED
#define SKELETALANIMATIONCOMPONENT2_H_INCLUDED


#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class RAnimation;
struct AnimationNodeData;
class RAnimatedModel;
struct StaticModelNode;


class SkeletalAnimationComponent2 final : public Component
{
	public:
		static const int MAX_BONES = 100;

		RAnimation* _animation;

		std::vector<glm::mat4> _finalBoneMatrices;
		float _currentTime;

		float _animationDuration;
		int _animationTicksPerSecond;

		bool _play;

		RAnimatedModel* _animatedModel;

		std::unordered_map<std::string, std::string> _boneMap;
		std::vector<glm::mat4> _defaultTranslationBoneMatrices;
		std::vector<glm::mat4> _defaultRotationBoneMatrices;
		std::vector<AnimationNodeData*> _cachedModelNodes;

		void onAttachedToScenObject() override;

		void calculateDefaultRotationInModel(AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

		void calculateBoneTransformInModel(AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));
		void calculateBoneTransformInAnimation(AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent2(RAnimation* animation, const std::unordered_map<std::string, std::string>& animationNodeNameToBoneNameInModelMap);
		~SkeletalAnimationComponent2();

		void update(float deltaTime) override;

		inline const std::vector<glm::mat4>& getFinalBoneMatrices() { return _finalBoneMatrices; }
		inline const RAnimation* getAnimiation() { return _animation; }

		inline const float getCurrentTime() { return _currentTime; }
		inline const float getAnimationDuration() { return _animationDuration; }
		inline const int getAnimationTicksPerSecond() { return _animationTicksPerSecond; }
		inline const bool isPlay() { return _play; }

		void setCurrentTime(float currentTime) { _currentTime = currentTime; }
		void setAnimationDuration(float animationDuration) { _animationDuration = animationDuration; }
		void setAnimationTicksPerSecond(int animationTicksPerSecond) { _animationTicksPerSecond = animationTicksPerSecond; }
		void setPlay(bool play) { _play = play; }

};


#endif // SKELETALANIMATIONCOMPONENT2_H_INCLUDED