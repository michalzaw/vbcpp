#ifndef SKELETALANIMATIONCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONCOMPONENT_H_INCLUDED


#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class RAnimation;
struct AnimationNodeData;
class RAnimatedModel;
struct StaticModelNode;


class SkeletalAnimationComponent final : public Component
{
	private:
		static const int MAX_BONES = 100;

		RAnimation* _animation;

		std::vector<glm::mat4> _finalBoneMatrices;
		float _currentTime;
		float _deltaTime;

		RAnimatedModel* _animatedModel;

		std::vector<glm::mat4> _boneTransformMatricesInModel;
		std::vector<glm::mat4> _boneTransformMatricesInAnimation;
		std::vector<glm::mat4> _boneTransformOffsetsMatrices;

		void onAttachedToScenObject() override;

		std::string mapAnimationNodeNameToBoneNameInModel(const std::string& animationNodeName);


		void calculateDefaultBoneTransformInModel(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));
		void calculateDefaultBoneTransformInAnimation(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));


		void calculateBoneTransform(const StaticModelNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f));
		void calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform = glm::mat4(1.0f));

	public:
		SkeletalAnimationComponent(RAnimation* animation, const std::unordered_map<std::string, std::string>& animationNodeNameToBoneNameInModelMap);
		~SkeletalAnimationComponent();

		void update(float deltaTime) override;

		inline const std::vector<glm::mat4>& getFinalBoneMatrices() { return _finalBoneMatrices; }

		std::unordered_map<std::string, std::string> _animationNodeNameToBoneNameInModelMap; // todo: animation - pirvate

};


#endif // SKELETALANIMATIONCOMPONENT_H_INCLUDED
