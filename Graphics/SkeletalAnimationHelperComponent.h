#ifndef SKELETALANIMATIONHELPERCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONHELPERCOMPONENT_H_INCLUDED


#include <vector>

#include "../Scene/Component.h"


class RAnimation;
class RAnimatedModel;
struct AnimationNodeData;


struct BoneHelperData final
{
	SceneObject* helperObject;
	AnimationNodeData* animationNode;
};


class SkeletalAnimationHelperComponent final : public Component
{
	private:
		RAnimation* _animation;
		RAnimatedModel* _animatedModel;

		std::vector<BoneHelperData> _modelBoneHelperDatas;
		std::vector<BoneHelperData> _animationBoneHelperDatas;

		void onAttachedToScenObject() override;

		SceneObject* createBoneObject(AnimationNodeData* nodeData, const glm::vec4& color);
		void createBoneObjectForModel(AnimationNodeData* nodeData, SceneObject* parent);
		void createBoneObjectForAnimation(AnimationNodeData* nodeData, SceneObject* parent);

	public:
		SkeletalAnimationHelperComponent(RAnimation* animation, RAnimatedModel* animatedModel);
		~SkeletalAnimationHelperComponent();

		void update(float deltaTime) override;

};


#endif // SKELETALANIMATIONHELPERCOMPONENT_H_INCLUDED